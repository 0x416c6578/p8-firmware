#include "display.h"
#include "pinoutP8.h"
#include "fastSPI.h"
#include "font.h"
#define LCD_BUFFER_SIZE 15000 //LCD Buffer set to 15kbytes (approx = 1/2 RAM) meaning you can write up to 7500 pixels into the buffer without having to run over

/*
     THE DISPLAY:
        ◀ 240 ▶
    ______..._______
   |                |
   |   12:32        |
   .   01/01/20     .  ▲
   .                . 240
   .                .  ▼
   |                |
   |                |
   |______..._______|

  The top left pixel is (0,0), with the bottom right being (239,239)
  Writing to the display is as simple as:

  uint8_t buf[4]; //Byte buffer
  sendSPICommand(0x2A); //Column address set
  buf[0] = 0x00;
  buf[1] = 50; //xStart = 50
  buf[2] = 0x00;
  buf[3] = 60; //yStart = 60
  writeSPI(buf, 4);
  sendSPICommand(0x2B); //Row address set
  buf[0] = 0x00;
  buf[1] = 50; //yStart = 50
  buf[2] = 0x00;
  buf[3] = 60; //yEnd = 60
  writeSPI(buf, 4);
  sendSPICommand(0x2C); //Memory write
  //Since the column / row addresses are inclusive, you must right 121 green half-words to RAM, NOT 100
  for (int i = 0; i < 121; i++){
    writeSPISingleByte(0b11111000);
    writeSPISingleByte(0b00000000);
  }

  However writing a single byte every time is very inefficient, so we want to create a buffer for the current window
  Using the much faster writeSPI function to write the data out in 255 byte chunks will speed up things significantly

*/


uint8_t lcdBuffer[LCD_BUFFER_SIZE + 4];
uint32_t windowArea = 0;
uint32_t windowWidth = 0;
uint32_t windowHeight = 0;

/*
   Initialize display
*/
void initDisplay() {
  uint8_t buf[25];
  pinMode(LCD_CS, OUTPUT); //Chip select
  pinMode(LCD_RS, OUTPUT); //Command/data select
  pinMode(LCD_RESET, OUTPUT); //Display reset
  pinMode(LCD_DET, OUTPUT); //?
  digitalWrite(LCD_CS, HIGH); //Disable display SPI communication (active low)
  digitalWrite(LCD_RS, HIGH); //Data/command selector
  //Reset display to get into known state
  digitalWrite(LCD_RESET, HIGH);
  delay(20);
  digitalWrite(LCD_RESET, LOW);
  delay(100);
  digitalWrite(LCD_RESET, HIGH);
  delay(100);
  preWrite();
  sendSPICommand(0x36); //Memory data access control
  /*
    Bit D7- Page Address Order (0=BTT, 1=TTB)
    Bit D6- Column Address Order (0=LTR, 1=RTL
    Bit D5- Page/Column Order (0=norm, 1=reverse)
    Note: Bits D7 to D5, alse refer to section 8.12 Address Control
    Bit D4- Line Address Order (0=TTB, 1=BTT
    Bit D3- RGB/BGR Order (0=RGB, 1=BGR)
    Bit D2- Display Data Latch Data Order (0=LTR, 1=RTL)
  */
  writeSPISingleByte(0b00000000); //(Defaults)
  sendSPICommand(0x3a); //Data colour coding
  /*
     D0-D2 = 101 means 16 bits per pixel
     Formatted as follows: 0bRRRRR GGGGGG BBBBB (16 bits)
  */
  writeSPISingleByte(0b00000101);
  sendSPICommand(0xb2); //Porch control
  writeSPISingleByte(0b00001100);
  writeSPISingleByte(0b00001100);
  writeSPISingleByte(0b00000000);
  writeSPISingleByte(0b00110011);
  writeSPISingleByte(0b00110011);
  sendSPICommand(0xb7); //Gate control
  writeSPISingleByte(0b00110101);
  sendSPICommand(0xbb); //VCOM setting
  writeSPISingleByte(0b00011001);
  sendSPICommand(0xc0); //LCM control
  writeSPISingleByte(0b00101100);
  sendSPICommand(0xc2); //VDV and VRH command enable
  writeSPISingleByte(0b00000001);
  sendSPICommand(0xc3); //VRH set
  writeSPISingleByte(0b00010010);
  sendSPICommand(0xc4); //VDV set
  writeSPISingleByte(0b00100000);
  sendSPICommand(0xc6); //Framerate control in normal mode
  writeSPISingleByte(0b00001111);
  sendSPICommand(0xd0); //Power control stuff
  buf[0] = 0b10100100;
  buf[1] = 0b10100001;
  writeSPI(buf, 2);
  sendSPICommand(224); //Positive voltage gamma control
  uint8_t gammaPos[] = {208, 4, 13, 17, 19, 43, 63, 84, 76, 24, 13, 11, 31, 35};
  writeSPI(gammaPos, 14);
  sendSPICommand(225); //Negative voltage gamma control
  uint8_t gammaNeg[] = {208, 4, 12, 17, 19, 44, 63, 68, 81, 47, 31, 31, 32, 35};
  writeSPI(gammaNeg, 14);
  sendSPICommand(0x21); //Display inversion on
  sendSPICommand(0x11); //Sleep mode off (needs 5msec wait for voltage stabalization)
  delay(30);
  sendSPICommand(0x29); //Display on
  delay(30);
  clearDisplay();
  postWrite();
}

/*
   Write a string to the specified position
   If ignoreBlankPixels is true, the character write function will skip over blank pixels in a character
   This will greatly speed up writing a large window of text, but should only be used when the area you are writing to is blank
   This can be ensured by completly blanking the screen before writing
   TODO: implement dash '-' based wrapping for nicer string displaying
*/
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, String string, bool ignoreBlankPixels) {
  int currentLine = 0; //Current line
  int charPos = 0; //Position of the character we are on along the line
  for (int i = 0; i < string.length(); i++) { //Loop through every character of the string
    //If printing the next character would result in it being of screen
    if (x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos > 240 - FONT_WIDTH) {
      currentLine++;
      charPos = 0;
    }
    writeChar(x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos, y + currentLine * 8 * pixelsPerPixel, pixelsPerPixel, string[i], ignoreBlankPixels);
    charPos++;
  }
}


bool shouldPixelBeHere(uint32_t xPosInCharacter, uint32_t yPosInCharacter, uint8_t pixelsPerPixel, char character) {
  //uint8_t newChar[] = {0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
  return (font[character][xPosInCharacter] >> yPosInCharacter) & 1; //MIGHT NEED AN OFFSET HERE IF THE FONT CHANGES
  //return (newChar[xPosInCharacter] >> yPosInCharacter) & 1;
}

void writeChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, bool ignoreBlankPixels) {
  preWrite();
  uint16_t colour = 0b1111111111111111;

  //Width and height of the character on the display
  int characterDispWidth = FONT_WIDTH * pixelsPerPixel;
  int characterDispHeight = 8 * pixelsPerPixel;
  setRowColRAMAddr(x, y, characterDispWidth, characterDispHeight);

  bool pixelHere = false;
  //Row goes between 0 and 7, column goes between 0 and the font width
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < FONT_WIDTH; col++) {
      pixelHere = shouldPixelBeHere(col, row, pixelsPerPixel, character);
      setDisplayPixels(col, row, pixelsPerPixel, pixelHere, colour);
    }
  }
  sendSPICommand(0x2C);
  writeSPI(lcdBuffer, characterDispWidth * characterDispHeight * 2);
  postWrite();
}

void setDisplayPixels(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colour) {
  int columnFontIndexScaledByPixelCount = charColumn * pixelsPerPixel;
  int rowFontIndexScaledByPixelCount = charRow * pixelsPerPixel;
  int pixelsPerRow = FONT_WIDTH * pixelsPerPixel;
  for (int i = 0; i < pixelsPerPixel; i++) {
    for (int j = 0; j < pixelsPerPixel; j++) {
      lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j))] = pixelInCharHere ? (colour >> 8) & 0xFF : 0x00;
      lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j)) + 1] = pixelInCharHere ? colour & 0xFF : 0x00;
    }
  }
}

/*
   We have a font size (number of lcd pixels per font pixel)
   A display pixel x offset from the character top-left corner (0 -> (FONT_WIDTH*pixelsPerPixel)-1)
   A display pixel y offset from the character top-left corner (0 -> (8*pixelsPerPixel)-1)
   A boolean saying whether there is a pixel there
   shouldPixelBeHere takes a font (x,y) 0 <= x < FONT_WIDTH, 0 <= y < 8, and returns a boolean as to whether there should be a pixel there
   We need a function that takes:
     The font pixel x,y
     The number of pixels per pixel
     A bool as to whether there is a pixel in (x,y) in the font or not
     The pixel colour (low priority)
   Which loops through pixelsPerPixel^2 times and writes to the correct TWO positions in the lcd buffer array the colour
   It may look like
     for i in 0->pixelsPerPixel-1
       for j in 0->pixelsPerPixel-1
   If the lcdBuffer was a single byte per pixel, we could get the element of the array that (x,y) represents by
     Scaling the font position up by the number of pixels per display pixel (x*pixelsPerPixel and y*pixelsPerPixel)
       This will give the origin of that pixel in the display character window
     We must then do the for loop at this point, adding i/j to the new origin positions as an offset so that we now have a coordinate in the display window for every sub-pixel of the current pixel we are on
     We can get the number of pixels in a row by doing pixelsInRow = FONT_WIDTH*pixelsPerPixel
     Now if the lcdBuffer was a single byte per pixel, we could just do
     lcdBuffer[newYAddOffset*pixelsInRow + newXAddOffset] to get the position in the buffer of the current pixel we are on
       This is because the display windows fills from top left, top row first, meaning that the offset of the current display pixel is just the y*pixelsPerRow, add the x value
     Since there are two bytes per pixel however, we must edit the expression that accesses the buffer to take this into account.
     So now the MSByte index of a pixel will be twice the current index, and the LSByte index will be that plus 1

   In explicit form, this looks like:
*/

/*
  void setDisplayPixels(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colour) {
    int columnFontIndexScaledByPixelCount = charColumn * pixelsPerPixel;
    int rowFontIndexScaledByPixelCount = charRow * pixelsPerPixel;
    int pixelsPerRow = FONT_WIDTH * pixelsPerPixel;
    int newXAddOffset, newYAddOffset;
    for (int i = 0; i < pixelsPerPixel; i++) {
      for (int j = 0; j < pixelsPerPixel; j++) {
        newXAddOffset = columnFontIndexScaledByPixelCount + j;
        newYAddOffset = rowFontIndexScaledByPixelCount + i;
        lcdBuffer[2*(newYAddOffset * pixelsPerRow + newXAddOffset)] = pixelInCharHere ? (colour >> 8) & 0xFF : 0x00;
        lcdBuffer[2*(newYAddOffset * pixelsPerRow + newXAddOffset) + 1] = pixelInCharHere ? colour & 0xFF : 0x00;
      }
    }
  }
*/

/*
   Draw a rect with origin x,y and width w, height h
*/
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour) {
  preWrite();
  setRowColRAMAddr(x, y, w, h);
  displayColour(colour);
  postWrite();
}

/*
   Set the column and row RAM addresses for writing to the display
   You must select a region in the LCD RAM to write pixel data to
   This region has an xStart, xEnd, yStart and yEnd address
   As you write half-words (pixels) over SPI, the RAM fills horizontally per row
*/
void setRowColRAMAddr(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
  uint8_t buf[4]; //Parameter buffer
  windowArea = w * h; //Calculate window area
  sendSPICommand(0x2A); //Column address set
  buf[0] = 0x00;
  buf[1] = x;
  buf[2] = 0x00;
  buf[3] = (x + w - 1);
  writeSPI(buf, 4);
  sendSPICommand(0x2B); //Row address set
  buf[0] = 0x00;
  buf[1] = y;
  buf[2] = 0x00;
  buf[3] = ((y + h - 1) & 0xFF);
  writeSPI(buf, 4);
}

/*
   Fill the current window with the colour specified (16 bit RGB 5-6-5)
*/
void displayColour(uint16_t colour) {
  sendSPICommand(0x2C); //Memory write
  uint32_t numberOfBytesToWriteToLCD;
  uint32_t numberBytesInWindowArea = (windowArea * 2);
  uint32_t lcdBufferSize = LCD_BUFFER_SIZE; //Size of LCD buffer
  //If we are comfortable that the number of bytes the current window will hold in a buffer is less than the max buffer size:
  if (numberBytesInWindowArea < lcdBufferSize)
    lcdBufferSize = numberBytesInWindowArea; //Set the buffer size to be that of the window area * 2 (number of bytes that the window would occupy)

  for (int i = 0; i <= lcdBufferSize; i++) { //Loop through buffer
    //Write every pixel (half-word) into the LCD buffer
    lcdBuffer[i++] = (colour >> 8) & 0xFF; //Post increment meaning that it first writes to position i, then increments i
    lcdBuffer[i] = colour & 0xFF; //Writes to the (now) position of i++
  }
  do {
    if (numberBytesInWindowArea >= LCD_BUFFER_SIZE)
      numberOfBytesToWriteToLCD = LCD_BUFFER_SIZE;
    else
      numberOfBytesToWriteToLCD = numberBytesInWindowArea;
    writeSPI(lcdBuffer, numberOfBytesToWriteToLCD);
    numberBytesInWindowArea -= numberOfBytesToWriteToLCD ;
  }
  while (numberBytesInWindowArea > 0);
}

/*
   Clear display
*/
void clearDisplay() {
  preWrite();
  drawRect(0, 0, 240, 240, 0x0000);
  postWrite();
}
