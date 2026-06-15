/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --bpp 1 --size 12 --no-compress --stride 1 --align 1 --font drafting-mono.regular.ttf --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_/\"'()[]{}<>+*&|#@;:%^ --format lvgl -o drafting_mono_12.c
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



#ifndef DRAFTING_MONO_12
#define DRAFTING_MONO_12 1
#endif

#if DRAFTING_MONO_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x24, 0x49, 0xf9, 0x42, 0x9f, 0xd2, 0x24, 0x40,

    /* U+0025 "%" */
    0x3, 0xea, 0xbc, 0x10, 0xf3, 0x55, 0x5c, 0x0,

    /* U+0026 "&" */
    0x72, 0x8, 0x1f, 0x92, 0x29, 0x1c,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x34, 0x88, 0x88, 0x84, 0x61,

    /* U+0029 ")" */
    0xc2, 0x21, 0x11, 0x12, 0x2c,

    /* U+002A "*" */
    0x25, 0x76, 0xa5, 0x0,

    /* U+002B "+" */
    0x27, 0xc8, 0x40,

    /* U+002D "-" */
    0xf0,

    /* U+002F "/" */
    0x0, 0x84, 0x22, 0x11, 0x8, 0x44, 0x0,

    /* U+0030 "0" */
    0x7b, 0x28, 0x69, 0x86, 0x1c, 0xde,

    /* U+0031 "1" */
    0x27, 0x8, 0x42, 0x10, 0x84, 0xf8,

    /* U+0032 "2" */
    0x39, 0x14, 0x42, 0x10, 0x84, 0x3f,

    /* U+0033 "3" */
    0x74, 0x42, 0x23, 0x84, 0x23, 0x36, 0x0,

    /* U+0034 "4" */
    0x8, 0x10, 0x40, 0x82, 0x4, 0x91, 0x3f, 0x4,
    0x8,

    /* U+0035 "5" */
    0x7d, 0x4, 0x2e, 0xc4, 0x10, 0x42, 0x33, 0x0,

    /* U+0036 "6" */
    0x10, 0xc6, 0x10, 0xbb, 0x38, 0x61, 0xcd, 0xe0,

    /* U+0037 "7" */
    0xfe, 0x28, 0x84, 0x10, 0x82, 0x10, 0x41, 0x0,

    /* U+0038 "8" */
    0x7a, 0x18, 0x5e, 0xce, 0x18, 0x5e,

    /* U+0039 "9" */
    0x7b, 0x38, 0x61, 0xcd, 0xd0, 0x86, 0x30, 0x80,

    /* U+003A ":" */
    0xf0, 0xf0,

    /* U+003B ";" */
    0xf0, 0xea,

    /* U+003C "<" */
    0x7, 0x8e, 0x10,

    /* U+003E ">" */
    0xe, 0x17, 0x80,

    /* U+0040 "@" */
    0x3c, 0xc7, 0xee, 0x5c, 0xbf, 0xb0, 0x3c,

    /* U+0041 "A" */
    0x78, 0x30, 0xa1, 0x42, 0x4f, 0x91, 0x21,

    /* U+0042 "B" */
    0xf9, 0x14, 0x5e, 0x45, 0x14, 0x7e,

    /* U+0043 "C" */
    0x75, 0x38, 0xe0, 0x82, 0xc, 0x9e,

    /* U+0044 "D" */
    0xf9, 0x24, 0x51, 0x45, 0x14, 0xbc,

    /* U+0045 "E" */
    0xfd, 0x14, 0x1c, 0x41, 0x14, 0x7f,

    /* U+0046 "F" */
    0xfd, 0x14, 0x1c, 0x41, 0x4, 0x3c,

    /* U+0047 "G" */
    0x75, 0x38, 0x60, 0x9e, 0x1c, 0xdd,

    /* U+0048 "H" */
    0xee, 0x89, 0x13, 0xe4, 0x48, 0x91, 0x77,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x9f,

    /* U+004A "J" */
    0x1c, 0x20, 0x82, 0x8, 0x28, 0x9c,

    /* U+004B "K" */
    0xee, 0x91, 0x43, 0x87, 0x9, 0x11, 0x77,

    /* U+004C "L" */
    0xe1, 0x4, 0x10, 0x41, 0x14, 0x7f,

    /* U+004D "M" */
    0xee, 0xd9, 0xb3, 0xa5, 0x4a, 0x91, 0x77,

    /* U+004E "N" */
    0xe5, 0x95, 0x55, 0x55, 0x34, 0xd1,

    /* U+004F "O" */
    0x79, 0x28, 0x61, 0x86, 0x1c, 0xde,

    /* U+0050 "P" */
    0xf9, 0x14, 0x51, 0x79, 0x4, 0x38,

    /* U+0051 "Q" */
    0x78, 0x92, 0x14, 0x28, 0x56, 0xb6, 0x3c, 0x14,
    0x18,

    /* U+0052 "R" */
    0xf8, 0x89, 0x13, 0xc4, 0x89, 0x12, 0xf2,

    /* U+0053 "S" */
    0x3a, 0x8d, 0x1, 0x80, 0xd8, 0x78, 0xee,

    /* U+0054 "T" */
    0xff, 0x26, 0x48, 0x81, 0x2, 0x4, 0x1c,

    /* U+0055 "U" */
    0xee, 0x89, 0x12, 0x24, 0x48, 0x91, 0x1c,

    /* U+0056 "V" */
    0xee, 0x89, 0x11, 0x42, 0x85, 0x6, 0x8,

    /* U+0057 "W" */
    0xee, 0x89, 0x52, 0xa7, 0x4d, 0x9b, 0x36,

    /* U+0058 "X" */
    0xee, 0x48, 0xa0, 0x81, 0x85, 0x11, 0x77,

    /* U+0059 "Y" */
    0xee, 0x88, 0xa1, 0x41, 0x2, 0x4, 0x1c,

    /* U+005A "Z" */
    0xfc, 0xa4, 0x42, 0x22, 0x3f,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0xf0,

    /* U+005C "\\" */
    0x2, 0x10, 0x42, 0x10, 0x42, 0x8, 0x40,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0xf0,

    /* U+005E "^" */
    0x22, 0xa3, 0x0,

    /* U+005F "_" */
    0xf8,

    /* U+0061 "a" */
    0x70, 0x23, 0xb2, 0x9b, 0xb0,

    /* U+0062 "b" */
    0xc0, 0x81, 0x73, 0x34, 0x28, 0x59, 0xbe,

    /* U+0063 "c" */
    0x76, 0x63, 0x8, 0xbc,

    /* U+0064 "d" */
    0xc, 0x17, 0x73, 0x86, 0x1c, 0xdd,

    /* U+0065 "e" */
    0x74, 0x7f, 0x8, 0xbc,

    /* U+0066 "f" */
    0x39, 0x4, 0x3e, 0x41, 0x4, 0x10, 0xf0,

    /* U+0067 "g" */
    0x77, 0x12, 0x23, 0x88, 0x1f, 0xa1, 0x7c,

    /* U+0068 "h" */
    0xc1, 0x5, 0x99, 0x45, 0x14, 0x51,

    /* U+0069 "i" */
    0x20, 0x38, 0x42, 0x10, 0x9f,

    /* U+006A "j" */
    0x8, 0xe, 0x10, 0x84, 0x21, 0x4b, 0x80,

    /* U+006B "k" */
    0xc0, 0x81, 0x12, 0x45, 0xf, 0x13, 0x23,

    /* U+006C "l" */
    0xe1, 0x8, 0x42, 0x10, 0x9f,

    /* U+006D "m" */
    0xfd, 0x55, 0x55, 0x55, 0x50,

    /* U+006E "n" */
    0xd9, 0x94, 0x51, 0x45, 0x10,

    /* U+006F "o" */
    0x7b, 0x38, 0x61, 0xcd, 0xe0,

    /* U+0070 "p" */
    0xf9, 0x14, 0x51, 0x4d, 0xe4, 0x38,

    /* U+0071 "q" */
    0x7d, 0x9a, 0x14, 0x2c, 0xce, 0x81, 0x7,

    /* U+0072 "r" */
    0xdd, 0x94, 0x50, 0x43, 0xc0,

    /* U+0073 "s" */
    0x74, 0x30, 0x78, 0xf8,

    /* U+0074 "t" */
    0x42, 0x3e, 0x84, 0x21, 0x7,

    /* U+0075 "u" */
    0xcd, 0x14, 0x51, 0x4c, 0xd0,

    /* U+0076 "v" */
    0x46, 0x88, 0x91, 0x42, 0x82, 0x0,

    /* U+0077 "w" */
    0x82, 0xa5, 0x53, 0x66, 0xcc, 0x80,

    /* U+0078 "x" */
    0x89, 0x43, 0xc, 0x5a, 0x20,

    /* U+0079 "y" */
    0x46, 0x88, 0x91, 0x41, 0x82, 0x24, 0x70,

    /* U+007A "z" */
    0xfa, 0x42, 0x19, 0x47, 0xf0,

    /* U+007B "{" */
    0x39, 0x8, 0x4c, 0x10, 0x84, 0x38,

    /* U+007C "|" */
    0xff, 0xc0,

    /* U+007D "}" */
    0xe0, 0x8c, 0x20, 0x88, 0x42, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 115, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 2, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 10, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 18, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 24, .adv_w = 115, .box_w = 1, .box_h = 4, .ofs_x = 3, .ofs_y = 4},
    {.bitmap_index = 25, .adv_w = 115, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 30, .adv_w = 115, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 35, .adv_w = 115, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 39, .adv_w = 115, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 42, .adv_w = 115, .box_w = 4, .box_h = 1, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 43, .adv_w = 115, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 50, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 115, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 75, .adv_w = 115, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 84, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 92, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 108, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 115, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 122, .adv_w = 115, .box_w = 2, .box_h = 6, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 124, .adv_w = 115, .box_w = 2, .box_h = 8, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 126, .adv_w = 115, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 129, .adv_w = 115, .box_w = 4, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 132, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 115, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 232, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 238, .adv_w = 115, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 247, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 282, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 115, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 115, .box_w = 4, .box_h = 9, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 313, .adv_w = 115, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 320, .adv_w = 115, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 325, .adv_w = 115, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 328, .adv_w = 115, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 329, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 369, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 375, .adv_w = 115, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 380, .adv_w = 115, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 387, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 115, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 409, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 420, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 427, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 432, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 115, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 115, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 115, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 458, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 470, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 475, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 481, .adv_w = 115, .box_w = 1, .box_h = 10, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 483, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 0, 2, 3, 4, 5, 6,
    7, 8, 0, 9, 0, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 0, 24
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 34, .range_length = 29, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 29, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 64, .range_length = 32, .glyph_id_start = 26,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 29, .glyph_id_start = 58,
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
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};

extern const lv_font_t lv_font_montserrat_12;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t drafting_mono_12 = {
#else
lv_font_t drafting_mono_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_12,
#endif
    .user_data = NULL,
};



#endif /*#if DRAFTING_MONO_12*/
