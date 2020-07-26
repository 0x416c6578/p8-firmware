#define LV_ATTRIBUTE_LARGE_CONST
//Typedefs for embedded int types
/* typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef signed char int8_t; */

typedef struct {
  uint16_t bitmap_index; /**< Start index of the bitmap. A font can be max 4 GB. */
  uint16_t adv_w;        /**< Draw the next glyph after this width. 28.4 format (real_value * 16 is stored). */
  uint8_t box_w;         /**< Width of the glyph's bounding box*/
  uint8_t box_h;         /**< Height of the glyph's bounding box*/
  int8_t ofs_x;          /**< x offset of the bounding box*/
  int8_t ofs_y;          /**< y offset of the bounding box. Measured from the top of the line*/
} lv_font_fmt_txt_glyph_dsc_t;

static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+20 " " */
    0x0,

    /* U+21 "!" */
    0xff, 0xff, 0x3,

    /* U+22 "\"" */
    0xde, 0xf7, 0xbd, 0x80,

    /* U+23 "#" */
    0x13, 0x9, 0x84, 0x8f, 0xf3, 0x21, 0x10, 0x88,
    0x4c, 0xff, 0x12, 0x19, 0xc, 0x80,

    /* U+24 "$" */
    0x18, 0x18, 0x7e, 0xc7, 0xc3, 0xc0, 0x70, 0x3e,
    0xf, 0x3, 0xc3, 0xe3, 0x7e, 0x18, 0x18,

    /* U+25 "%" */
    0x60, 0x48, 0x24, 0x12, 0x19, 0x3b, 0x30, 0x60,
    0x6e, 0xc8, 0x84, 0x42, 0x20, 0xe0,

    /* U+26 "&" */
    0x3c, 0x66, 0x66, 0x60, 0x30, 0x70, 0xdb, 0xcb,
    0xce, 0xc6, 0xce, 0x7b,

    /* U+27 "'" */
    0xff, 0xc0,

    /* U+28 "(" */
    0x0, 0xdc, 0xcc, 0x63, 0x18, 0xc6, 0x31, 0x86,
    0x38, 0xe1,

    /* U+29 ")" */
    0x6, 0x18, 0x61, 0x8c, 0x63, 0x18, 0xc6, 0x33,
    0x3b, 0x90,

    /* U+2A "*" */
    0x18, 0x18, 0xdb, 0xff, 0x18, 0x3c, 0x66, 0x42,

    /* U+2B "+" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18,

    /* U+2C "," */
    0x7f, 0x68,

    /* U+2D "-" */
    0xfc,

    /* U+2E "." */
    0xff, 0x80,

    /* U+2F "/" */
    0x2, 0x6, 0x6, 0xc, 0xc, 0x8, 0x18, 0x18,
    0x10, 0x30, 0x30, 0x60, 0x60, 0x40, 0xc0,

    /* U+30 "0" */
    0x39, 0xdb, 0x1e, 0x3c, 0x7b, 0xf7, 0xef, 0xc7,
    0x8d, 0xb1, 0xc0,

    /* U+31 "1" */
    0x38, 0xf3, 0x64, 0xc1, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x67, 0xf0,

    /* U+32 "2" */
    0x79, 0xcb, 0x18, 0x30, 0x61, 0xc3, 0xc, 0x30,
    0xc3, 0x7, 0xf0,

    /* U+33 "3" */
    0xfe, 0x1c, 0x70, 0xc3, 0x7, 0x3, 0x3, 0x7,
    0xf, 0x33, 0xc0,

    /* U+34 "4" */
    0xc, 0x38, 0x61, 0x83, 0xc, 0x39, 0xe3, 0xfe,
    0xc, 0x18, 0x30,

    /* U+35 "5" */
    0xfd, 0x83, 0x6, 0xc, 0x1f, 0xb3, 0x83, 0x6,
    0xf, 0x3b, 0xc0,

    /* U+36 "6" */
    0xc, 0x18, 0x38, 0x30, 0x60, 0x7c, 0xe6, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+37 "7" */
    0xff, 0xc2, 0xc6, 0x6, 0x4, 0xc, 0xc, 0x18,
    0x18, 0x18, 0x30, 0x30,

    /* U+38 "8" */
    0x3c, 0xe7, 0xc3, 0xc3, 0x66, 0x18, 0x66, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+39 "9" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0x67, 0x3e, 0x6,
    0xc, 0x1c, 0x18, 0x30,

    /* U+3A ":" */
    0xff, 0x80, 0x3f, 0xe0,

    /* U+3B ";" */
    0x77, 0x70, 0x0, 0x7, 0x66, 0x4c,

    /* U+3C "<" */
    0x0, 0xc, 0x73, 0x8c, 0x1c, 0xe, 0x7, 0x2,

    /* U+3D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+3E ">" */
    0x1, 0x81, 0xc0, 0xe0, 0x61, 0xce, 0x70, 0x80,

    /* U+3F "?" */
    0xf0, 0x60, 0xc3, 0xc, 0x77, 0x98, 0x60, 0x0,
    0x18,

    /* U+40 "@" */
    0x3e, 0x31, 0xb0, 0x78, 0x3c, 0xde, 0xdf, 0x67,
    0xb3, 0xd9, 0xec, 0xf3, 0xf8, 0xc, 0x3, 0x0,
    0xf0,

    /* U+41 "A" */
    0x38, 0x38, 0x3c, 0x2c, 0x6c, 0x64, 0x66, 0x66,
    0xfe, 0xc2, 0xc3, 0x83,

    /* U+42 "B" */
    0xfd, 0x9f, 0x1e, 0x3c, 0x79, 0xbe, 0x63, 0xc7,
    0x8f, 0x1f, 0xe0,

    /* U+43 "C" */
    0x3c, 0x66, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc3, 0x66, 0x3c,

    /* U+44 "D" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x37, 0xc0,

    /* U+45 "E" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0xe0, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+46 "F" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0xe0, 0xc1,
    0x83, 0x6, 0x0,

    /* U+47 "G" */
    0x3c, 0xcf, 0x1e, 0xc, 0x18, 0x37, 0xe3, 0xc7,
    0x8d, 0x39, 0xc0,

    /* U+48 "H" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7f, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+49 "I" */
    0xfe, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc7, 0xf0,

    /* U+4A "J" */
    0x7f, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
    0x3, 0xc3, 0x46, 0x3c,

    /* U+4B "K" */
    0xc3, 0xc6, 0xc6, 0xcc, 0xcc, 0xf8, 0xcc, 0xcc,
    0xc6, 0xc6, 0xc2, 0xc3,

    /* U+4C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+4D "M" */
    0xe7, 0xe7, 0xe7, 0xf7, 0xdb, 0xdb, 0xdb, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3,

    /* U+4E "N" */
    0xe7, 0xcf, 0x9f, 0x3d, 0x7a, 0xf5, 0xeb, 0xcf,
    0x9f, 0x3e, 0x70,

    /* U+4F "O" */
    0x39, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xb1, 0xc0,

    /* U+50 "P" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+51 "Q" */
    0x39, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xb1, 0xc0, 0x81, 0x81, 0x80,

    /* U+52 "R" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x79, 0xbe, 0x6c, 0xc9,
    0x9b, 0x36, 0x30,

    /* U+53 "S" */
    0x3c, 0x47, 0xc3, 0xc0, 0xf0, 0x7c, 0x3e, 0x7,
    0x3, 0xc3, 0xe6, 0x3c,

    /* U+54 "T" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18,

    /* U+55 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1b, 0xe0,

    /* U+56 "V" */
    0x83, 0xc3, 0xc2, 0xc6, 0x46, 0x66, 0x64, 0x6c,
    0x2c, 0x3c, 0x38, 0x38,

    /* U+57 "W" */
    0x99, 0xcc, 0xe6, 0x5a, 0xad, 0x56, 0xab, 0x54,
    0xea, 0x65, 0x33, 0x99, 0xcc, 0xe0,

    /* U+58 "X" */
    0xc3, 0x46, 0x66, 0x2c, 0x38, 0x18, 0x38, 0x3c,
    0x6c, 0x66, 0xc6, 0xc3,

    /* U+59 "Y" */
    0x61, 0x98, 0x62, 0x10, 0xcc, 0x13, 0x7, 0x81,
    0xe0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+5A "Z" */
    0xfe, 0xc, 0x30, 0x61, 0x83, 0xc, 0x18, 0x60,
    0xc3, 0x7, 0xf0,

    /* U+5B "[" */
    0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xf0,

    /* U+5C "\\" */
    0xc0, 0x40, 0x60, 0x60, 0x30, 0x30, 0x30, 0x18,
    0x18, 0x8, 0xc, 0xc, 0x6, 0x6, 0x6,

    /* U+5D "]" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0x8f, 0xe0,

    /* U+5E "^" */
    0x18, 0x30, 0xf1, 0x66, 0x48, 0xf0, 0x80,

    /* U+5F "_" */
    0xff,

    /* U+60 "`" */
    0xd9, 0x80,

    /* U+61 "a" */
    0x3e, 0x67, 0x3, 0x3, 0x7f, 0xc3, 0xc3, 0xc7,
    0x7b,

    /* U+62 "b" */
    0xc1, 0x83, 0x6, 0xee, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x3f, 0xe0,

    /* U+63 "c" */
    0x3c, 0x66, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x66,
    0x3c,

    /* U+64 "d" */
    0x6, 0xc, 0x1b, 0xbe, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8f, 0xbb, 0xf0,

    /* U+65 "e" */
    0x7d, 0xdf, 0x1e, 0x3f, 0xf8, 0x30, 0x63, 0x7c,

    /* U+66 "f" */
    0x1f, 0x30, 0x30, 0x30, 0xff, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,

    /* U+67 "g" */
    0x77, 0xdf, 0x1e, 0x3c, 0x78, 0xf3, 0xbb, 0x6,
    0xc, 0x19, 0xe0,

    /* U+68 "h" */
    0xc1, 0x83, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+69 "i" */
    0x0, 0x30, 0x0, 0xf, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x60, 0xcf, 0xe0,

    /* U+6A "j" */
    0x0, 0x30, 0x0, 0x7c, 0x30, 0xc3, 0xc, 0x30,
    0xc3, 0xc, 0x31, 0xbc,

    /* U+6B "k" */
    0xc0, 0xc0, 0xc0, 0xc3, 0xc6, 0xcc, 0xcc, 0xf8,
    0xcc, 0xcc, 0xc6, 0xc3,

    /* U+6C "l" */
    0xf8, 0xc, 0x6, 0x3, 0x1, 0x80, 0xc0, 0x60,
    0x30, 0x18, 0xc, 0x6, 0x1, 0xf0,

    /* U+6D "m" */
    0xfe, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb,

    /* U+6E "n" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,

    /* U+6F "o" */
    0x79, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xe7, 0x38,

    /* U+70 "p" */
    0xdd, 0xdf, 0x1e, 0x3c, 0x78, 0xf1, 0xe7, 0xfd,
    0x83, 0x6, 0x0,

    /* U+71 "q" */
    0x77, 0xdf, 0x1e, 0x3c, 0x78, 0xf1, 0xf7, 0x76,
    0xc, 0x18, 0x30,

    /* U+72 "r" */
    0xdd, 0xdf, 0x1e, 0x3c, 0x18, 0x30, 0x60, 0xc0,

    /* U+73 "s" */
    0x7d, 0x8f, 0x7, 0xc7, 0xc3, 0xc1, 0xe3, 0x7c,

    /* U+74 "t" */
    0x30, 0x30, 0x30, 0xff, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x1f,

    /* U+75 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0x7c,

    /* U+76 "v" */
    0xc3, 0xc2, 0x46, 0x66, 0x64, 0x2c, 0x3c, 0x38,
    0x18,

    /* U+77 "w" */
    0x99, 0x99, 0x99, 0x99, 0xdb, 0xff, 0x66, 0x66,
    0x66,

    /* U+78 "x" */
    0xc6, 0x66, 0x2c, 0x38, 0x18, 0x3c, 0x6c, 0x66,
    0xc3,

    /* U+79 "y" */
    0xc3, 0xc2, 0x66, 0x66, 0x64, 0x3c, 0x3c, 0x18,
    0x18, 0x18, 0x30, 0x30,

    /* U+7A "z" */
    0xfe, 0xc, 0x30, 0xc3, 0x86, 0x18, 0x60, 0xfe,

    /* U+7B "{" */
    0xe, 0x30, 0x60, 0xc1, 0x83, 0x6, 0x70, 0x18,
    0x30, 0x60, 0xc1, 0x83, 0x3, 0x80,

    /* U+7C "|" */
    0xff, 0xff, 0xff, 0xfc,

    /* U+7D "}" */
    0xe0, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x7, 0x30,
    0x60, 0xc1, 0x83, 0x6, 0x38, 0x0,

    /* U+7E "~" */
    0x73, 0xd3, 0xce
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 154, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 154, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 154, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 8, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 37, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 51, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 154, .box_w = 2, .box_h = 5, .ofs_x = 4, .ofs_y = 7},
    {.bitmap_index = 65, .adv_w = 154, .box_w = 5, .box_h = 16, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 75, .adv_w = 154, .box_w = 5, .box_h = 16, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 85, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 93, .adv_w = 154, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 100, .adv_w = 154, .box_w = 3, .box_h = 5, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 102, .adv_w = 154, .box_w = 6, .box_h = 1, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 103, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 120, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 154, .box_w = 3, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 154, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 244, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 252, .adv_w = 154, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 257, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 265, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 154, .box_w = 9, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 291, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 359, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 370, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 381, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 416, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 439, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 450, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 487, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 545, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 571, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 586, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 154, .box_w = 4, .box_h = 15, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 605, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 620, .adv_w = 154, .box_w = 5, .box_h = 15, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 630, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 637, .adv_w = 154, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 638, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 10},
    {.bitmap_index = 640, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 649, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 660, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 669, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 680, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 688, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 711, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 722, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 734, .adv_w = 154, .box_w = 6, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 746, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 758, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 772, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 781, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 789, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 797, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 808, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 819, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 827, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 835, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 847, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 855, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 864, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 873, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 882, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 894, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 902, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 916, .adv_w = 154, .box_w = 2, .box_h = 15, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 920, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 934, .adv_w = 154, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 4}
};
