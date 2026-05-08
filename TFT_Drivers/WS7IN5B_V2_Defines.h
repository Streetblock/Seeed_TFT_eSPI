#ifndef EPD_WIDTH
#define EPD_WIDTH 800
#endif

#ifndef EPD_HEIGHT
#define EPD_HEIGHT 480
#endif

#ifndef TFT_WIDTH
#define TFT_WIDTH EPD_WIDTH
#endif

#ifndef TFT_HEIGHT
#define TFT_HEIGHT EPD_HEIGHT
#endif

// 4bpp sprite lets us map WHITE/BLACK/RED from one drawing buffer.
#define EPD_COLOR_DEPTH 4
#define USE_BWRY_EPAPER

#define EPD_NOP 0xFF
#define EPD_PNLSET 0x00
#define EPD_DISPON 0x04
#define EPD_DISPOFF 0x02
#define EPD_SLPIN 0x07
#define EPD_SLPOUT 0xFF
#define EPD_PTLIN 0x91
#define EPD_PTLOUT 0x92
#define EPD_PTLW 0x90

#define TFT_SWRST 0xFF
#define TFT_CASET 0xFF
#define TFT_PASET 0xFF
#define TFT_RAMWR 0x13
#define TFT_RAMRD 0xFF
#define TFT_INVON EPD_DISPON
#define TFT_INVOFF EPD_DISPOFF

#define TFT_INIT_DELAY 0

#ifdef TFT_BUSY
#define CHECK_BUSY()                   \
    do                                 \
    {                                  \
        delay(10);                     \
        while (!digitalRead(TFT_BUSY)) \
        {                              \
            delay(10);                 \
        }                              \
    } while (0)
#else
#define CHECK_BUSY()
#endif

static inline bool ws7in5b_is_black(uint8_t idx)
{
    // BWR palette index for black in USE_BWRY_EPAPER mode.
    uint8_t c = (idx & 0x0F);
    // Test mapping: treat yellow as black on BW plane.
    return (c == 0x0F) || (c == 0x0B);
}

static inline bool ws7in5b_is_red(uint8_t idx)
{
    uint8_t c = (idx & 0x0F);
    // BWR palette indexes for red/yellow in USE_BWRY_EPAPER mode.
    // Test mapping: yellow is also sent to red plane.
    return (c == 0x06) || (c == 0x0B);
}

static inline uint8_t ws7in5b_read_nibble(const uint8_t* colors, size_t src_bytes_per_row, size_t row, size_t x)
{
    size_t bidx = row * src_bytes_per_row + (x >> 1);
    uint8_t b = colors[bidx];
    return (x & 1) ? (b & 0x0F) : ((b >> 4) & 0x0F);
}

#define EPD_INIT()                     \
    do                                 \
    {                                  \
        writecommand(0x01);            \
        writedata(0x07);               \
        writedata(0x07);               \
        writedata(0x3F);               \
        writedata(0x3F);               \
        writecommand(0x06);            \
        writedata(0x17);               \
        writedata(0x17);               \
        writedata(0x28);               \
        writedata(0x17);               \
        writecommand(0x04);            \
        delay(100);                    \
        CHECK_BUSY();                  \
        writecommand(0x00);            \
        writedata(0x0F);               \
        writecommand(0x61);            \
        writedata(EPD_WIDTH >> 8);     \
        writedata(EPD_WIDTH & 0xFF);   \
        writedata(EPD_HEIGHT >> 8);    \
        writedata(EPD_HEIGHT & 0xFF);  \
        writecommand(0x15);            \
        writedata(0x00);               \
        writecommand(0x50);            \
        writedata(0x11);               \
        writedata(0x07);               \
        writecommand(0x60);            \
        writedata(0x22);               \
    } while (0)

#define EPD_UPDATE()                   \
    do                                 \
    {                                  \
        writecommand(0x12);            \
        delay(100);                    \
        CHECK_BUSY();                  \
    } while (0)

#define EPD_SLEEP()                    \
    do                                 \
    {                                  \
        writecommand(0x02);            \
        CHECK_BUSY();                  \
        writecommand(0x07);            \
        writedata(0xA5);               \
    } while (0)

#define EPD_WAKEUP()                   \
    do                                 \
    {                                  \
        digitalWrite(TFT_RST, LOW);    \
        delay(4);                      \
        digitalWrite(TFT_RST, HIGH);   \
        delay(200);                    \
        EPD_INIT();                    \
    } while (0)

#define EPD_SET_WINDOW(x1, y1, x2, y2) \
    do                                  \
    {                                   \
    } while (0)

// 0x10 = B/W plane: bit 0 => black, bit 1 => white
#define EPD_PUSH_OLD_COLORS(w, h, colors)                                        \
    do                                                                            \
    {                                                                             \
        const uint16_t out_w = (uint16_t)(w);                                      \
        const uint16_t out_h = (uint16_t)(h);                                      \
        const uint16_t src_bytes_per_row = (uint16_t)((out_w + 1) / 2);            \
        const uint16_t dst_bytes_per_row = (uint16_t)((out_w + 7) / 8);            \
        writecommand(0x10);                                                       \
        for (uint16_t row = 0; row < out_h; row++)                                 \
        {                                                                         \
            for (uint16_t bc = 0; bc < dst_bytes_per_row; bc++)                   \
            {                                                                     \
                uint8_t out = 0;                                                  \
                for (uint8_t i = 0; i < 8; i++)                                   \
                {                                                                 \
                    uint16_t x = (uint16_t)(bc * 8 + i);                          \
                    uint8_t n = ws7in5b_read_nibble(colors, src_bytes_per_row, row, x); \
                    out <<= 1;                                                    \
                    out |= ws7in5b_is_black(n) ? 0 : 1;                           \
                }                                                                 \
                writedata(out);                                                   \
            }                                                                     \
        }                                                                         \
    } while (0)

// 0x13 = RED plane: bit 1 => red, bit 0 => not-red
#define EPD_PUSH_NEW_COLORS(w, h, colors)                                         \
    do                                                                            \
    {                                                                             \
        const uint16_t out_w = (uint16_t)(w);                                      \
        const uint16_t out_h = (uint16_t)(h);                                      \
        const uint16_t src_bytes_per_row = (uint16_t)((out_w + 1) / 2);            \
        const uint16_t dst_bytes_per_row = (uint16_t)((out_w + 7) / 8);            \
        writecommand(0x13);                                                       \
        for (uint16_t row = 0; row < out_h; row++)                                 \
        {                                                                         \
            for (uint16_t bc = 0; bc < dst_bytes_per_row; bc++)                   \
            {                                                                     \
                uint8_t out = 0;                                                  \
                for (uint8_t i = 0; i < 8; i++)                                   \
                {                                                                 \
                    uint16_t x = (uint16_t)(bc * 8 + i);                          \
                    uint8_t n = ws7in5b_read_nibble(colors, src_bytes_per_row, row, x); \
                    out <<= 1;                                                    \
                    out |= ws7in5b_is_red(n) ? 1 : 0;                             \
                }                                                                 \
                writedata(out);                                                   \
            }                                                                     \
        }                                                                         \
    } while (0)

#define EPD_PUSH_OLD_COLORS_FLIP(w, h, colors)                                    \
    do                                                                            \
    {                                                                             \
        const uint16_t out_w = (uint16_t)(w);                                      \
        const uint16_t out_h = (uint16_t)(h);                                      \
        const uint16_t src_bytes_per_row = (uint16_t)((out_w + 1) / 2);            \
        const uint16_t dst_bytes_per_row = (uint16_t)((out_w + 7) / 8);            \
        writecommand(0x10);                                                       \
        for (uint16_t row = 0; row < out_h; row++)                                 \
        {                                                                         \
            for (uint16_t bc = 0; bc < dst_bytes_per_row; bc++)                   \
            {                                                                     \
                uint8_t out = 0;                                                  \
                for (uint8_t i = 0; i < 8; i++)                                   \
                {                                                                 \
                    uint16_t x = (uint16_t)((out_w - 1) - (bc * 8 + i));          \
                    uint8_t n = ws7in5b_read_nibble(colors, src_bytes_per_row, row, x); \
                    out <<= 1;                                                    \
                    out |= ws7in5b_is_black(n) ? 0 : 1;                           \
                }                                                                 \
                writedata(out);                                                   \
            }                                                                     \
        }                                                                         \
    } while (0)

#define EPD_PUSH_NEW_COLORS_FLIP(w, h, colors)                                    \
    do                                                                            \
    {                                                                             \
        const uint16_t out_w = (uint16_t)(w);                                      \
        const uint16_t out_h = (uint16_t)(h);                                      \
        const uint16_t src_bytes_per_row = (uint16_t)((out_w + 1) / 2);            \
        const uint16_t dst_bytes_per_row = (uint16_t)((out_w + 7) / 8);            \
        writecommand(0x13);                                                       \
        for (uint16_t row = 0; row < out_h; row++)                                 \
        {                                                                         \
            for (uint16_t bc = 0; bc < dst_bytes_per_row; bc++)                   \
            {                                                                     \
                uint8_t out = 0;                                                  \
                for (uint8_t i = 0; i < 8; i++)                                   \
                {                                                                 \
                    uint16_t x = (uint16_t)((out_w - 1) - (bc * 8 + i));          \
                    uint8_t n = ws7in5b_read_nibble(colors, src_bytes_per_row, row, x); \
                    out <<= 1;                                                    \
                    out |= ws7in5b_is_red(n) ? 1 : 0;                             \
                }                                                                 \
                writedata(out);                                                   \
            }                                                                     \
        }                                                                         \
    } while (0)

#define EPD_SET_TEMP(temp)              \
    do                                  \
    {                                   \
    } while (0)
