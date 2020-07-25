#include <stdio.h>

#include "font.h"

//gcc lvglFontPreview.c -o out -Wall; ./out

typedef struct {
  uint8_t x, y;
} point;

void displayChar(char toWrite) {
  int index = toWrite - 32;                                 //Get the offset relative to 0x20
  index = index + 1;                                        //Add 1 since id=0 is reserved
  lv_font_fmt_txt_glyph_dsc_t charInfo = glyph_dsc[index];  //Get character info
  //Start writing the character from the bounding box origin rather than the character origin
  int byteNumber;
  int byteOffset;
  for (int y = 0; y < charInfo.box_h; y++) {
    for (int x = 0; x < charInfo.box_w; x++) {
      byteNumber = (int)((y * charInfo.box_w) + x) / 8;
      byteOffset = ((y * charInfo.box_w) + x) % 8;
      // printf("%d %d %d\n", byteNumber, byteOffset, ((font[charInfo.bitmap_index + byteNumber] << byteOffset) & 0x80) >> 7);
      printf("%c ", ((font[charInfo.bitmap_index + byteNumber] << byteOffset) & 0x80) >> 7 ? '@' : '.');
    }
    printf("\n");
  }
}

int main(int argc, char* argv[]) {
  for (int i = 32; i < 127; i++) {
    printf("\n");
    displayChar(i);
  }
}