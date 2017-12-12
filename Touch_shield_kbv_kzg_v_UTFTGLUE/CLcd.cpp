#include "CLcd.h"
 
 SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang SD_SPI_CONFIGURATION==3
extern  uint16_t  dashboardZolte[];
extern  uint16_t  dashboardBiale[];
extern  uint16_t  infoBiale[];
extern  uint16_t  infoZolte[];
extern  uint16_t  mechanicBiale[]; 
extern  uint16_t  mechanicZolte[];

void CLcd::begin()
  {  
    Serial.println("start cldc begin");
    uint16_t tmp;
   name = "S6D0154";
    
     
    switch (Orientation) {      // adjust for different aspects
        case 0:   break;        //no change,  calibrated for PORTRAIT
        case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
        case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
        case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    }

   
    ts = TouchScreen(XP, YP, XM, YM, 300);     //call the constructor AGAIN with new values.

   InitLCD(3);
   show_Serial();
   setFont(SmallFont);
   clrScr();
   

    BOXSIZE = width() / 6;
   
   
     bool good = SD.begin(SD_CS);
    if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
    }
     initGUI();
    delay(300);
    Serial.println("koniec lcd begin");
    }

 
 void CLcd::stary_loop()
{
   

        // are we in top color box area ?
        if (ypos < BOXSIZE) {               //draw white border on selected color box
            oldcolor = currentcolor;

            if (xpos < BOXSIZE) {
                currentcolor = RED;
                setColor(WHITE);
                drawRect(0, 0, BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 2) {
                currentcolor = YELLOW;
                drawRect(BOXSIZE, 0, 2*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 3) {
                currentcolor = GREEN;
                drawRect(BOXSIZE * 2, 0, 3*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 4) {
                currentcolor = CYAN;
                drawRect(BOXSIZE * 3, 0, 4*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 5) {
                currentcolor = BLUE;
                drawRect(BOXSIZE * 4, 0, 5*BOXSIZE, BOXSIZE);
            } else if (xpos < BOXSIZE * 6) {
                currentcolor = MAGENTA;
                drawRect(BOXSIZE * 5, 0, 6*BOXSIZE, BOXSIZE);
            }

            if (oldcolor != currentcolor) { //rub out the previous white border
                if (oldcolor == RED) { setColor(RED);fillRect(0, 0, BOXSIZE, BOXSIZE );}
                if (oldcolor == YELLOW){ setColor(YELLOW); fillRect(BOXSIZE, 0, 2*BOXSIZE, BOXSIZE );}
                if (oldcolor == GREEN) { setColor(GREEN);fillRect(BOXSIZE * 2, 0, 3*BOXSIZE, BOXSIZE );}
                if (oldcolor == CYAN) { setColor(CYAN);fillRect(BOXSIZE * 3, 0, 4*BOXSIZE, BOXSIZE );}
                if (oldcolor == BLUE){ setColor(BLUE); fillRect(BOXSIZE * 4, 0, 5*BOXSIZE, BOXSIZE );}
                if (oldcolor == MAGENTA) { setColor(MAGENTA);fillRect(BOXSIZE * 5, 0, 6*BOXSIZE, BOXSIZE );}
            }
        }
        // are we in drawing area ?
        if (((ypos - PENRADIUS) > BOXSIZE) && ((ypos + PENRADIUS) <TFT_H)) {
           setColor(currentcolor);
            fillCircle(xpos, ypos, PENRADIUS);
        }
        // are we in erase area ?
        if (ypos > TFT_H - 15) {
            // press the bottom of the screen to erase
             setColor(BLACK);
            fillRect(0, BOXSIZE, TFT_W, TFT_H );
        }
        setColor(0xdddddd);
    fillRect(0, TFT_H - 15, TFT_W, TFT_H );
    
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
///////////////////////// init ekranow
ekranInfo=new CEkranInfo(this,EKRAN_INFO,_rozkazCallBack);
ekrany[EKRAN_INFO]=ekranInfo;
ekrany[EKRAN_INFO]->begin();
ekranDashboard=new CEkranDashboard(this,EKRAN_DASHBOARD,_rozkazCallBack);
ekrany[EKRAN_DASHBOARD]=ekranDashboard;
ekrany[EKRAN_DASHBOARD]->begin();
ekranDebug=new CEkranDebug(this,EKRAN_DEBUG,_rozkazCallBack);
ekrany[EKRAN_DEBUG]=ekranDebug;
ekrany[EKRAN_DEBUG]->begin();

//////////////////////////////////
zmienEkran(EKRAN_INFO);

	Serial.println("koniec cldc initGUI");
} 
 
 void CLcd::zmienEkran(uint8_t e)
 {
	 if(e>ILE_EKRANOW)
	 {
		 Serial.println("Blad CLcd::zmienEkran nie ma tylu ekranow.");
		 return;
	 }
  _ekran=e;
  przerysujEkran=true;
  Serial.println("zmiana ekranu");
 }
 
  uint16_t CLcd::loop(CWiatrak Wiatraki[], CKomora Komory[])
  {
	if(przerysujEkran)
		 { 
		  ekrany[_ekran]->RysujZTlem(Wiatraki,Komory);
      przerysujEkran=false;
		 }
	if(touch()==1)// byl touch
	{
		return ekrany[_ekran]->Touch(xpos,ypos);
				
	}
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

uint8_t CLcd::showBMP(char *nm, uint16_t x, uint16_t y)
{
  Serial.print(nm);
  Serial.print(" x=");
   Serial.print(x);
   Serial.print(" y=");
    Serial.println(y);
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

    if ((x >= width()) || (y >= height()))
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
        if ((x + w) >= width())       // Crop area to be loaded
            w = width() - x;
        if ((y + h) >= height())      //
            h = height() - y;

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
        setAddrWindow(x, y, x + w - 1, y + h - 1);
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
                            color = color565(r, g, b);
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
                pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        setAddrWindow(0, 0, width() - 1, height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}

//=========================================================
     void CLcd::drawMidpointCircle(uint16_t cx,uint16_t cy,uint16_t radius,uint16_t startAngle, uint16_t endangle)  {
        // Settings
        /*int startAngle = 1;
        int endangle = 360;
        int cx = 150;  // x axis value for the center of the circle
        int cy = 150;  // y axis value for the center of the circle
        int radius = 50;
		http://eduinf.waw.pl/inf/utils/002_roz/2008_23.php
*/
        // Standard Midpoint Circle algorithm
        uint16_t p = (5 - radius * 4) / 4;
        uint16_t x = 0;
        uint16_t y = radius;

        drawCirclePoints( cx, cy, x, 20, 1, 360);

        while (x <= y) {
            x++;
            if (p < 0) {
                p += 2 * x + 1;
            } else {
                y--;
                p += 2 * (x - y) + 1;
            }
            drawCirclePoints( cx, cy, x, y, startAngle, endangle);
        }
    }

    void CLcd::drawCirclePoints( uint16_t centerX, uint16_t centerY, uint16_t x, uint16_t y, uint16_t startAngle, uint16_t endAngle) 
	{

        // Calculate the angle the current point makes with the circle center
        uint16_t rad=atan2(y, x);
        uint16_t angle = (uint16_t)(rad * 4068) / 71; 
		setColor(255,255,255);
        // draw the circle points as long as they lie in the range specified
        if (x < y) {
            // draw point in range 0 to 45 degrees
            if (90 - angle >= startAngle && 90 - angle <= endAngle) {
                drawPixel( centerX - y, centerY - x);
            }

            // draw point in range 45 to 90 degrees
            if (angle >= startAngle && angle <= endAngle) {
               drawPixel( centerX - x, centerY - y);
            }

            // draw point in range 90 to 135 degrees
            if (180 - angle >= startAngle && 180 - angle <= endAngle) {
                drawPixel( centerX + x, centerY - y);
            }

            // draw point in range 135 to 180 degrees
            if (angle + 90 >= startAngle && angle + 90 <= endAngle) {
                drawPixel( centerX + y, centerY - x);
            }

            // draw point in range 180 to 225 degrees
            if (270 - angle >= startAngle && 270 - angle <= endAngle) {
               drawPixel( centerX + y, centerY + x);
            }

            // draw point in range 225 to 270 degrees
            if (angle + 180 >= startAngle && angle + 180 <= endAngle) {
                drawPixel( centerX + x, centerY + y);
            }

            // draw point in range 270 to 315 degrees
            if (360 - angle >= startAngle && 360 - angle <= endAngle) {
                drawPixel( centerX - x, centerY + y);
            }

            // draw point in range 315 to 360 degrees
            if (angle + 270 >= startAngle && angle + 270 <= endAngle) {
                drawPixel( centerX - y, centerY + x);
            }
        }
    }
//=========================================================

void CLcd::kopnietyKwadrat(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t g,int przesunX, int przesunY)
{
	for(uint16_t i=0;i<g;i++)
	{
		drawLine(x+i*przesunX,y+i*przesunY,x2+i*przesunX,y2+i*przesunY);
	}
	
}

/////////////////////////// utft geometry

void CLcd::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
  drawLine(x1, y1, x2, y2);
 drawLine(x2, y2, x3, y3);
 drawLine(x3, y3, x1, y1);
}

void CLcd::fillTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3)
{
  int32_t xs, xe;
  int16_t y, ly;

  if (y1 > y2)
  {
    swap(y1, y2); 
    swap(x1, x2);
  }
  if (y2 > y3)
  {
    swap(y3, y2);
    swap(x3, x2);
  }
  if (y1 > y2)
  {
    swap(y1, y2);
    swap(x1, x2);
  }
  
  if(y1 == y3)  // Single line triangles
  {
    xs = xe = x1;
    if(x2 < xs)     xs = x2;
    else if(x2 > xe)  xe = x2;
    if(x3 < xs)     xs = x3;
    else if(x3 > xe)  xe = x3;
   drawLine(xs, y1, xe-xs,y1);
    return;
  }
  
  // Upper part
  if (y2 == y3) ly = y2;
  else          ly = y2-1;
  
  for(y=y1; y<=ly; y++)
  {
    xs = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
    drawLine(xs, y, xe-xs,y);
  }
  
  // Lower part
  for(; y<=y3; y++)
  {
    xs = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
    xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
   drawLine(xs, y, xe-xs,y);
  }
}

void CLcd::drawArc(uint16_t x, uint16_t y, uint16_t r, int32_t startAngle, int32_t endAngle, int32_t thickness)
{
  int32_t rDelta =0;// -(thickness/2);
  int32_t px, py, cx, cy;

  startAngle -= 90;
  endAngle   -= 90;
  
  if (startAngle!=endAngle)
  {
    for (int32_t i=0; i<thickness; i++)
    {
      px = x + cos((startAngle*3.14)/180) * (r+rDelta+i);
      py = y + sin((startAngle*3.14)/180) * (r+rDelta+i);
      for (int32_t d=startAngle+1; d<endAngle+1; d++)
      {
        cx = x + cos((d*3.14)/180) * (r+rDelta+i);
        cy = y + sin((d*3.14)/180) * (r+rDelta+i);
       drawLine(px, py, cx, cy);
        px = cx;
        py = cy;
      }
    }
  }
  else
  {
    px = x + cos((startAngle*3.14)/180) * (r+rDelta);
    py = y + sin((startAngle*3.14)/180) * (r+rDelta);
    cx = x + cos((startAngle*3.14)/180) * (r-rDelta);
    cy = y + sin((startAngle*3.14)/180) * (r-rDelta);
    drawLine(px, py, cx, cy);
  }
}

void CLcd::drawPie(uint16_t x, uint16_t y, uint16_t r, int32_t startAngle, int32_t endAngle)
{
  int32_t px, py, cx, cy;

  startAngle -= 90;
  endAngle   -= 90;
  if (startAngle>endAngle)
    startAngle -= 360;
  
  px = x + cos((startAngle*3.14)/180) * r;
  py = y + sin((startAngle*3.14)/180) * r;
  drawLine(x, y, px, py);
  for (int32_t d=startAngle+1; d<endAngle+1; d++)
  {
    cx = x + cos((d*3.14)/180) * r;
    cy = y + sin((d*3.14)/180) * r;
   drawLine(px, py, cx, cy);
    px = cx;
    py = cy;
  }
  drawLine(x, y, px, py);
}

