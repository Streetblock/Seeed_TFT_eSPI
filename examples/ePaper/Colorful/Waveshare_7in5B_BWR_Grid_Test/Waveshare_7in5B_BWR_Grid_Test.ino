#include "driver.h"
#include "TFT_eSPI.h"
#ifdef EPAPER_ENABLE
EPaper epaper;
#endif
static const int kScreenW = 800;
static const int kScreenH = 480;
static const int kCell = 40;
static const int kCols = kScreenW / kCell; // 20
static const int kRows = kScreenH / kCell; // 12
static uint32_t cellColor(int row, int col)
{
    // Diagonales Muster: Jede Zeile ist um 1 versetzt
    int tile = (row + col) % 4; 

    switch (tile)
    {
    case 0:
        return TFT_WHITE;  // Entspricht deiner "1"
    case 1:
        return TFT_YELLOW; // Entspricht deiner "2"
    case 2:
        return TFT_RED;    // Entspricht deiner "3"
    default:
        return TFT_BLACK;  // Entspricht deiner "4"
    }
}
static uint32_t textColorForBg(uint32_t bg)
{
    // Requested contrast rule:
    // black text on white/red, white text on yellow/black.
    if (bg == TFT_WHITE || bg == TFT_RED)
        return TFT_BLACK;
    return TFT_WHITE;
}
void setup()
{
#ifdef EPAPER_ENABLE
    epaper.begin();
    epaper.setRotation(0);
    epaper.fillScreen(TFT_WHITE);
    epaper.setTextSize(1);
    epaper.setTextFont(2);
    epaper.setTextDatum(MC_DATUM);
    int n = 1;
    for (int row = 0; row < kRows; row++)
    {
        for (int col = 0; col < kCols; col++)
        {
            int x = col * kCell;
            int y = row * kCell;
            uint32_t bg = cellColor(row, col);
            uint32_t fg = textColorForBg(bg);
            epaper.fillRect(x, y, kCell, kCell, bg);
            epaper.setTextColor(fg, bg);
            epaper.drawNumber(n, x + (kCell / 2), y + (kCell / 2));
            n++;
        }
    }
    epaper.update();
#endif
}

void loop()
{
}
