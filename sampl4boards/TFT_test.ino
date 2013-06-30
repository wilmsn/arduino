/*
Testaufbau mit Arduino UNO + Ethernet Shield + 1.8" SPI Display
Verbindungen
Display ------  Arduino(Ethernet Shield)
1 GND    -----  GND
2 VCC    -----  5V
3
4
5
6 Reset  -----  Reset
7 A0     -----  9
8 SDA    -----  11
9 SCK    -----  13
10 CS    -----  5
11 SCK  (eingebaute SD)
12 MISO (eingebaute SD)
13 MOSI (eingebaute SD)
14 CS   (eingebaute SD)
15 LED + -----  5V
16 LED - -----  GND
*/
// TFT Display
#define sclk 13
#define mosi 11
#define cs 5 
#define dc 9
#define rst 8
// SD Karte
#define SD_CS 4    // 4 is CS for SD Card on ethernet shield
// Pin 10 is needed as CS for Ethernet Controler

// Color definitions
#define  BLACK           0x00,0x00,0x00
#define	BLUE            0x00,0x00,0xFF
#define	RED             255,0,0
//0xFF,0x00,0x00
#define	GREEN           0x00,0xFF,0x00
#define CYAN            0x00,0xFF,0xFF
#define MAGENTA         0xFF,0x00,0xFF
#define YELLOW          0xFF,0xFF,0x00
#define WHITE           0xFF,0xFF,0xFF

#include <TFT.h>
#include <SPI.h>
#include <SD.h>

// the file itself
File bmpFile;
File myFile;

// information we extract about the bitmap file
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;
int16_t screenHeight, screenWidth;

// Option 1: use any pins but a little slower
//ST7735 tft = ST7735(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
TFT screen = TFT(cs, dc, rst);

void setup(void) {
  // initialize the screen
  pinMode(SD_CS, OUTPUT);
  pinMode(cs, OUTPUT);
  screen.begin();
  screen.setRotation(1); // Optional fÃ¼r Portrait/Landscape Mode Wertebereich 0...3; Default 1
  screenWidth = screen.width();
  screenHeight = screen.height();
  
  // make the background white
  screen.background(WHITE);

  // Some simple text and graphics
  screen.stroke(RED);
  screen.setTextSize(2);
  screen.text("Test", 1, 15);  

  screen.stroke(GREEN);
  screen.setTextSize(3);
  screen.text("Test", 1, 35);  
  
  screen.stroke(BLUE);
  screen.setTextSize(4);
  screen.text("Test", 1, 65);  

  //a box of single pixel
  screen.fill(BLUE);
  screen.stroke(YELLOW);
  for(uint8_t x=10; x < 21; x++) {
    for (uint8_t y=110; y < 121; y++) {
      screen.point(x, y);
    }
  }  
  screen.stroke(CYAN);
  screen.line(0,100,screen.width(),100);

  screen.stroke(GREEN);
  screen.fill(RED);
  screen.rect(30,110,10,10);
  screen.noFill();
  screen.rect(50,110,10,10);
  
  screen.stroke(MAGENTA);
  screen.noFill();
  screen.circle(100,115,6);
  screen.fill(BLACK);
  screen.circle(120,115,6);
  delay(5000); // some time to lok at ....

  // set the text size, color (black) and write text to the screen in the top left corner
  screen.stroke(BLACK);
  screen.setTextSize(1);
  // make the background white
  screen.background(WHITE);
  if (!SD.begin(SD_CS)) {
    screen.text("No SD", 1, 1);  
  } else {
    screen.text("SD OK", 1, 1);  
    bmpFile = SD.open("test.bmp");
    if (! bmpFile) {
      screen.text("No BMPFILE", 1, 11);  
      // Testfile anlegen
      myFile = SD.open("example.txt", FILE_WRITE);
      myFile.close();
      if (SD.exists("example.txt")) {
        screen.text("No EXAMPLEFILE", 1, 21);  
      } else {
        screen.text("EXAMPLEFILE OK", 1, 21);  
      }
    } else {
      bmpdraw(bmpFile, 20, 20, 140, 108);
    }
  }
}

void loop() {
  
}

void bmpdraw(File f, int x, int y, int xmax, int ymax) {
  uint8_t r, g, b, i, j;
  // first step: reading bitmapheader
  if (! bmpReadHeader(bmpFile)) { 
    screen.text("bad bmp",x,y);
    return;
  }
  for (i=0; i< bmpHeight; i++) {  
    for (j=0; j<bmpWidth; j++) {
      b = bmpFile.read();
      g = bmpFile.read();
      r = bmpFile.read();
      if ( j < screenWidth & j <= xmax - x) {
        if ( i < screenHeight & i <= ymax - y) {
          screen.drawPixel(j+x,i+y,screen.newColor(r,g,b));
        }
      }
    }
  }
}

#define BUFFPIXEL 20
void bmpdraw1(File f, int x, int y) {
  screen.text("Zeichne BMP", 1, 21);  
  bmpFile.seek(bmpImageoffset);
  
  uint16_t p; 
  uint8_t g, b;
  int i, j;
  
  uint8_t sdbuffer[2 * BUFFPIXEL];  // 3 * pixels to buffer
  uint8_t buffidx = 2*BUFFPIXEL;

  screen.setAddrWindow(x, y, x+bmpWidth-1, y+bmpHeight-1);
  
  for (i=0; i< bmpHeight; i++) {  
    for (j=0; j<bmpWidth; j++) {
        sdbuffer[0] = bmpFile.read();
        sdbuffer[1] = bmpFile.read();
        sdbuffer[2] = bmpFile.read();
      b = sdbuffer[0];     // blue
      g = sdbuffer[1];     // green
      p = sdbuffer[2];     // red
      
      p >>= 3;
      p <<= 6;
      
      g >>= 2;
      p |= g;
      p <<= 5;
      
      b >>= 3;
      p |= b;
      screen.pushColor(p);
    }
  }
}

boolean bmpReadHeader(File f) {
   // read header
  uint32_t tmp;
  if (read16(f) != 0x4D42) {
    // magic bytes missing
    return false;
  }
  // read file size
  tmp = read32(f);  
  // read and ignore creator bytes
  read32(f);
  bmpImageoffset = read32(f);  
  // read DIB header
  tmp = read32(f);
  bmpWidth = read32(f);
  bmpHeight = read32(f);
  if (read16(f) != 1) return false;
  bmpDepth = read16(f);
  if (read32(f) != 0) return false;  // compression not supported!
  return true;
}
/*********************************************/

// These read data from the SD card file and convert them to big endian 
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f) {
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}


// LITTLE ENDIAN!
uint32_t read32(File f) {
  uint32_t d;
  uint16_t b;
 
  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}

