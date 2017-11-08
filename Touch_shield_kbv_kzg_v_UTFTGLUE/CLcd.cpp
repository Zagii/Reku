#include "CLcd.h"
 UTFTGLUE tft(0x0154,A2,A1,A3,A4,A0);
 SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang SD_SPI_CONFIGURATION==3
extern  uint16_t  dashboardZolte[];
extern  uint16_t  dashboardBiale[];
extern  uint16_t  infoBiale[];
extern  uint16_t  infoZolte[];
extern  uint16_t  mechanicBiale[];
extern  uint16_t  mechanicZolte[];

void CLcd::begin()
  {  
    uint16_t tmp;
   name = "S6D0154";
    
     
    switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    }

   
    ts = TouchScreen(XP, YP, XM, YM, 300);     //call the constructor AGAIN with new values.

   tft.InitLCD(3);
   tft.setFont(SmallFont);
   tft.clrScr();
    show_tft();

    BOXSIZE = tft.width() / 6;
   
   
     bool good = SD.begin(SD_CS);
    if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
    }
     initGUI();
    delay(300);
    }

     void CLcd::show_tft(void)
{
  
}
 void CLcd::stary_loop()
{
   

        // are we in top color box area ?
        if (ypos < BOXSIZE) {               //draw white border on selected color box
            oldcolor = currentcolor;

            if (xpos < BOXSIZE) {
                currentcolor = RED;
                tft.setColor(WHITE);
                tft.drawRect(0, 0, BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 2) {
                currentcolor = YELLOW;
                tft.drawRect(BOXSIZE, 0, 2*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 3) {
                currentcolor = GREEN;
                tft.drawRect(BOXSIZE * 2, 0, 3*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 4) {
                currentcolor = CYAN;
                tft.drawRect(BOXSIZE * 3, 0, 4*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 5) {
                currentcolor = BLUE;
                tft.drawRect(BOXSIZE * 4, 0, 5*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 6) {
                currentcolor = MAGENTA;
                tft.drawRect(BOXSIZE * 5, 0, 6*BOXSIZE, BOXSIZE);
            }

            if (oldcolor != currentcolor) { //rub out the previous white border
                if (oldcolor == RED) { tft.setColor(RED);tft.fillRect(0, 0, BOXSIZE, BOXSIZE );}
                if (oldcolor == YELLOW){ tft.setColor(YELLOW); tft.fillRect(BOXSIZE, 0, 2*BOXSIZE, BOXSIZE );}
                if (oldcolor == GREEN) { tft.setColor(GREEN);tft.fillRect(BOXSIZE * 2, 0, 3*BOXSIZE, BOXSIZE );}
                if (oldcolor == CYAN) { tft.setColor(CYAN);tft.fillRect(BOXSIZE * 3, 0, 4*BOXSIZE, BOXSIZE );}
                if (oldcolor == BLUE){ tft.setColor(BLUE); tft.fillRect(BOXSIZE * 4, 0, 5*BOXSIZE, BOXSIZE );}
                if (oldcolor == MAGENTA) { tft.setColor(MAGENTA);tft.fillRect(BOXSIZE * 5, 0, 6*BOXSIZE, BOXSIZE );}
            }
        }
        // are we in drawing area ?
        if (((ypos - PENRADIUS) > BOXSIZE) && ((ypos + PENRADIUS) <TFT_H)) {
           tft.setColor(currentcolor);
            tft.fillCircle(xpos, ypos, PENRADIUS);
        }
        // are we in erase area ?
        if (ypos > TFT_H - 15) {
            // press the bottom of the screen to erase
              tft.setColor(BLACK);
            tft.fillRect(0, BOXSIZE, TFT_W, TFT_H );
        }
        tft.setColor(0xdddddd);
    tft.fillRect(0, TFT_H - 15, TFT_W, TFT_H );
    
}
void CLcd::show_Serial(void)
{
    Serial.print(F("Found "));
    Serial.print(name);
    Serial.println(F(" LCD driver"));
    Serial.print(F("ID=0x"));
    Serial.println(identifier, HEX);
    Serial.println("Screen is " + String(TFT_W) + "x" + String(TFT_H));
    Serial.println("Calibration is: ");
    Serial.println("LEFT = " + String(TS_LEFT) + " RT  = " + String(TS_RT));
    Serial.println("TOP  = " + String(TS_TOP)  + " BOT = " + String(TS_BOT));
    Serial.print("Wiring is: ");
    Serial.println(SwapXY ? "SWAPXY" : "PORTRAIT");
    Serial.println("YP=" + String(YP)  + " XM=" + String(XM));
    Serial.println("YM=" + String(YM)  + " XP=" + String(XP));
}

void CLcd::initGUI()
{
  
b1=CButtonWnd( &tft,0,250,150,70,40,"Czyste");

b2=CButtonWnd(&tft,1,250,100,70,40,"Rysowanie");
bplus=CButtonWnd(&tft,2,180,100,40,40,"Plus");
bminus=CButtonWnd(&tft,3,250,100,40,40,"Minus");
stary_loop();
bInfo=CButtonWnd(&tft,4,15,190,45,45,infoBiale,infoZolte);
bDashboard=CButtonWnd(&tft,5,80,190,45,45,dashboardBiale,dashboardZolte);
bDebug=CButtonWnd(&tft,6,145,190,45,45,mechanicBiale,mechanicZolte);
zmienEkran(EKRAN_INFO);
 }
       
  void CLcd::loopDyn()
  {
   }
   void CLcd::loopStat()
   {

    }
 void CLcd::zmienEkran(uint8_t e)
 {
  ekran=e;
 // tft.clrScr();
  RysujMenuDol();
 }
 void CLcd::  RysujMenuDol()
 {
  tft.setColor(0x000000);
  tft.drawLine(0,25,320,25);//<<<<<<<<<<<<<<<<<<<<<<,,gora
  //tft.fillRect(0,190,320,240);
  tft.setColor(255, 0, 0);
  tft.drawLine(0,185,320,185);
        
  switch(ekran)
  {
    case EKRAN_INFO:
      bInfo.zmienStan(STAN_AKTYWNY_WYBRANY);
      bDashboard.zmienStan(STAN_AKTYWNY);
      bDebug.zmienStan(STAN_AKTYWNY);
    break;
    case EKRAN_DASHBOARD:
      bInfo.zmienStan(STAN_AKTYWNY);
      bDashboard.zmienStan(STAN_AKTYWNY_WYBRANY);
      bDebug.zmienStan(STAN_AKTYWNY);
    break;
    case EKRAN_DEBUG:
      bInfo.zmienStan(STAN_AKTYWNY);
      bDashboard.zmienStan(STAN_AKTYWNY);
      bDebug.zmienStan(STAN_AKTYWNY_WYBRANY);
    break;
   }
 }

 uint8_t CLcd::loop(CWiatrak Wiatraki[], CKomora Komory[])
 {
  uint8_t odswiez=0;
  uint8_t ret=0;
  if(touch()==1)// byl touch
  {
     String s=String(100,DEC);
      String s2;
      String s3;
    switch(ekran)
    {
      case ekran_test: 
      if(b1.czyKlik(xpos,ypos))
      {ekran=ekran_debug;
      odswiez=2;}
      odswiez=1;
      break;
      case ekran_debug:
      if(bplus.czyKlik(xpos,ypos))
      {
        if(aw+25<=250)
        aw+=25;
        analogWrite(46,aw);
      }
       if(bminus.czyKlik(xpos,ypos))
      {
        if(aw-25>=0)
        aw-=25;
        analogWrite(46,aw);
      }
      s2=String(10*aw/25);
      s3= s +" rpm, pwm="+s2+" %";
      tft.setColor(0,0,0);
      tft.fillRect(15,90,150,130);
      tft.setTextColor(0xffffff);
      tft.print(s3.c_str(),20,100);
     
      Serial.println(s3);
      break;
      case ekran_prosty:
      break;
    case EKRAN_INFO:
      if(bDashboard.czyKlik(xpos,ypos))
      {
        zmienEkran(EKRAN_DASHBOARD);
        odswiez=1;
       }
      if(bDebug.czyKlik(xpos,ypos))
      {
      zmienEkran(EKRAN_DEBUG);
        odswiez=1;
       }
    break;
    case EKRAN_DASHBOARD:
       if(bInfo.czyKlik(xpos,ypos))
      {
        zmienEkran(EKRAN_INFO);
        odswiez=1;
       }
      if(bDebug.czyKlik(xpos,ypos))
      {
        zmienEkran(EKRAN_DEBUG);
        odswiez=1;
       }
    break;
    case EKRAN_DEBUG:
       if(bDashboard.czyKlik(xpos,ypos))
      {
        zmienEkran(EKRAN_DASHBOARD);
        odswiez=1;
       }
      if(bInfo.czyKlik(xpos,ypos))
      {
        zmienEkran(EKRAN_INFO);
        odswiez=1;
       }
    break;
    }
  }
  if(odswiez==2){b1.zmienStan(1);
                b2.zmienStan(0);}
  if(odswiez==1) /// rysowanie ekranu gdy trzeba odswiezyc
  {
    
    switch(ekran)
    {
      case ekran_test: 
      stary_loop();
 
      break;
      case ekran_debug:
      //   b1.zmienStan(1);
       //   b2.zmienStan(0);
       
    //   bplus.Rysuj();
    //   bminus.Rysuj();
    zmienEkran(EKRAN_INFO);
      break;
      case ekran_prosty:
      
      break;
    }
  }
  return ret;
 }

 int CLcd::touch()
 {
      
    tp = ts.getPoint();   //tp.x, tp.y are ADC values
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    pinMode(XP, OUTPUT);
    pinMode(YM, OUTPUT);


    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
    if (SwapXY != 1) SWAP(tp.x, tp.y);
       
        xpos = map(tp.x, TS_LEFT, TS_RT, 0, TFT_W);
        ypos = map(tp.y, TS_TOP, TS_BOT, 0, TFT_H);


       return 1;
    }
    return 0;
 }



uint16_t CLcd::read16(File& f) {
    uint16_t result;         // read little-endian
    result = f.read();       // LSB
    result |= f.read() << 8; // MSB
    return result;
}

uint32_t CLcd::read32(File& f) {
    uint32_t result;
    result = f.read(); // LSB
    result |= f.read() << 8;
    result |= f.read() << 16;
    result |= f.read() << 24; // MSB
    return result;
}

uint8_t CLcd::showBMP(char *nm, int x, int y)
{
    File bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    bmpFile = SD.open(nm);      // Parse BMP header
    bmpID = read16(bmpFile);    // BMP signature
    (void) read32(bmpFile);     // Read & ignore file size
    (void) read32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    (void) read32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    n = read16(bmpFile);        // # planes -- must be '1'
    bmpDepth = read16(bmpFile); // bits per pixel
    pos = read32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = read32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, *sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                        case 24:
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            color = tft.color565(r, g, b);
                            break;
                        case 16:
                            b = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (is565)
                                color = (r << 8) | (b);
                            else
                                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                            break;
                        case 1:
                        case 4:
                        case 8:
                            if (r == 0)
                                b = sdbuffer[buffidx++], r = 8;
                            color = palette[(b >> bitshift) & bitmask];
                            r -= bmpDepth;
                            b <<= bmpDepth;
                            break;
                    }
                    lcdbuffer[lcdidx] = color;

                }
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}

