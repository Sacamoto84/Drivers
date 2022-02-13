//#include "global.h"
#include "uTFT_define.h"

//extern volatile uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8 + 10 ];

#include "u8g.h"

_u8g_t u8g;

volatile uint8_t u8g_Buffer_On_Overflow_Protect;  //Включение режима защиты от выхода за границы буфера, отключение ускоряет отрисовку

//http://microsin.net/programming/avr/u8glib-font-and-string-handling.html

//Включение режима защиты от выхода заграницы буфера при отрисовки строк
void u8g_Buffer_On_Overflow_Protect_On_Off(uint32_t i)
{
	u8g_Buffer_On_Overflow_Protect = i;
}

void u8g_Draw8Pixel(uTFT_LCD_t* LCD, u8g_t *u8g, int32_t ix, int32_t iy, uint8_t dir, uint8_t data)
{
	uint8_t i;
	i = 0;
	uint8_t ii;
	if (dir) //По Y
	{  
		for(i=0;i<8;i++)
		{
			ii =  (data>>i)&0x01;
			if (ii)
				uTFT_SetPixel(LCD, ix, iy-i,  1);
			else
				uTFT_SetPixel(LCD, ix, iy-i,  0);
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			ii =  (data>>i)&0x01;
			if (ii)
				uTFT_SetPixel(LCD, ix-i, iy,  1);
			else
				uTFT_SetPixel(LCD, ix-i, iy,  0);
		}
	}
}

//C защитой от переполнения
void u8g_Draw8Pixel_dir_0(uTFT_LCD_t * LCD, int32_t ix, int32_t iy, uint8_t data)
{
	int32_t i;
	int32_t ii;
  int32_t q;
	
for(i=0;i<8;i++)
	{
	ii =  (data>>i) & 0x01;
	q = ix-i;	
	if (ii)
      uTFT_SetPixel(LCD, q, iy, uTFT.Color);
  }
//				
////      uTFT_SetPixel(ix-i, iy,  uTFT.Color);
////      //SSD1306_SetPixel(ix-i, iy, 1);
////	    SSD1306_Buffer[(ix-i) + (iy / 8) * 128]  |= 1 << (iy % 8);
////			
////			
////			
//			
//				//uTFT_SetPixel(ix-i, iy,  uTFT.BColor);
////			SSD1306_Buffer[(ix-i) + (iy / 8) * 128] &= ~(1 << (iy % 8));			
////	

////	else
////		#if (LCD_Buffer_Color_Bit == 16)    			
////				{	
////						if ((q<0)||(iy<0)||(q >= TFT_WIDTH )||	(iy >= TFT_HEIGHT)) 	return;
////						LCD_Buffer[q + iy*TFT_WIDTH]=  uTFT.BColor;
////				}
////		#endif	
}

/*========================================================================*/
/* low level byte and word access */

/* removed NOINLINE, because it leads to smaller code, might also be faster */
static uint8_t u8g_font_get_byte(const uint8_t *font, uint8_t offset)
{
	uint8_t r;
  font += offset;
	r = *font;
  return r;  
}

static uint16_t u8g_font_get_word(const uint8_t *font, uint8_t offset)
{
    uint16_t pos;
	
    font += offset;
    pos =  *font ;
    font++;
    pos <<= 8;
    pos += *font;
    return pos;
}


/*========================================================================*/
/* direct access on the font */
static uint8_t u8g_font_GetFormat(const u8g_fntpgm_uint8_t *font)
{
  return u8g_font_get_byte(font, 0);
}

static uint8_t u8g_font_GetFontGlyphStructureSize(const u8g_fntpgm_uint8_t *font)
{
  switch(u8g_font_GetFormat(font))
  {
    case 0: return 6;
    case 1: return 3;
    case 2: return 6;
  }
  return 3;
}

static uint8_t u8g_font_GetBBXWidth(const void *font)
{
  return u8g_font_get_byte(font, 1);
}

static uint8_t u8g_font_GetBBXHeight(const void *font)
{
  return u8g_font_get_byte(font, 2);
}

static int8_t u8g_font_GetBBXOffX(const void *font)
{
  return u8g_font_get_byte(font, 3);
}

static int8_t u8g_font_GetBBXOffY(const void *font)
{
  return u8g_font_get_byte(font, 4);
}

uint8_t u8g_font_GetCapitalAHeight(const void *font)
{
  return u8g_font_get_byte(font, 5);
}

uint16_t u8g_font_GetEncoding65Pos(const void *font)
{
    return u8g_font_get_word(font, 6);
}

uint16_t u8g_font_GetEncoding97Pos(const void *font)
{
    return u8g_font_get_word(font, 8);
}

uint8_t u8g_font_GetFontStartEncoding(const void *font)
{
  return u8g_font_get_byte(font, 10);
}

uint8_t u8g_font_GetFontEndEncoding(const void *font)
{
  return u8g_font_get_byte(font, 11);
}

int8_t u8g_font_GetLowerGDescent(const void *font)
{
  return u8g_font_get_byte(font, 12);
}

int8_t u8g_font_GetFontAscent(const void *font)
{
  return u8g_font_get_byte(font, 13);
}

int8_t u8g_font_GetFontDescent(const void *font)
{
  return u8g_font_get_byte(font, 14);
}

int8_t u8g_font_GetFontXAscent(const void *font)
{
  return u8g_font_get_byte(font, 15);
}

int8_t u8g_font_GetFontXDescent(const void *font)
{
  return u8g_font_get_byte(font, 16);
}

/* return the data start for a font and the glyph pointer */
static uint8_t *u8g_font_GetGlyphDataStart(const void *font, u8g_glyph_t g)
{
  return ((u8g_fntpgm_uint8_t *)g) + u8g_font_GetFontGlyphStructureSize(font);
}

/* calculate the overall length of the font, only used to create the picture for the google wiki */
size_t u8g_font_GetSize(const void *font)
{
  uint8_t *p = (uint8_t *)(font);
  uint8_t font_format = u8g_font_GetFormat(font);
  uint8_t data_structure_size = u8g_font_GetFontGlyphStructureSize(font);
  uint8_t start, end;
  uint8_t i;
  uint8_t mask = 255;
  
  start = u8g_font_GetFontStartEncoding(font);
  end = u8g_font_GetFontEndEncoding(font);

  if ( font_format == 1 )
    mask = 15;

  p += U8G_FONT_DATA_STRUCT_SIZE;       /* skip font general information */  

  i = start;  
  for(;;)
  {
    if ( u8g_pgm_read((uint8_t *)(p)) == 255 )
    {
      p += 1;
    }
    else
    {
      p += u8g_pgm_read( ((uint8_t *)(p)) + 2 ) & mask;
      p += data_structure_size;
    }
    if ( i == end )
      break;
    i++;
  }
    
  return p - (uint8_t *)font;
}

/*========================================================================*/
/* glyph handling */
static void u8g_CopyGlyphDataToCache(u8g_t *u8g, u8g_glyph_t g)
{
  uint8_t tmp;
  switch( u8g_font_GetFormat(u8g->font) )
  {
    case 0:
    case 2:
  /*
    format 0
    glyph information 
    offset
    0             BBX width                                       unsigned
    1             BBX height                                      unsigned
    2             data size                                       unsigned    (BBX width + 7)/8 * BBX height
    3             DWIDTH                                          signed
    4             BBX xoffset                                     signed
    5             BBX yoffset                                     signed
  byte 0 == 255 indicates empty glyph
  */
      u8g->glyph_width =  u8g_pgm_read( ((uint8_t *)g) + 0 );
      u8g->glyph_height =  u8g_pgm_read( ((uint8_t *)g) + 1 );
      u8g->glyph_dx =  u8g_pgm_read( ((uint8_t *)g) + 3 );
      u8g->glyph_x =  u8g_pgm_read( ((uint8_t *)g) + 4 );
      u8g->glyph_y =  u8g_pgm_read( ((uint8_t *)g) + 5 );
      break;
    case 1:
    default:
      /*
format 1
  0             BBX xoffset                                    signed   --> upper 4 Bit
  0             BBX yoffset                                    signed --> lower 4 Bit
  1             BBX width                                      unsigned --> upper 4 Bit
  1             BBX height                                     unsigned --> lower 4 Bit
  2             data size                                      unsigned -(BBX width + 7)/8 * BBX height  --> lower 4 Bit
  2             DWIDTH                                         signed --> upper  4 Bit
  byte 0 == 255 indicates empty glyph
      */
    
      tmp = u8g_pgm_read( ((uint8_t *)g) + 0 );
      u8g->glyph_y =  tmp & 15;
      u8g->glyph_y-=2;
      tmp >>= 4;
      u8g->glyph_x =  tmp;
    
      tmp = u8g_pgm_read( ((uint8_t *)g) + 1 );
      u8g->glyph_height =  tmp & 15;
      tmp >>= 4;
      u8g->glyph_width =  tmp;
      
      tmp = u8g_pgm_read( ((uint8_t *)g) + 2 );
      tmp >>= 4;
      u8g->glyph_dx = tmp;
    
      
      break;
  }
}

//void u8g_FillEmptyGlyphCache(u8g_t *u8g) U8G_NOINLINE;
static void u8g_FillEmptyGlyphCache(u8g_t *u8g)
{
  u8g->glyph_dx = 0;
  u8g->glyph_width = 0;
  u8g->glyph_height = 0;
  u8g->glyph_x = 0;
  u8g->glyph_y = 0;
}

/*
  Find (with some speed optimization) and return a pointer to the glyph data structure
  Also uncompress (format 1) and copy the content of the data structure to the u8g structure
*/
u8g_glyph_t u8g_GetGlyph(u8g_t *u8g, uint8_t requested_encoding)
{
  uint8_t *p = (uint8_t *)(u8g->font);
  uint8_t font_format = u8g_font_GetFormat(u8g->font);
  uint8_t data_structure_size = u8g_font_GetFontGlyphStructureSize(u8g->font);
  uint8_t start, end;
  uint16_t pos;
  uint8_t i;
  uint8_t mask = 255;

  if ( font_format == 1 ) mask = 15;
  
  start = u8g_font_GetFontStartEncoding(u8g->font);
  end =   u8g_font_GetFontEndEncoding(u8g->font);

  pos = u8g_font_GetEncoding97Pos(u8g->font);
	
  if ( requested_encoding >= 97 && pos > 0 )
  {
    p+= pos;
    start = 97;
  }

  else 
  {
    pos = u8g_font_GetEncoding65Pos(u8g->font);
    if ( requested_encoding >= 65 && pos > 0 )
    {
      p+= pos;
      start = 65;
    }
    else
      p += U8G_FONT_DATA_STRUCT_SIZE;       /* skip font general information 17*/   
  }
  
  if ( requested_encoding > end )
  {
    u8g_FillEmptyGlyphCache(u8g);
    return NULL;                      /* not found */
  }
  
  i = start;
  if ( i <= end )
  {
    for(;;)
    {
      if ( u8g_pgm_read((uint8_t *)(p)) == 255 )
      {
        p += 1;
      }
      else
      {
        if ( i == requested_encoding )
        {
          u8g_CopyGlyphDataToCache(u8g, p);
          return p;
        }
        p += u8g_pgm_read( ((uint8_t *)(p)) + 2 ) & mask;
        p += data_structure_size;
      }
      if ( i == end )
        break;
      i++;
    }
  }
  
  u8g_FillEmptyGlyphCache(u8g);
    
  return NULL;
}

/*========================================================================*/
/* glyph drawing procedures */
int8_t u8g_draw_glyph(uTFT_LCD_t * LCD, u8g_t *u8g, uint8_t x, uint8_t y, uint8_t encoding)
{
  const uint8_t *data;
  uint32_t w, h;
  uint32_t i, j;
	
  //uint8_t ix, iy;
  int32_t ix, iy;
	
  //{
    u8g_glyph_t g = u8g_GetGlyph(u8g, encoding);
    if ( g == NULL  )
      return 0;
    data = u8g_font_GetGlyphDataStart(u8g->font, g);
  //}
  
  w = u8g->glyph_width;
  h = u8g->glyph_height;
  
  x += u8g->glyph_x;
  y -= u8g->glyph_y;
  y--;
  
  /* now, w is reused as bytes per line */
  w += 7;
  w /= 8;
  
  iy = y;
  iy -= h;
  iy++;
	
  for( j = 0; j < h; j++ )
  {
    ix = x;
    for( i = 0; i < w; i++ )
    {
			u8g_Draw8Pixel_dir_0(LCD, ix, iy, u8g_pgm_read(data));					
      data++;
      ix+=8;
    }
    iy++;
  }
  return u8g->glyph_dx;
}

/*========================================================================*/
/* string drawing procedures */
//uint8_t u8g_DrawStr(u8g_t *u8g, uint8_t x, uint8_t y, const char *s)
uint8_t u8g_DrawStr(uTFT_LCD_t * LCD, uint8_t x, uint8_t y, const char *s)
{
	
  uint8_t t = 0;
  int8_t d;
    
  while( *s != '\0' )
  {
        d = u8g_draw_glyph(LCD, &u8g, x, y, *s);
		
		x += d;
    t += d;
    s++;
  }
  return t;
}

/*========================================================================*/
/* set ascent/descent for reference point calculation */

void u8g_UpdateRefHeight(u8g_t *u8g)
{
  uint16_t ls;
  if ( u8g->font == NULL ) return;
	
  if ( u8g->font_height_mode == U8G_FONT_HEIGHT_MODE_TEXT )
  {
    u8g->font_ref_ascent = u8g_font_GetCapitalAHeight(u8g->font);
    u8g->font_ref_descent = u8g_font_GetLowerGDescent(u8g->font);
  }
 else if ( u8g->font_height_mode == U8G_FONT_HEIGHT_MODE_XTEXT )
  {
    u8g->font_ref_ascent = u8g_font_GetFontXAscent(u8g->font);
    u8g->font_ref_descent = u8g_font_GetFontXDescent(u8g->font);
  }
  else
  {
    u8g->font_ref_ascent = u8g_font_GetFontAscent(u8g->font);
    u8g->font_ref_descent = u8g_font_GetFontDescent(u8g->font);
  }
  
  ls = u8g->font_ref_ascent - u8g->font_ref_descent;
  if ( u8g->font_line_spacing_factor != 64 )
  {
    ls &= 255;
    ls *= u8g->font_line_spacing_factor;
    ls >>= 6;
  }
  u8g->line_spacing = ls;
}

void u8g_SetFontRefHeightText(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_TEXT;
  u8g_UpdateRefHeight(u8g);
}

void u8g_SetFontRefHeightExtendedText(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_XTEXT;
  u8g_UpdateRefHeight(u8g);
}


void u8g_SetFontRefHeightAll(u8g_t *u8g)
{
  u8g->font_height_mode = U8G_FONT_HEIGHT_MODE_ALL;
  u8g_UpdateRefHeight(u8g);
}

/* factor = 64: linespaceing == ascent and descent */
void u8g_SetFontLineSpacingFactor(u8g_t *u8g, uint8_t  factor)
{
  u8g->font_line_spacing_factor = factor;
  u8g_UpdateRefHeight(u8g);
}



/*=========================================================================*/
/* callback procedures to correct the y position */

uint8_t u8g_font_calc_vref_font(u8g_t *u8g)
{
  return 0;
}

void u8g_SetFontPosBaseline(u8g_t *u8g)
{
  u8g->font_calc_vref = u8g_font_calc_vref_font;
}


uint8_t u8g_font_calc_vref_bottom(u8g_t *u8g)
{
  /* y += (uint8_t)(u8g_int_t)(u8g->font_ref_descent); */
  return (uint8_t)(u8g_int_t)(u8g->font_ref_descent);
}

void u8g_SetFontPosBottom(u8g_t *u8g)
{
 //////// u8g->font_calc_vref = u8g_font_calc_vref_bottom;
}

uint8_t u8g_font_calc_vref_top(u8g_t *u8g)
{
  uint8_t tmp;
  /* reference pos is one pixel above the upper edge of the reference glyph */

  /*
  y += (uint8_t)(u8g_int_t)(u8g->font_ref_ascent);
  y++;
  */
  tmp = (uint8_t)(u8g_int_t)(u8g->font_ref_ascent);
  tmp++;
  return tmp;
}

void u8g_SetFontPosTop(u8g_t *u8g)
{
 u8g->font_calc_vref = u8g_font_calc_vref_top;
}

uint8_t u8g_font_calc_vref_center(u8g_t *u8g)
{
  int8_t tmp;
  tmp = u8g->font_ref_ascent;
  tmp -= u8g->font_ref_descent;
  tmp /= 2;
  tmp += u8g->font_ref_descent;  
  /* y += (uint8_t)(u8g_int_t)(tmp); */
  return tmp;
}

void u8g_SetFontPosCenter(u8g_t *u8g)
{
 ///////// u8g->font_calc_vref = u8g_font_calc_vref_center;
}

/*========================================================================*/
/* string pixel width calculation */

char u8g_font_get_char(const void *s)
{
  return *(const char *)(s);
}


typedef char (*u8g_font_get_char_fn)(const void *s);

uint8_t u8g_font_calc_str_pixel_width(u8g_t *u8g, const char *s, u8g_font_get_char_fn get_char )
{
  uint8_t  w;
  uint8_t enc;
  
  /* reset the total minimal width to zero, this will be expanded during calculation */
  w = 0;
    
  enc = get_char(s);
  
  /* check for empty string, width is already 0 */
  if ( enc == '\0' )
  {
    return w;
  }
  
  /* get the glyph information of the first char. This must be valid, because we already checked for the empty string */
  /* if *s is not inside the font, then the cached parameters of the glyph are all zero */
  u8g_GetGlyph(u8g, enc);

  /* strlen(s) == 1:       width = width(s[0]) */
  /* strlen(s) == 2:       width = - offx(s[0]) + deltax(s[0]) + offx(s[1]) + width(s[1]) */
  /* strlen(s) == 3:       width = - offx(s[0]) + deltax(s[0]) + deltax(s[1]) + offx(s[2]) + width(s[2]) */
  
  /* assume that the string has size 2 or more, than start with negative offset-x */
  /* for string with size 1, this will be nullified after the loop */
  w = -u8g->glyph_x;  
  for(;;)
  {
    
    /* check and stop if the end of the string is reached */
    s++;
    if ( get_char(s) == '\0' )
      break;
    
    /* if there are still more characters, add the delta to the next glyph */
    w += u8g->glyph_dx;
    
    /* store the encoding in a local variable, used also after the for(;;) loop */
    enc = get_char(s);
    
    /* load the next glyph information */
    u8g_GetGlyph(u8g, enc);
  }
  
  /* finally calculate the width of the last char */
  /* here is another exception, if the last char is a black, use the dx value instead */
  if ( enc != ' ' )
  {
    /* if g was not updated in the for loop (strlen() == 1), then the initial offset x gets removed */
    w += u8g->glyph_width;
    w += u8g->glyph_x;
  }
  else
  {
    w += u8g->glyph_dx;
  }
  
  
  return w;
}
/*========================================================================*/
//Определение длшины строки
uint8_t u8g_GetStrWidth(u8g_t *u8g, const char *s)
{
  uint8_t  w;
  uint8_t encoding;
  
  /* reset the total width to zero, this will be expanded during calculation */
  w = 0;
  
  for(;;)
  {
    encoding = *s;
    if ( encoding == 0 )
      break;

    /* load glyph information */
    u8g_GetGlyph(u8g, encoding);
    w += u8g->glyph_dx;    
    
    /* goto next char */
    s++;
  }
  
  return w;  
}
/*========================================================================*/
/* calculation of font/glyph/string characteristics */
/*
  Description:
    Calculate parameter for the minimal bounding box on a given string
  Output
    buf->y_min          extend of the lower left edge if the string below (y_min<0) or above (y_min>0) baseline (descent)
    buf->y_max          extend of the upper left edge if the string below (y_min<0) or above (y_min>0) baseline (ascent)
    buf->w                 the width of the string
*/
struct u8g_str_size_struct
{
  int8_t y_min;         /* descent */
  int8_t y_max;         /* ascent */
  int8_t x, y;          /* the reference point of the font (negated!) */
  uint8_t w;         /* width of the overall string */
};
typedef struct u8g_str_size_struct u8g_str_size_t;

//u8g.setFont(u8g_font_osb18);
//u8g.setFontPosTop();
//u8g.drawStr(0, 20, "ABC");

void u8g_SetFont(const u8g_fntpgm_uint8_t  *font)
{
  if ( u8g.font != font )
  {
    u8g.font = font;             //Запись адреса массива в структуру
    u8g_UpdateRefHeight(&u8g);
    u8g_SetFontPosBaseline(&u8g);
  }
}





