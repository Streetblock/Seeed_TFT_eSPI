rotation = m % 4;

writecommand(EPD_PNLSET);
switch (rotation)
{
case 0:
    writedata(0x0F);
    _width = _init_width;
    _height = _init_height;
    break;
case 1:
    writedata(0x0B);
    _width = _init_height;
    _height = _init_width;
    break;
case 2:
    writedata(0x03);
    _width = _init_width;
    _height = _init_height;
    break;
case 3:
    writedata(0x07);
    _width = _init_height;
    _height = _init_width;
    break;
}
