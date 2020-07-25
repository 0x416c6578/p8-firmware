# Converting To LVGL Fonts
## Understanding How LVGL Fonts work
- LVGL uses a compressed format for storing fonts, meaning that only useful data is stored in memory
- This means if a character is mainly whitespace, for example a fullstop, that a lot of the pointless data of the empty space is removed
  - Currently, fonts in p8-firmware store 8*FONT_WIDTH bits per character, whereas LVGL fonts are variable
- A font is stored as a big bitmap array of bytes
```c
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+20 " " */

    /* U+21 "!" */
    0xf2,

    /* U+22 "\"" */
    0x99, 0x90,

    /* U+23 "#" */
    0x49, 0x2f, 0xd2, 0xfd, 0x24, 0x80, //... and so on ...
```

- There is also a corresponding array of font information structs following this format
  - The UNICODE value of a character can be put into this array as an index to get the corresponding character font information
  - From this struct, the offset of the character in the bitmap array can be found
```c
typedef struct {
    uint32_t bitmap_index;  <- Start index of the bitmap. A font can be max 4 GB. 
    uint32_t adv_w;         <- Draw the next glyph after this width. 28.4 format (real_value * 16 is stored)
    uint8_t box_w;  <- Width of the glyph's bounding box
    uint8_t box_h;  <- Height of the glyph's bounding box
    int8_t ofs_x;   <- x offset of the bounding box
    int8_t ofs_y;   <- y offset of the bounding box. Measured from the top of the line
} lv_font_fmt_txt_glyph_dsc_t;
```
- The array looks like 
```c
...
	{.bitmap_index = 0, .adv_w = 0, .box_h = 0, .box_w = 0, .ofs_x = 0, .ofs_y = 0}      /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_h = 0, .box_w = 0, .ofs_x = 0, .ofs_y = 0},   //Space
    {.bitmap_index = 0, .adv_w = 128, .box_h = 7, .box_w = 1, .ofs_x = 3, .ofs_y = -1},  //!
    {.bitmap_index = 1, .adv_w = 128, .box_h = 3, .box_w = 4, .ofs_x = 2, .ofs_y = 3},   //Backslash
    {.bitmap_index = 3, .adv_w = 128, .box_h = 7, .box_w = 6, .ofs_x = 1, .ofs_y = -1},  //#
...
```
- This font data is for a monospace font (hence the width to advance per char is 128/16 = 8 pixels (look at struct for info))
- For the exclamation mark, the character bounding box dimensions are 7 down * 1 accross
- As you can see, there is a tradeoff of having to store metadata (and the calculations that go with that), however it greatly reduces the font size in memory
## Reading Font Bitmap Data
- The only part of a character that is written to is the bounding box area, meaning that the data stored in the bitmap correlates to only that part of the character in the bounding box
- For '!', the data stored is 0xf2, or 0b1111001(0) (the thing in brackets is just padding)
- Data is read into the bounding box starting in the top-left and working along, then down
  - The bounding box is a 7 down * 1 accross box, meaning the letter looks like:
```
___
|@|
---
|@|
---
|@|
---
|@|
---
| |
---
| |
---
|@|
---
```
- Data is read into the bounding box starting in the top-left and working down before accross
- Another example is the pound (#), its data being 0x49, 0x2f, 0xd2, 0xfd, 0x24, 0x80, or 0b01001001|00101111|11010010|11111101|00100100|10(000000)
- This looks like:
```
_____________
| |@| | |@| |
-------------
| |@| | |@| |
-------------
|@|@|@|@|@|@|
-------------
| |@| | |@| |
-------------
|@|@|@|@|@|@|
-------------
| |@| | |@| |
-------------
| |@| | |@| |
-------------
```
- To calculate the number of bytes that should be read for a character, find the bounding box area with width*height, then find the next multiple of 8 greater than it, finally dividing that by 8 to find the number of bytes to read into the bounding box
  - So for example '#', the bounding box area is 7\*6=42, the next highest multiple of 8 is 48, which is 6\*8, meaning you should read indexes offset+0, offset+1,...,offset+5 of the bitmap array
  - This also means the last 6 bits of the last byte are unused

## LVGL Font Engine
- So now that we know how LVGL fonts work, we must figure out how to implement them into `p8-firmware`
