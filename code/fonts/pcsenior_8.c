/*******************************************************************************
 * Size: 8 px
 * Bpp: 1
 * Opts: --bpp 1 --size 8 --no-compress --stride 1 --align 1 --font pcsenior.ttf --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~[{]}:;"'\|/.,<>^&*$#@()!? _ --format lvgl -o pcsenior_8.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef PCSENIOR_8
#define PCSENIOR_8 1
#endif

#if PCSENIOR_8

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0x6f, 0xf6, 0x60, 0x60,

    /* U+0022 "\"" */
    0xde, 0xf6,

    /* U+0023 "#" */
    0x6c, 0xdb, 0xfb, 0x6f, 0xed, 0x9b, 0x0,

    /* U+0024 "$" */
    0x31, 0xfc, 0x1e, 0xf, 0xe3, 0x0,

    /* U+0026 "&" */
    0x38, 0xd8, 0xe3, 0xbd, 0xd9, 0x9d, 0x80,

    /* U+0027 "'" */
    0x6f, 0x0,

    /* U+0028 "(" */
    0x36, 0xcc, 0xc6, 0x30,

    /* U+0029 ")" */
    0xc6, 0x33, 0x36, 0xc0,

    /* U+002A "*" */
    0x66, 0x3c, 0xff, 0x3c, 0x66,

    /* U+002C "," */
    0x6f, 0x0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x6, 0x18, 0x61, 0x86, 0x18, 0x20, 0x0,

    /* U+0030 "0" */
    0x7d, 0x8f, 0x3e, 0xff, 0x7c, 0xdf, 0x0,

    /* U+0031 "1" */
    0x31, 0xc3, 0xc, 0x30, 0xcf, 0xc0,

    /* U+0032 "2" */
    0x7b, 0x30, 0xce, 0x63, 0x3f, 0xc0,

    /* U+0033 "3" */
    0x7b, 0x30, 0xce, 0xf, 0x37, 0x80,

    /* U+0034 "4" */
    0x1c, 0x79, 0xb6, 0x6f, 0xe1, 0x87, 0x80,

    /* U+0035 "5" */
    0xff, 0xf, 0x83, 0xf, 0x37, 0x80,

    /* U+0036 "6" */
    0x39, 0x8c, 0x3e, 0xcf, 0x37, 0x80,

    /* U+0037 "7" */
    0xff, 0x30, 0xc6, 0x30, 0xc3, 0x0,

    /* U+0038 "8" */
    0x7b, 0x3c, 0xde, 0xcf, 0x37, 0x80,

    /* U+0039 "9" */
    0x7b, 0x3c, 0xdf, 0xc, 0x67, 0x0,

    /* U+003A ":" */
    0xf0, 0xf0,

    /* U+003B ";" */
    0x6c, 0x6, 0xf0,

    /* U+003C "<" */
    0x19, 0x99, 0x86, 0x18, 0x60,

    /* U+003E ">" */
    0xc3, 0xc, 0x33, 0x33, 0x0,

    /* U+003F "?" */
    0x7b, 0x30, 0xc6, 0x30, 0x3, 0x0,

    /* U+0040 "@" */
    0x7d, 0x8f, 0x7e, 0xfd, 0xf8, 0x1e, 0x0,

    /* U+0041 "A" */
    0x31, 0xec, 0xf3, 0xff, 0x3c, 0xc0,

    /* U+0042 "B" */
    0xfc, 0xcd, 0x9b, 0xe6, 0x6c, 0xff, 0x0,

    /* U+0043 "C" */
    0x3c, 0xcf, 0x6, 0xc, 0xc, 0xcf, 0x0,

    /* U+0044 "D" */
    0xf8, 0xd9, 0x9b, 0x36, 0x6d, 0xbe, 0x0,

    /* U+0045 "E" */
    0xfe, 0xc5, 0xa3, 0xc6, 0x8c, 0x7f, 0x80,

    /* U+0046 "F" */
    0xfe, 0xc5, 0xa3, 0xc6, 0x8c, 0x3c, 0x0,

    /* U+0047 "G" */
    0x3c, 0xcf, 0x6, 0xc, 0xec, 0xcf, 0x80,

    /* U+0048 "H" */
    0xcf, 0x3c, 0xff, 0xcf, 0x3c, 0xc0,

    /* U+0049 "I" */
    0xf6, 0x66, 0x66, 0xf0,

    /* U+004A "J" */
    0x1e, 0x18, 0x30, 0x6c, 0xd9, 0x9e, 0x0,

    /* U+004B "K" */
    0xe6, 0xcd, 0xb3, 0xc6, 0xcc, 0xf9, 0x80,

    /* U+004C "L" */
    0xf0, 0xc1, 0x83, 0x6, 0x2c, 0xff, 0x80,

    /* U+004D "M" */
    0xc7, 0xdf, 0xff, 0xfd, 0x78, 0xf1, 0x80,

    /* U+004E "N" */
    0xc7, 0xcf, 0xde, 0xfc, 0xf8, 0xf1, 0x80,

    /* U+004F "O" */
    0x38, 0xdb, 0x1e, 0x3c, 0x6d, 0x8e, 0x0,

    /* U+0050 "P" */
    0xfc, 0xcd, 0x9b, 0xe6, 0xc, 0x3c, 0x0,

    /* U+0051 "Q" */
    0x7b, 0x3c, 0xf3, 0xdd, 0xe1, 0xc0,

    /* U+0052 "R" */
    0xfc, 0xcd, 0x9b, 0xe6, 0xcc, 0xf9, 0x80,

    /* U+0053 "S" */
    0x7b, 0x3e, 0x1c, 0x1f, 0x37, 0x80,

    /* U+0054 "T" */
    0xfe, 0xd3, 0xc, 0x30, 0xc7, 0x80,

    /* U+0055 "U" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3f, 0xc0,

    /* U+0056 "V" */
    0xcf, 0x3c, 0xf3, 0xcd, 0xe3, 0x0,

    /* U+0057 "W" */
    0xc7, 0x8f, 0x1e, 0xbf, 0xfd, 0xf1, 0x80,

    /* U+0058 "X" */
    0xc7, 0x8d, 0xb1, 0xc3, 0x8d, 0xb1, 0x80,

    /* U+0059 "Y" */
    0xcf, 0x3c, 0xde, 0x30, 0xc7, 0x80,

    /* U+005A "Z" */
    0xff, 0x8e, 0x30, 0xc3, 0x2c, 0xff, 0x80,

    /* U+005B "[" */
    0xfc, 0xcc, 0xcc, 0xf0,

    /* U+005C "\\" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0xf0,

    /* U+005E "^" */
    0x10, 0x71, 0xb6, 0x30,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xd9, 0x80,

    /* U+0061 "a" */
    0x78, 0x19, 0xf6, 0x67, 0x60,

    /* U+0062 "b" */
    0xe0, 0xc1, 0x83, 0xe6, 0x6c, 0xf7, 0x0,

    /* U+0063 "c" */
    0x7b, 0x3c, 0x33, 0x78,

    /* U+0064 "d" */
    0x1c, 0x18, 0x33, 0xec, 0xd9, 0x9d, 0x80,

    /* U+0065 "e" */
    0x7b, 0x3f, 0xf0, 0x78,

    /* U+0066 "f" */
    0x39, 0xb6, 0x3c, 0x61, 0x8f, 0x0,

    /* U+0067 "g" */
    0x77, 0x9b, 0x33, 0xe0, 0xdf, 0x0,

    /* U+0068 "h" */
    0xe0, 0xc1, 0xb3, 0xb6, 0x6c, 0xf9, 0x80,

    /* U+0069 "i" */
    0x60, 0xe6, 0x66, 0xf0,

    /* U+006A "j" */
    0xc, 0x0, 0xc3, 0xf, 0x3c, 0xde,

    /* U+006B "k" */
    0xe0, 0xc1, 0x9b, 0x67, 0x8d, 0xb9, 0x80,

    /* U+006C "l" */
    0xe6, 0x66, 0x66, 0xf0,

    /* U+006D "m" */
    0xcd, 0xff, 0xfe, 0xbc, 0x60,

    /* U+006E "n" */
    0xfb, 0x3c, 0xf3, 0xcc,

    /* U+006F "o" */
    0x7b, 0x3c, 0xf3, 0x78,

    /* U+0070 "p" */
    0xdc, 0xcd, 0x9b, 0xe6, 0x1e, 0x0,

    /* U+0071 "q" */
    0x77, 0x9b, 0x33, 0xe0, 0xc3, 0xc0,

    /* U+0072 "r" */
    0xdc, 0xed, 0x9b, 0xf, 0x0,

    /* U+0073 "s" */
    0x7f, 0x7, 0x83, 0xf8,

    /* U+0074 "t" */
    0x23, 0x3e, 0xc6, 0x34, 0xc0,

    /* U+0075 "u" */
    0xcd, 0x9b, 0x36, 0x67, 0x60,

    /* U+0076 "v" */
    0xcf, 0x3c, 0xde, 0x30,

    /* U+0077 "w" */
    0xc7, 0xaf, 0xff, 0xf6, 0xc0,

    /* U+0078 "x" */
    0xc6, 0xd8, 0xe3, 0x6c, 0x60,

    /* U+0079 "y" */
    0xcf, 0x3c, 0xdf, 0xf, 0xe0,

    /* U+007A "z" */
    0xfe, 0x63, 0x19, 0xfc,

    /* U+007B "{" */
    0x1c, 0xc3, 0x38, 0x30, 0xc1, 0xc0,

    /* U+007C "|" */
    0xfc, 0xfc,

    /* U+007D "}" */
    0xe0, 0xc3, 0x7, 0x30, 0xce, 0x0,

    /* U+007E "~" */
    0x77, 0xb8
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 128, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 5, .adv_w = 128, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 7, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 14, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 20, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 27, .adv_w = 128, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 29, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 33, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 37, .adv_w = 128, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 128, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 44, .adv_w = 128, .box_w = 2, .box_h = 2, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 45, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 52, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 59, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 65, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 71, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 77, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 84, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 90, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 96, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 102, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 108, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 114, .adv_w = 128, .box_w = 2, .box_h = 6, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 116, .adv_w = 128, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 119, .adv_w = 128, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 124, .adv_w = 128, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 129, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 135, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 142, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 148, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 155, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 162, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 169, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 176, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 183, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 190, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 196, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 200, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 207, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 214, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 221, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 228, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 235, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 242, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 249, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 255, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 262, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 268, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 274, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 280, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 286, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 293, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 300, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 306, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 313, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 317, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 324, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 328, .adv_w = 128, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 332, .adv_w = 128, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 333, .adv_w = 128, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 335, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 340, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 347, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 351, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 358, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 362, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 368, .adv_w = 128, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 374, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 381, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 385, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 391, .adv_w = 128, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 398, .adv_w = 128, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 402, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 407, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 411, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 415, .adv_w = 128, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 421, .adv_w = 128, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 427, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 432, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 436, .adv_w = 128, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 441, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 446, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 450, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 455, .adv_w = 128, .box_w = 7, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 460, .adv_w = 128, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 465, .adv_w = 128, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 469, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 475, .adv_w = 128, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 477, .adv_w = 128, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 483, .adv_w = 128, .box_w = 7, .box_h = 2, .ofs_x = 0, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 2, 3, 4, 0, 5, 6,
    7, 8, 9, 0, 10, 0, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 29, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 29, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 62, .range_length = 65, .glyph_id_start = 27,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t pcsenior_8 = {
#else
lv_font_t pcsenior_8 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 8,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if PCSENIOR_8*/
