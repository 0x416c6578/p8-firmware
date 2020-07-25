#include <stdio.h>

#include "font.h"

/* 
  Displaying a font with this viewer:
    This font viewer uses LVGL format fonts
    To use this viewer, copy the arrays glyph_dsc and gylph_bitmap in their entireties
    to the font.h file, leaving the stuff at the top in place
  You can then call displayChar(char) to print a character to stdout
  Compile with - gcc lvglFontPreview.c -o out -Wall
 */

void displayChar(char toWrite) {
  int index = toWrite - 32;                                 //Get the offset relative to 0x20 (' ')
  index = index + 1;                                        //Add 1 since id=0 is reserved
  lv_font_fmt_txt_glyph_dsc_t charInfo = glyph_dsc[index];  //Get character info
  //Start writing the character from the bounding box origin rather than the character origin
  int byteNumber;  //Current byte NUMBER of font
  int byteOffset;  //Current bit of the current byte
  for (int y = 0; y < charInfo.box_h; y++) {
    for (int x = 0; x < charInfo.box_w; x++) {
      byteNumber = ((y * charInfo.box_w) + x) / 8;
      byteOffset = ((y * charInfo.box_w) + x) % 8;
      printf("%c ", ((gylph_bitmap[charInfo.bitmap_index + byteNumber] << byteOffset) & 0x80) >> 7 ? '@' : '.');  //'@' if pixel there, else '.'
    }
    printf("\n");  //Move to next line
  }
}

int main(int argc, char* argv[]) {
  //Print all characters:
  for (int i = 32; i < 127; i++) {
    printf("\n");
    displayChar(i);
  }
}