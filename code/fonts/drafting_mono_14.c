/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --stride 1 --align 1 --font drafting-mono.regular.ttf --symbols ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_/\"'()[]{}<>+*&|#@;:%^ --format lvgl -o drafting_mono_14.c
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



#ifndef DRAFTING_MONO_14
#define DRAFTING_MONO_14 1
#endif

#if DRAFTING_MONO_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x12, 0x12, 0x12, 0x7f, 0x24, 0x24, 0xff, 0x24,
    0x48, 0x48,

    /* U+0025 "%" */
    0x4, 0xfa, 0x95, 0x4a, 0x8a, 0x7, 0x15, 0x2a,
    0x95, 0x18, 0x0,

    /* U+0026 "&" */
    0x38, 0x81, 0x3, 0x3, 0xe8, 0xa1, 0x42, 0xc4,
    0xf0,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x2, 0x44, 0x88, 0x88, 0x84, 0x21,

    /* U+0029 ")" */
    0xc, 0x22, 0x11, 0x11, 0x12, 0x48,

    /* U+002A "*" */
    0x10, 0x5e, 0xcc, 0x68, 0x0,

    /* U+002B "+" */
    0x21, 0x3e, 0x42, 0x0,

    /* U+002D "-" */
    0xf8,

    /* U+002F "/" */
    0xc, 0x20, 0x84, 0x10, 0xc2, 0x8, 0x41, 0xc,
    0x0,

    /* U+0030 "0" */
    0x38, 0x8a, 0xc, 0x19, 0x32, 0x60, 0xc1, 0x44,
    0x70,

    /* U+0031 "1" */
    0x11, 0xc9, 0x4, 0x10, 0x41, 0x4, 0x13, 0xf0,

    /* U+0032 "2" */
    0x39, 0x18, 0x41, 0x8, 0xc6, 0x30, 0x83, 0xf0,

    /* U+0033 "3" */
    0x39, 0x88, 0x10, 0x43, 0xc0, 0x40, 0x81, 0x4,
    0x33, 0x80,

    /* U+0034 "4" */
    0x8, 0x30, 0x40, 0x83, 0x4, 0x19, 0x22, 0x85,
    0xfc, 0x10, 0x20,

    /* U+0035 "5" */
    0x7e, 0x81, 0x2, 0xe6, 0x20, 0x40, 0x82, 0xc,
    0x61, 0x0,

    /* U+0036 "6" */
    0x0, 0x30, 0xc3, 0x4, 0x17, 0xb1, 0xc1, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0037 "7" */
    0xff, 0xe, 0x14, 0x40, 0x82, 0x4, 0x10, 0x20,
    0x41, 0x0, 0x0,

    /* U+0038 "8" */
    0x38, 0x89, 0x12, 0x23, 0x88, 0xa0, 0xc1, 0xc6,
    0xf8,

    /* U+0039 "9" */
    0x38, 0x8a, 0xc, 0x18, 0x38, 0xde, 0x82, 0xc,
    0x30, 0xc0, 0x0,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0x3a, 0x80,

    /* U+003C "<" */
    0x9, 0xb1, 0x83, 0x4,

    /* U+003E ">" */
    0x83, 0x6, 0x36, 0x40,

    /* U+0040 "@" */
    0x1e, 0x23, 0x45, 0x9d, 0xa5, 0xa5, 0xad, 0xb6,
    0x40, 0x3c,

    /* U+0041 "A" */
    0x38, 0x1c, 0x14, 0x14, 0x24, 0x22, 0x3e, 0x42,
    0x43, 0x41,

    /* U+0042 "B" */
    0xf8, 0x89, 0x12, 0x27, 0x88, 0xd0, 0xa1, 0x47,
    0xf8,

    /* U+0043 "C" */
    0x3a, 0x8e, 0x1c, 0x8, 0x10, 0x20, 0x43, 0x44,
    0x70,

    /* U+0044 "D" */
    0xf8, 0x89, 0x1a, 0x14, 0x28, 0x50, 0xa3, 0x45,
    0xf0,

    /* U+0045 "E" */
    0xfc, 0x89, 0x2, 0x7, 0x88, 0x10, 0xa1, 0x47,
    0xfc,

    /* U+0046 "F" */
    0xfc, 0x89, 0x12, 0x14, 0xf, 0x10, 0x20, 0x41,
    0xe0,

    /* U+0047 "G" */
    0x3a, 0x8e, 0xc, 0x8, 0x11, 0xe0, 0xc1, 0x46,
    0x74,

    /* U+0048 "H" */
    0xe7, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42,
    0x42, 0xe7,

    /* U+0049 "I" */
    0x7c, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x11,
    0xfc,

    /* U+004A "J" */
    0x1e, 0x8, 0x10, 0x20, 0x40, 0x81, 0x62, 0x4c,
    0x70,

    /* U+004B "K" */
    0xee, 0x44, 0x48, 0x50, 0x70, 0x78, 0x48, 0x4c,
    0x44, 0xef,

    /* U+004C "L" */
    0xe0, 0x81, 0x2, 0x4, 0x8, 0x10, 0xa1, 0x43,
    0xfc,

    /* U+004D "M" */
    0xe7, 0x66, 0x66, 0x5a, 0x5a, 0x5a, 0x52, 0x42,
    0x42, 0xe7,

    /* U+004E "N" */
    0xe2, 0xc5, 0x4a, 0x94, 0xa9, 0x51, 0xa3, 0x46,
    0x84,

    /* U+004F "O" */
    0x38, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x44,
    0x70,

    /* U+0050 "P" */
    0xfc, 0x8d, 0xa, 0x14, 0x6f, 0x90, 0x20, 0x41,
    0xe0,

    /* U+0051 "Q" */
    0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0xb2, 0xda,
    0x4c, 0x38, 0x9, 0x6,

    /* U+0052 "R" */
    0xfc, 0x46, 0x42, 0x42, 0x46, 0x78, 0x4c, 0x45,
    0x45, 0xf6,

    /* U+0053 "S" */
    0x3a, 0x8d, 0xb, 0x3, 0x81, 0xb0, 0xe1, 0xe7,
    0xb8,

    /* U+0054 "T" */
    0xff, 0x93, 0x91, 0x91, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x38,

    /* U+0055 "U" */
    0xe7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
    0x66, 0x3c,

    /* U+0056 "V" */
    0xf7, 0x62, 0x22, 0x26, 0x24, 0x34, 0x14, 0x18,
    0x18, 0x8,

    /* U+0057 "W" */
    0xe7, 0x82, 0xd2, 0x52, 0x5a, 0x5a, 0x6a, 0x6c,
    0x64, 0x64,

    /* U+0058 "X" */
    0x77, 0x26, 0x34, 0x18, 0x18, 0x18, 0x14, 0x24,
    0x22, 0xf7,

    /* U+0059 "Y" */
    0xee, 0x89, 0x11, 0x42, 0x82, 0x4, 0x8, 0x10,
    0x70,

    /* U+005A "Z" */
    0xfe, 0x28, 0x84, 0x30, 0x86, 0x51, 0x87, 0xf0,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x8f,

    /* U+005C "\\" */
    0x84, 0x10, 0x86, 0x10, 0x82, 0x10, 0xc2,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x1f,

    /* U+005E "^" */
    0x30, 0xa4, 0xe1,

    /* U+005F "_" */
    0xf8,

    /* U+0061 "a" */
    0x38, 0x44, 0xc, 0x74, 0x84, 0x8c, 0x76,

    /* U+0062 "b" */
    0xc0, 0x81, 0x2, 0xe6, 0x68, 0x50, 0xa1, 0x66,
    0xb8,

    /* U+0063 "c" */
    0x39, 0x18, 0x60, 0x83, 0x17, 0x80,

    /* U+0064 "d" */
    0xe, 0x4, 0x9, 0xd4, 0x70, 0x60, 0xc1, 0x46,
    0x74,

    /* U+0065 "e" */
    0x39, 0x18, 0x7f, 0x83, 0x17, 0x80,

    /* U+0066 "f" */
    0x1e, 0x44, 0x81, 0xf, 0xc4, 0x8, 0x10, 0x20,
    0x43, 0xf0,

    /* U+0067 "g" */
    0x7f, 0xa, 0x13, 0xc8, 0x10, 0x3f, 0x42, 0xf8,

    /* U+0068 "h" */
    0xc0, 0x81, 0x2, 0xe6, 0x28, 0x50, 0xa1, 0x42,
    0x84,

    /* U+0069 "i" */
    0x30, 0xc0, 0x38, 0x20, 0x82, 0x8, 0x23, 0xf0,

    /* U+006A "j" */
    0x8, 0x40, 0xf0, 0x84, 0x21, 0x8, 0x43, 0xe0,
    0x0,

    /* U+006B "k" */
    0xc0, 0x40, 0x40, 0x46, 0x4c, 0x58, 0x78, 0x6c,
    0x44, 0x42,

    /* U+006C "l" */
    0x70, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x11,
    0xfc,

    /* U+006D "m" */
    0xff, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49,

    /* U+006E "n" */
    0xdc, 0xc5, 0xa, 0x14, 0x28, 0x50, 0x80,

    /* U+006F "o" */
    0x38, 0x8a, 0xc, 0x18, 0x38, 0x8e, 0x0,

    /* U+0070 "p" */
    0xdc, 0xcd, 0xa, 0x14, 0x2c, 0xd7, 0x20, 0xf0,

    /* U+0071 "q" */
    0x3a, 0x46, 0x82, 0x82, 0x82, 0xc6, 0x7a, 0x2,
    0xf,

    /* U+0072 "r" */
    0xef, 0x33, 0x21, 0x20, 0x20, 0x20, 0xf8,

    /* U+0073 "s" */
    0x3c, 0x81, 0x81, 0xe0, 0x28, 0x4f, 0x0,

    /* U+0074 "t" */
    0x41, 0xf, 0xd0, 0x41, 0x4, 0x11, 0x38,

    /* U+0075 "u" */
    0xc6, 0x85, 0xa, 0x14, 0x28, 0xce, 0x80,

    /* U+0076 "v" */
    0x87, 0x89, 0x32, 0x46, 0x86, 0xc, 0x0,

    /* U+0077 "w" */
    0xc1, 0x41, 0x5b, 0x5a, 0x56, 0x26, 0x26,

    /* U+0078 "x" */
    0xc4, 0xd0, 0xe1, 0x82, 0x89, 0xb1, 0x80,

    /* U+0079 "y" */
    0x43, 0x62, 0x22, 0x34, 0x14, 0x18, 0x8, 0x10,
    0xf0, 0x0,

    /* U+007A "z" */
    0xfe, 0x29, 0x8, 0x67, 0x1f, 0xc0,

    /* U+007B "{" */
    0x3b, 0x18, 0x46, 0x41, 0x8c, 0x63, 0xc0,

    /* U+007C "|" */
    0xff, 0xf0,

    /* U+007D "}" */
    0xe1, 0x8c, 0x43, 0x4, 0xc6, 0x37, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 134, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 2, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 23, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 32, .adv_w = 134, .box_w = 1, .box_h = 4, .ofs_x = 4, .ofs_y = 6},
    {.bitmap_index = 33, .adv_w = 134, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 39, .adv_w = 134, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 45, .adv_w = 134, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 50, .adv_w = 134, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 54, .adv_w = 134, .box_w = 5, .box_h = 1, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 55, .adv_w = 134, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 134, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 81, .adv_w = 134, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 99, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 110, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 120, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 142, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 134, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 162, .adv_w = 134, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 134, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 167, .adv_w = 134, .box_w = 5, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 171, .adv_w = 134, .box_w = 5, .box_h = 6, .ofs_x = 2, .ofs_y = 2},
    {.bitmap_index = 175, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 333, .adv_w = 134, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 345, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 134, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 134, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 134, .box_w = 5, .box_h = 11, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 443, .adv_w = 134, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 134, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 451, .adv_w = 134, .box_w = 5, .box_h = 1, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 452, .adv_w = 134, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 459, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 468, .adv_w = 134, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 474, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 134, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 499, .adv_w = 134, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 507, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 134, .box_w = 6, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 524, .adv_w = 134, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 533, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 543, .adv_w = 134, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 552, .adv_w = 134, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 559, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 566, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 573, .adv_w = 134, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 581, .adv_w = 134, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 590, .adv_w = 134, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 604, .adv_w = 134, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 134, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 632, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 639, .adv_w = 134, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 649, .adv_w = 134, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 655, .adv_w = 134, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 662, .adv_w = 134, .box_w = 1, .box_h = 12, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 664, .adv_w = 134, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = 0}
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

extern const lv_font_t lv_font_montserrat_14;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t drafting_mono_14 = {
#else
lv_font_t drafting_mono_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
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
    .fallback = &lv_font_montserrat_14,
#endif
    .user_data = NULL,
};



#endif /*#if DRAFTING_MONO_14*/
