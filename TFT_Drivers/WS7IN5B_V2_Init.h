{
#ifdef TFT_BUSY
    pinMode(TFT_BUSY, INPUT);
#endif
#ifdef TFT_ENABLE
    pinMode(TFT_ENABLE, OUTPUT);
    digitalWrite(TFT_ENABLE, HIGH);
#endif

    digitalWrite(TFT_RST, LOW);
    delay(4);
    digitalWrite(TFT_RST, HIGH);
    delay(200);

    EPD_INIT();
}
