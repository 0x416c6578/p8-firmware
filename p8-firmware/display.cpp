#include "headers/display.h"
#define LCD_BUFFER_SIZE 15000  //LCD Buffer set to 15kbytes (approx = 1/2 RAM) meaning you can write up to 7500 pixels into the buffer without having to run over

uint8_t lcdBuffer[LCD_BUFFER_SIZE + 4];
uint32_t windowArea = 0;
uint32_t windowWidth = 0;
uint32_t windowHeight = 0;

/*
  Initialize display
*/
void initDisplay() {
  uint8_t buf[25];
  pinMode(LCD_CS, OUTPUT);     //Chip select
  pinMode(LCD_RS, OUTPUT);     //Command/data select
  pinMode(LCD_RESET, OUTPUT);  //Display reset
  pinMode(LCD_DET, OUTPUT);    //?
  digitalWrite(LCD_CS, HIGH);  //Disable display SPI communication (active low)
  digitalWrite(LCD_RS, HIGH);  //Data/command selector
  //Reset display to get into known state
  digitalWrite(LCD_RESET, HIGH);
  delay(20);
  digitalWrite(LCD_RESET, LOW);
  delay(100);
  digitalWrite(LCD_RESET, HIGH);
  delay(100);
  preWrite();
  sendSPICommand(0x36);  //Memory data access control
  /*
    Bit D7- Page Address Order (0=BTT, 1=TTB)
    Bit D6- Column Address Order (0=LTR, 1=RTL
    Bit D5- Page/Column Order (0=norm, 1=reverse)
    Note: Bits D7 to D5, alse refer to section 8.12 Address Control
    Bit D4- Line Address Order (0=TTB, 1=BTT
    Bit D3- RGB/BGR Order (0=RGB, 1=BGR)
    Bit D2- Display Data Latch Data Order (0=LTR, 1=RTL)
  */
  writeSPISingleByte(0b00000000);  //(Defaults)
  sendSPICommand(0x3a);            //Data colour coding
  /*
    D0-D2 = 101 means 16 bits per pixel
    Formatted as follows: 0bRRRRR GGGGGG BBBBB (16 bits)
  */
  writeSPISingleByte(0b00000101);
  sendSPICommand(0xb2);  //Porch control
  writeSPISingleByte(0b00001100);
  writeSPISingleByte(0b00001100);
  writeSPISingleByte(0b00000000);
  writeSPISingleByte(0b00110011);
  writeSPISingleByte(0b00110011);
  sendSPICommand(0xb7);  //Gate control
  writeSPISingleByte(0b00110101);
  sendSPICommand(0xbb);  //VCOM setting
  writeSPISingleByte(0b00011001);
  sendSPICommand(0xc0);  //LCM control
  writeSPISingleByte(0b00101100);
  sendSPICommand(0xc2);  //VDV and VRH command enable
  writeSPISingleByte(0b00000001);
  sendSPICommand(0xc3);  //VRH set
  writeSPISingleByte(0b00010010);
  sendSPICommand(0xc4);  //VDV set
  writeSPISingleByte(0b00100000);
  sendSPICommand(0xc6);  //Framerate control in normal mode
  writeSPISingleByte(0b00001111);
  sendSPICommand(0xd0);  //Power control stuff
  buf[0] = 0b10100100;
  buf[1] = 0b10100001;
  writeSPI(buf, 2);
  sendSPICommand(224);  //Positive voltage gamma control
  uint8_t gammaPos[] = {208, 4, 13, 17, 19, 43, 63, 84, 76, 24, 13, 11, 31, 35};
  writeSPI(gammaPos, 14);
  sendSPICommand(225);  //Negative voltage gamma control
  uint8_t gammaNeg[] = {208, 4, 12, 17, 19, 44, 63, 68, 81, 47, 31, 31, 32, 35};
  writeSPI(gammaNeg, 14);
  sendSPICommand(0x21);  //Display inversion on
  sendSPICommand(0x11);  //Sleep mode off (needs 5msec wait for voltage stabalization)
  delay(30);
  sendSPICommand(0x29);  //Display on
  delay(30);
  clearDisplay();
  postWrite();
}

/* 
  Wake display
 */
void wakeDisplay() {
  preWrite();
  sendSPICommand(0x29);  //Display on
  sendSPICommand(0x11);  //Sleep out
  postWrite();
}

/* 
  Sleep display
 */
void sleepDisplay() {
  preWrite();
  sendSPICommand(0x28);  //Display off
  sendSPICommand(0x10);  //Sleep in
  postWrite();
}

/*
  Write a string to the specified position using a string literal (null terminated char array)
*/
void drawString(coord pos, uint8_t pixelsPerPixel, char* string, uint16_t colourFG, uint16_t colourBG) {
  int currentLine = 0;      //Current line
  int charPos = 0;          //Position of the character we are on along the line
  int i = 0;                //Character index
  while (string[i] != 0) {  //Loop through every character of the string (only stop when you reach the null terminator)
    drawChar({pos.x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos,
              pos.y + currentLine * FONT_HEIGHT * pixelsPerPixel},
             pixelsPerPixel, string[i], colourFG, colourBG);
    //Move to the next character
    charPos++;
    i++;
  }
}

/*
  Write an (up to 9 digit) integer to x,y, without preceding zeroes (useful when you know the numbers you are writing will have the same number of digits on rewriting)
  The logic for writing the string is basically the same as drawString
*/
void drawIntWithoutPrecedingZeroes(coord pos, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG, uint16_t colourBG) {
  if (toWrite == 0) {
    drawChar({pos.x, pos.y}, pixelsPerPixel, '0', colourFG, colourBG);
    return;
  }
  //Byte array for storing the digits
  uint8_t digits[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t i = 0;
  //Write the number (of num digits n) into the LAST n indexes of the digits array (everything preceding the number will be 0)
  while (toWrite) {
    digits[8 - i] = toWrite % 10;
    toWrite /= 10;
    i++;
  }
  //Find which index holds the first (non-zero) digit of the number
  for (i = 0; i < 9; i++) {
    if (digits[i] == 0)
      continue;
    break;
  }
  int currentLine = 0;  //Current line
  int charPos = 0;      //Position of the character we are on along the line
  char charToWrite[1];
  //Do the normal writing routine but for every digit of the number, starting from the first digit
  for (; i < 9; i++) {  //Loop through every character of the string
    //If printing the next character would result in it being of screen
    if (pos.x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos > 240 - FONT_WIDTH) {
      currentLine++;
      charPos = 0;
    }
    sprintf(charToWrite, "%d", digits[i]);
    drawChar({pos.x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos, pos.y + currentLine * FONT_HEIGHT * pixelsPerPixel}, pixelsPerPixel, charToWrite[0], colourFG, colourBG);
    charPos++;
  }
}

/*
  Write a number always with 9 digits to x,y (with preceding zeroes for variable length rewrites)
*/
void drawIntWithPrecedingZeroes(coord pos, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG, uint16_t colourBG) {
  if (toWrite == 0) {
    drawString({pos.x, pos.y}, pixelsPerPixel, "000000000", colourFG, colourBG);
    return;
  }
  //Byte array for storing the digits
  uint8_t digits[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t i = 0;
  //Write the number (of num digits n) into the LAST n indexes of the digits array (everything preceding the number will be 0)
  while (toWrite) {
    digits[8 - i] = toWrite % 10;
    toWrite /= 10;
    i++;
  }
  int currentLine = 0;  //Current line
  int charPos = 0;      //Position of the character we are on along the line
  char charToWrite[1];
  //Do the normal writing routine but for every digit of the number, starting from the 0th index (writing preceding zeroes)
  for (i = 0; i < 9; i++) {  //Loop through every character of the string
    //If printing the next character would result in it being of screen
    if (pos.x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos > 240 - FONT_WIDTH) {
      currentLine++;
      charPos = 0;
    }
    sprintf(charToWrite, "%d", digits[i]);
    drawChar({pos.x + charPos * pixelsPerPixel * FONT_WIDTH + pixelsPerPixel * charPos, pos.y + currentLine * FONT_HEIGHT * pixelsPerPixel}, pixelsPerPixel, charToWrite[0], colourFG, colourBG);
    charPos++;
  }
}

/*
  Write a character to the screen position (x,y)
*/
void drawChar(coord pos, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG) {
  preWrite();

  //Width and height of the character on the display
  int characterDispWidth = FONT_WIDTH * pixelsPerPixel;
  int characterDispHeight = FONT_HEIGHT * pixelsPerPixel;
  setDisplayWriteRegion({pos.x, pos.y}, characterDispWidth, characterDispHeight);  //Set the window of display memory to write to
  //Depending on the font, an offset to the current character index might be needed to skip over the unprintable characters
  int offset = FONT_NEEDS_OFFSET ? 32 : 0;

  //Row goes between 0 and font height, column goes between 0 and the font width
  for (int row = 0; row < FONT_HEIGHT; row++) {
    for (int col = 0; col < FONT_WIDTH; col++) {
      //(font[character][col] >> row) & 1 will return true if the font dictates that (col, row) should have a pixel there
      //drawCharPixelToBuffer writes into the LCD buffer the correct colour data for the current character pixel
      drawCharPixelToBuffer({col, row}, pixelsPerPixel, (font[character - offset][col] >> row) & 1, colourFG, colourBG);
    }
  }
  sendSPICommand(0x2C);
  //Size 8 is probably the largest useful font, and at that size, a character takes up < 6000 bytes in the buffer, meaning we are nowhere near to filling up the buffer with a character
  writeSPI(lcdBuffer, characterDispWidth * characterDispHeight * 2);  //Write the character to the display

  postWrite();
}

/*
  Add pixel data into the LCD buffer for the character's current pixel
  (logic is explained in Writeup.md)
*/
void drawCharPixelToBuffer(coord charPos, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG) {
  int columnFontIndexScaledByPixelCount = charPos.x * pixelsPerPixel;
  int rowFontIndexScaledByPixelCount = charPos.y * pixelsPerPixel;
  int pixelsPerRow = FONT_WIDTH * pixelsPerPixel;

  if (colourFG == COLOUR_WHITE && colourBG == COLOUR_BLACK) {
    //If we are writing white on black (very common), hardcode the colours and don't use the ones from the parameter
    //This is slightly more efficient
    for (int i = 0; i < pixelsPerPixel; i++) {
      for (int j = 0; j < pixelsPerPixel; j++) {
        lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j))] = pixelInCharHere ? 0xFF : 0x00;
        lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j)) + 1] = pixelInCharHere ? 0xFF : 0x00;
      }
    }
  } else {
    for (int i = 0; i < pixelsPerPixel; i++) {
      for (int j = 0; j < pixelsPerPixel; j++) {
        lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j))] = pixelInCharHere ? (colourFG >> 8) & 0xFF : (colourBG >> 8) & 0xFF;
        lcdBuffer[2 * ((rowFontIndexScaledByPixelCount + i) * pixelsPerRow + (columnFontIndexScaledByPixelCount + j)) + 1] = pixelInCharHere ? colourFG & 0xFF : colourBG & 0xFF;
      }
    }
  }
}

/*
  Draw a rect with origin x,y and width w, height h
*/
void drawFilledRect(coord pos, uint32_t w, uint32_t h, uint16_t colour) {
  preWrite();
  setDisplayWriteRegion({pos.x, pos.y}, w, h);
  sendSPICommand(0x2C);  //Memory write
  uint32_t numberOfBytesToWriteToLCD;
  uint32_t numberBytesInWindowArea = (windowArea * 2);
  uint32_t lcdBufferSize = LCD_BUFFER_SIZE;  //Size of LCD buffer
  //If we are comfortable that the number of bytes the current window will hold in a buffer is less than the max buffer size:
  if (numberBytesInWindowArea < lcdBufferSize)
    lcdBufferSize = numberBytesInWindowArea;  //Set the buffer size to be that of the window area * 2 (number of bytes that the window would occupy)

  for (int i = 0; i <= lcdBufferSize; i++) {  //Loop through buffer
    //Write every pixel (half-word) into the LCD buffer
    lcdBuffer[i++] = (colour >> 8) & 0xFF;  //Post increment meaning that it first writes to position i, then increments i
    lcdBuffer[i] = colour & 0xFF;           //Writes to the (now) position of i++
  }
  do {
    if (numberBytesInWindowArea >= LCD_BUFFER_SIZE)
      numberOfBytesToWriteToLCD = LCD_BUFFER_SIZE;
    else
      numberOfBytesToWriteToLCD = numberBytesInWindowArea;
    writeSPI(lcdBuffer, numberOfBytesToWriteToLCD);
    numberBytesInWindowArea -= numberOfBytesToWriteToLCD;
  } while (numberBytesInWindowArea > 0);
  postWrite();
}

/*
  Set the column and row RAM addresses for writing to the display
  You must select a region in the LCD RAM to write pixel data to
  This region has an xStart, xEnd, yStart and yEnd address
  As you write half-words (pixels) over SPI, the RAM fills horizontally per row
*/
void setDisplayWriteRegion(coord pos, uint32_t w, uint32_t h) {
  uint8_t buf[4];  //Parameter buffer
  windowHeight = h;
  windowWidth = w;
  windowArea = w * h;    //Calculate window area
  sendSPICommand(0x2A);  //Column address set
  buf[0] = 0x00;         //Padding write value to make it 16 bit
  buf[1] = pos.x;
  buf[2] = 0x00;
  buf[3] = (pos.x + w - 1);
  writeSPI(buf, 4);
  sendSPICommand(0x2B);  //Row address set
  buf[0] = 0x00;
  buf[1] = pos.y;
  buf[2] = 0x00;
  buf[3] = ((pos.y + h - 1) & 0xFF);
  writeSPI(buf, 4);
}

/* 
  Draw a rectangle with outline of width lineWidth
 */
void drawUnfilledRect(coord pos, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour) {
  drawFilledRect({pos.x, pos.y}, w, h, COLOUR_BLACK);  //Clear rect of stuff
  //Top rect
  drawFilledRect({pos.x, pos.y}, w, lineWidth, colour);
  //Bottom rect
  drawFilledRect({pos.x, pos.y + h - lineWidth}, w, lineWidth, colour);
  //Left rect
  drawFilledRect({pos.x, pos.y}, lineWidth, h, colour);
  //Right rect
  drawFilledRect({pos.x + w - lineWidth, pos.y}, lineWidth, h, colour);
}

/* 
  Draw a rect outline with character in the middle (look at writeup for explanation)
 */
void drawUnfilledRectWithChar(coord pos, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize) {
  drawUnfilledRect({pos.x, pos.y}, w, h, lineWidth, rectColour);
  drawChar({(pos.x + (w / 2)) - (STR_WIDTH("-", fontSize) / 2),
            (pos.y + (h / 2)) - ((FONT_HEIGHT * fontSize) / 2)},
           fontSize, character, COLOUR_WHITE, COLOUR_BLACK);
}

/*
  Clear display (clear whole display when no arg (or false) is passed in)
*/
void clearDisplay(bool leaveAppDrawer) {
  drawFilledRect({0, 0}, 240, leaveAppDrawer ? 213 : 240, 0x0000);
}

void writeNewChar(coord pos, char toWrite) {
  preWrite();
  setDisplayWriteRegion({pos.x, pos.y}, 10, 16);  //Set the write region
  memset(lcdBuffer, 0x00, 10 * 16 * 2);           //Fully clear RAM region where we will be writing the character
  //The current coordinates inside the character BOUNDING BOX, not the overall character
  int bbX = 0;
  int bbY = 0;
  int byteNumber, byteOffset;  //The current character byte number, and the bit offset of that byte
                               // for (int row = glyph_dsc['~' - 32].ofs_x; row < glyph_dsc[toWrite - 32].ofs_x + glyph_dsc[toWrite - 32].box_w; row++) {
                               //   for (int col = glyph_dsc[toWrite - 32].ofs_y; col < glyph_dsc[toWrite - 32].ofs_y + glyph_dsc[toWrite - 32].box_h; col++) {
  for (int row = 0; row < 16; row++) {
    for (int col = 0; col < 10; col++) {
      //This will not work since the drawCharPixelToBuffer function uses the dimensions of the font in font.h (ie 8*5) rather than the 16*10 of the new font
      //TODO: Complete this function to allow writing lvgl fonts
      drawCharPixelToBuffer({pos.x + row, pos.y + col}, 1, 1 /* ((gylph_bitmap[glyph_dsc[toWrite - 32].bitmap_index + byteNumber] << byteOffset) & 0x80) >> 7 */, COLOUR_WHITE, COLOUR_BLACK);
      bbY++;
    }
    bbX++;
  }

  sendSPICommand(0x2C);
  writeSPI(lcdBuffer, 10 * 16 * 2);  //Write the character to the display

  postWrite();
}