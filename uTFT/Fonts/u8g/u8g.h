#ifndef _U8G_H
#define _U8G_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
#include <stddef.h>
#include "stdlib.h"
#include "string.h"	
	
#include "uTFT_Buffer.h"	
	
#define U8G_FONT_HEIGHT_MODE_TEXT 0
#define U8G_FONT_HEIGHT_MODE_XTEXT 1
#define U8G_FONT_HEIGHT_MODE_ALL 2	
	
/*
  ... instead the fields of the font data structure are accessed directly by offset 
  font information 
  offset
  0             font format
  1             FONTBOUNDINGBOX width           unsigned                  ????, ?????????? ?????? ???????.
  2             FONTBOUNDINGBOX height          unsigned                  ????, ?????????? ?????? ???????.
  3             FONTBOUNDINGBOX x-offset        signed
  4             FONTBOUNDINGBOX y-offset        signed
  5             capital A height                unsigned                  ?????? ????????? 'A'
  6             start 'A'                                                 ?????? ? ????????? ??????? 'A'.
  8             start 'a'                                                 ?????? ? ????????? ??????? 'a'.
  10            encoding start                                            ????????? ?? ?????? ????????? ? ???????? ???? ????????, ??????? ????? ??????????.
  11            encoding end                                              ?????? ?????????, ?. ?. ??? ??????? ??????? ? ???????????????.
  12            descent 'g'                     negative: below baseline  ????????? ??? ????? g, ????????????? ????????: ???? ??????? ?????
  13            font max ascent                                           ???????????? ????????? ???????
  14            font min decent                 negative: below baseline  ???????????? ????????? ???????. ????????????? ????????: ???? ??????? ?????.
  15            font xascent
  16            font xdecent                    negative: below baseline 
  
*/

/* size of the font data structure, there is no struct or class... */
#define U8G_FONT_DATA_STRUCT_SIZE 17

//U8Glib
typedef void * u8g_glyph_t;

typedef uint8_t u8g_fntpgm_uint8_t;
typedef int8_t  u8g_int_t;

typedef struct _u8g_t u8g_t;
typedef uint8_t (*u8g_font_calc_vref_fnptr)(u8g_t *u8g);

#  define u8g_pgm_read(adr) (*(const uint8_t *)(adr)) 
#  define U8G_PSTR(s) ((uint8_t *)(s))

typedef uint8_t uint8_t;

typedef struct _u8g_t
{
  uint8_t width;
  uint8_t height;
  
  const uint8_t *font;             /* regular font for all text procedures */
  uint8_t cursor_fg_color, cursor_bg_color;
  uint8_t cursor_encoding;
  uint8_t mode;                         /* display mode, one of U8G_MODE_xxx */
  uint8_t cursor_x;
  uint8_t cursor_y;
  
  int8_t glyph_dx;
  int8_t glyph_x;
  int8_t glyph_y;
  uint8_t glyph_width;
  uint8_t glyph_height;
  
  u8g_font_calc_vref_fnptr font_calc_vref;
  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;
  uint8_t font_line_spacing_factor;     /* line_spacing = factor * (ascent - descent) / 64 */
  uint8_t line_spacing;
}_u8g_t;

extern _u8g_t u8g;

void u8g_SetFont(const u8g_fntpgm_uint8_t  *font);

extern uint8_t u8g_DrawStr(uTFT_LCD_t * LCD, uint8_t x, uint8_t y, const char *s);

uint8_t u8g_GetStrWidth(u8g_t *u8g, const char *s);

///////*===============================================================*/
///////* font definitions */
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_5[];
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_7[];
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_9[];

extern const u8g_fntpgm_uint8_t u8g_font_u8glib_4[];
extern const u8g_fntpgm_uint8_t u8g_font_u8glib_4r[];


extern const u8g_fntpgm_uint8_t u8g_font_6x12_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13_75r[];

extern const u8g_fntpgm_uint8_t u8g_font_8x13_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_cu12_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_75r[];
extern const u8g_fntpgm_uint8_t u8g_font_10x20_75r[];

extern const u8g_fntpgm_uint8_t u8g_font_10x20_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_10x20_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_10x20[];
extern const u8g_fntpgm_uint8_t u8g_font_10x20r[];
extern const u8g_fntpgm_uint8_t u8g_font_4x6[];
extern const u8g_fntpgm_uint8_t u8g_font_4x6r[];
//extern const u8g_fntpgm_uint8_t u8g_font_4x6n[];
extern const u8g_fntpgm_uint8_t u8g_font_5x7[];
extern const u8g_fntpgm_uint8_t u8g_font_5x7r[];
extern const u8g_fntpgm_uint8_t u8g_font_5x8[];
extern const u8g_fntpgm_uint8_t u8g_font_5x8r[];
extern const u8g_fntpgm_uint8_t u8g_font_6x10[];
extern const u8g_fntpgm_uint8_t u8g_font_6x10r[];
extern const u8g_fntpgm_uint8_t u8g_font_6x12_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_6x12_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_6x12[];
extern const u8g_fntpgm_uint8_t u8g_font_6x12r[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13B[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13Br[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13r[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13O[];
extern const u8g_fntpgm_uint8_t u8g_font_6x13Or[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13B[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13Br[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13r[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13O[];
extern const u8g_fntpgm_uint8_t u8g_font_7x13Or[];
extern const u8g_fntpgm_uint8_t u8g_font_7x14B[];
extern const u8g_fntpgm_uint8_t u8g_font_7x14Br[];
extern const u8g_fntpgm_uint8_t u8g_font_7x14[];
extern const u8g_fntpgm_uint8_t u8g_font_7x14r[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13B[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13Br[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13r[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13O[];
extern const u8g_fntpgm_uint8_t u8g_font_8x13Or[];

extern const u8g_fntpgm_uint8_t u8g_font_9x15_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15B[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15Br[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15[];
extern const u8g_fntpgm_uint8_t u8g_font_9x15r[];

extern const u8g_fntpgm_uint8_t u8g_font_9x18_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18B[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18Br[];
extern const u8g_fntpgm_uint8_t u8g_font_9x18r[];

extern const u8g_fntpgm_uint8_t u8g_font_cursor[];
extern const u8g_fntpgm_uint8_t u8g_font_cursorr[];
extern const u8g_fntpgm_uint8_t u8g_font_micro[];

extern const u8g_fntpgm_uint8_t u8g_font_cu12_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_cu12_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_cu12[];

///////* 
//////  Free-Universal Bold 
//////  r: Reduced char set (codes 32 - 128)
//////  n: Numbers (codes 42 - 57)
//////  no char: Full set (codes 32 - 255)
//////*/

extern const u8g_fntpgm_uint8_t u8g_font_fub11[];
extern const u8g_fntpgm_uint8_t u8g_font_fub11r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub11n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub14[];
extern const u8g_fntpgm_uint8_t u8g_font_fub14r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub14n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub17[];
extern const u8g_fntpgm_uint8_t u8g_font_fub17r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub17n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub20[];
extern const u8g_fntpgm_uint8_t u8g_font_fub20r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub20n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub25[];
extern const u8g_fntpgm_uint8_t u8g_font_fub25r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub25n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub30[];
extern const u8g_fntpgm_uint8_t u8g_font_fub30r[];
extern const u8g_fntpgm_uint8_t u8g_font_fub30n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub35n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub42n[];
extern const u8g_fntpgm_uint8_t u8g_font_fub49n[];

/* 
  Free-Universal Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_fur11[];
extern const u8g_fntpgm_uint8_t u8g_font_fur11r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur11n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur14[];
extern const u8g_fntpgm_uint8_t u8g_font_fur14r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur14n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur17[];
extern const u8g_fntpgm_uint8_t u8g_font_fur17r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur17n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur20[];
extern const u8g_fntpgm_uint8_t u8g_font_fur20r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur20n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur25[];
extern const u8g_fntpgm_uint8_t u8g_font_fur25r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur25n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur30[];
extern const u8g_fntpgm_uint8_t u8g_font_fur30r[];
extern const u8g_fntpgm_uint8_t u8g_font_fur30n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur35n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur42n[];
extern const u8g_fntpgm_uint8_t u8g_font_fur49n[];

/* 
  Gentium Bold
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_gdb11[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb12[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb14[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb17[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb20[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb25[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb30[];

extern const u8g_fntpgm_uint8_t u8g_font_gdb11r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb12r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb14r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb17r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb20r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb25r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb30r[];

extern const u8g_fntpgm_uint8_t u8g_font_gdb11n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb12n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb14n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb17n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb20n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb25n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdb30n[];

/* 
  Gentium Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_gdr9[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr10[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr11[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr12[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr14[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr17[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr20[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr25[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr30[];

extern const u8g_fntpgm_uint8_t u8g_font_gdr9r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr10r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr11r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr12r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr14r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr17r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr20r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr25r[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr30r[];

extern const u8g_fntpgm_uint8_t u8g_font_gdr9n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr10n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr11n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr12n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr14n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr17n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr20n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr25n[];
extern const u8g_fntpgm_uint8_t u8g_font_gdr30n[];

/* 
  Old-Standard Bold
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_osb18[];
extern const u8g_fntpgm_uint8_t u8g_font_osb21[];
extern const u8g_fntpgm_uint8_t u8g_font_osb26[];
extern const u8g_fntpgm_uint8_t u8g_font_osb29[];
extern const u8g_fntpgm_uint8_t u8g_font_osb35[];

extern const u8g_fntpgm_uint8_t u8g_font_osb18r[];
extern const u8g_fntpgm_uint8_t u8g_font_osb21r[];
extern const u8g_fntpgm_uint8_t u8g_font_osb26r[];
extern const u8g_fntpgm_uint8_t u8g_font_osb29r[];
extern const u8g_fntpgm_uint8_t u8g_font_osb35r[];

extern const u8g_fntpgm_uint8_t u8g_font_osb18n[];
extern const u8g_fntpgm_uint8_t u8g_font_osb21n[];
extern const u8g_fntpgm_uint8_t u8g_font_osb26n[];
extern const u8g_fntpgm_uint8_t u8g_font_osb29n[];
extern const u8g_fntpgm_uint8_t u8g_font_osb35n[];

/* 
  Old-Standard Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_osr18[];
extern const u8g_fntpgm_uint8_t u8g_font_osr21[];
extern const u8g_fntpgm_uint8_t u8g_font_osr26[];
extern const u8g_fntpgm_uint8_t u8g_font_osr29[];
extern const u8g_fntpgm_uint8_t u8g_font_osr35[];

extern const u8g_fntpgm_uint8_t u8g_font_osr18r[];
extern const u8g_fntpgm_uint8_t u8g_font_osr21r[];
extern const u8g_fntpgm_uint8_t u8g_font_osr26r[];
extern const u8g_fntpgm_uint8_t u8g_font_osr29r[];
extern const u8g_fntpgm_uint8_t u8g_font_osr35r[];

extern const u8g_fntpgm_uint8_t u8g_font_osr18n[];
extern const u8g_fntpgm_uint8_t u8g_font_osr21n[];
extern const u8g_fntpgm_uint8_t u8g_font_osr26n[];
extern const u8g_fntpgm_uint8_t u8g_font_osr29n[];
extern const u8g_fntpgm_uint8_t u8g_font_osr35n[];

//extern const u8g_fntpgm_uint8_t u8g_font_osr41[];

/* GNU unifont */

extern const u8g_fntpgm_uint8_t u8g_font_unifont_18_19[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_72_73[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_67_75[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_76[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_77[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_78_79[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_86[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont[];
extern const u8g_fntpgm_uint8_t u8g_font_unifontr[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_0_8[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_2_3[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_4_5[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_8_9[];
extern const u8g_fntpgm_uint8_t u8g_font_unifont_12_13[];


/* 04b fonts */

extern const u8g_fntpgm_uint8_t u8g_font_04b_03b[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_03bn[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_03br[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_03[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_03n[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_03r[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_24[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_24n[];
extern const u8g_fntpgm_uint8_t u8g_font_04b_24r[];

/* orgdot fonts */

extern const u8g_fntpgm_uint8_t u8g_font_orgv01[];
extern const u8g_fntpgm_uint8_t u8g_font_orgv01r[];
extern const u8g_fntpgm_uint8_t u8g_font_orgv01n[];

extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0[];
extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0r[];
extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0n[];

extern const u8g_fntpgm_uint8_t u8g_font_tpssb[];
extern const u8g_fntpgm_uint8_t u8g_font_tpssbr[];
extern const u8g_fntpgm_uint8_t u8g_font_tpssbn[];

extern const u8g_fntpgm_uint8_t u8g_font_tpss[];
extern const u8g_fntpgm_uint8_t u8g_font_tpssr[];
extern const u8g_fntpgm_uint8_t u8g_font_tpssn[];

/* contributed */

extern const u8g_fntpgm_uint8_t u8g_font_freedoomr25n[];
extern const u8g_fntpgm_uint8_t u8g_font_freedoomr10r[];

/* adobe X11 */
extern const u8g_fntpgm_uint8_t u8g_font_courB08[];
extern const u8g_fntpgm_uint8_t u8g_font_courB08r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB10[];
extern const u8g_fntpgm_uint8_t u8g_font_courB10r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB12[];
extern const u8g_fntpgm_uint8_t u8g_font_courB12r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB14[];
extern const u8g_fntpgm_uint8_t u8g_font_courB14r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB18[];
extern const u8g_fntpgm_uint8_t u8g_font_courB18r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB24[];
extern const u8g_fntpgm_uint8_t u8g_font_courB24r[];
extern const u8g_fntpgm_uint8_t u8g_font_courB24n[];

extern const u8g_fntpgm_uint8_t u8g_font_courR08[];
extern const u8g_fntpgm_uint8_t u8g_font_courR08r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR10[];
extern const u8g_fntpgm_uint8_t u8g_font_courR10r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR12[];
extern const u8g_fntpgm_uint8_t u8g_font_courR12r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR14[];
extern const u8g_fntpgm_uint8_t u8g_font_courR14r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR18[];
extern const u8g_fntpgm_uint8_t u8g_font_courR18r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR24[];
extern const u8g_fntpgm_uint8_t u8g_font_courR24r[];
extern const u8g_fntpgm_uint8_t u8g_font_courR24n[];

extern const u8g_fntpgm_uint8_t u8g_font_helvB08[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB08r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB08n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB10[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB10r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB10n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB12[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB12r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB12n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB14[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB14r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB14n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB18[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB18r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB18n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB24[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB24r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvB24n[];

extern const u8g_fntpgm_uint8_t u8g_font_helvR08[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR08r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR08n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR10[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR10r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR10n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR12[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR12r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR12n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR14[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR14r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR14n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR18[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR18r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR18n[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR24[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR24r[];
extern const u8g_fntpgm_uint8_t u8g_font_helvR24n[];

extern const u8g_fntpgm_uint8_t u8g_font_ncenB08[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB08r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB10[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB10r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB12[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB12r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB14[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB14r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB18[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB18r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24n[];

extern const u8g_fntpgm_uint8_t u8g_font_ncenR08[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR08r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR10[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR10r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR12[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR12r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR14[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR14r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR18[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR18r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24r[];
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24n[];

extern const u8g_fntpgm_uint8_t u8g_font_symb08[];
extern const u8g_fntpgm_uint8_t u8g_font_symb08r[];
extern const u8g_fntpgm_uint8_t u8g_font_symb10[];
extern const u8g_fntpgm_uint8_t u8g_font_symb10r[];
extern const u8g_fntpgm_uint8_t u8g_font_symb12[];
extern const u8g_fntpgm_uint8_t u8g_font_symb12r[];
extern const u8g_fntpgm_uint8_t u8g_font_symb14[];
extern const u8g_fntpgm_uint8_t u8g_font_symb14r[];
extern const u8g_fntpgm_uint8_t u8g_font_symb18[];
extern const u8g_fntpgm_uint8_t u8g_font_symb18r[];
extern const u8g_fntpgm_uint8_t u8g_font_symb24[];
extern const u8g_fntpgm_uint8_t u8g_font_symb24r[];

extern const u8g_fntpgm_uint8_t u8g_font_timB08[];
extern const u8g_fntpgm_uint8_t u8g_font_timB08r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB10[];
extern const u8g_fntpgm_uint8_t u8g_font_timB10r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB12[];
extern const u8g_fntpgm_uint8_t u8g_font_timB12r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB14[];
extern const u8g_fntpgm_uint8_t u8g_font_timB14r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB18[];
extern const u8g_fntpgm_uint8_t u8g_font_timB18r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB24[];
extern const u8g_fntpgm_uint8_t u8g_font_timB24r[];
extern const u8g_fntpgm_uint8_t u8g_font_timB24n[];

extern const u8g_fntpgm_uint8_t u8g_font_timR08[];
extern const u8g_fntpgm_uint8_t u8g_font_timR08r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR10[];
extern const u8g_fntpgm_uint8_t u8g_font_timR10r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR12[];
extern const u8g_fntpgm_uint8_t u8g_font_timR12r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR14[];
extern const u8g_fntpgm_uint8_t u8g_font_timR14r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR18[];
extern const u8g_fntpgm_uint8_t u8g_font_timR18r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR24[];
extern const u8g_fntpgm_uint8_t u8g_font_timR24r[];
extern const u8g_fntpgm_uint8_t u8g_font_timR24n[];

/* fontstruct */

extern const u8g_fntpgm_uint8_t u8g_font_p01type[];
extern const u8g_fntpgm_uint8_t u8g_font_p01typer[];
extern const u8g_fntpgm_uint8_t u8g_font_p01typen[];

extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternate[];
extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternater[];
extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternaten[];

extern const u8g_fntpgm_uint8_t u8g_font_chikita[];
extern const u8g_fntpgm_uint8_t u8g_font_chikitar[];
extern const u8g_fntpgm_uint8_t u8g_font_chikitan[];

extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micro[];
extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micror[];
extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micron[];

extern const u8g_fntpgm_uint8_t u8g_font_trixel_square[];
extern const u8g_fntpgm_uint8_t u8g_font_trixel_squarer[];
extern const u8g_fntpgm_uint8_t u8g_font_trixel_squaren[];

extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niro[];
extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niror[];
extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niron[];

extern const u8g_fntpgm_uint8_t u8g_font_baby[];
extern const u8g_fntpgm_uint8_t u8g_font_babyr[];
extern const u8g_fntpgm_uint8_t u8g_font_babyn[];

extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07[];
extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07r[];
extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07n[];

/* profont */

extern const u8g_fntpgm_uint8_t u8g_font_profont10[];
extern const u8g_fntpgm_uint8_t u8g_font_profont10r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont11[];
extern const u8g_fntpgm_uint8_t u8g_font_profont11r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont12[];
extern const u8g_fntpgm_uint8_t u8g_font_profont12r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont15[];
extern const u8g_fntpgm_uint8_t u8g_font_profont15r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont17[];
extern const u8g_fntpgm_uint8_t u8g_font_profont17r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont22[];
extern const u8g_fntpgm_uint8_t u8g_font_profont22r[];
extern const u8g_fntpgm_uint8_t u8g_font_profont29[];
extern const u8g_fntpgm_uint8_t u8g_font_profont29r[];

extern const u8g_fntpgm_uint8_t rusMax20[];

#ifdef __cplusplus
}
#endif

#endif /* _U8G_H */

