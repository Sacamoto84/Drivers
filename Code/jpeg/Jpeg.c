#include "tjpgd.h"
#include "stm32f4xx_hal.h"

#include "fatfs.h"
#include "ff.h"
//#include "lcd_global.h"
#include "uTFT_define.h"

extern char str1[60];

void load_jpg (
    FIL *fp,        /* Pointer to the open file object to load */
    void *work,     /* Pointer to the working buffer (must be 4-byte aligned) */
    UINT sz_work    /* Size of the working buffer (must be power of 2) */
);
		
typedef struct {
    FIL *fp;       /* File pointer for input function */
    BYTE *fbuf;    /* Pointer to the frame buffer for output function */
    UINT wfbuf;    /* Width of the frame buffer [pix] */
} IODEV;

JDEC jd;
IODEV iodiv;
JRESULT res;
FIL JpegEncFile;
BYTE jdwork[5000];

UINT input_func(    JDEC* jd,       /* Decoder object */
    BYTE* buff,     /* Pointer to the read buffer (NULL:skip) */
    UINT nd         /* Number of bytes to read/skip from input stream */
)
{
    UINT rb;
    FIL *fil = (FIL*)jd->device;    /* Input stream of this session */
 
    if (buff) { /* Read nd bytes from the input strem */
        f_read(fil, buff, nd, &rb);
        return rb;  /* Returns number of bytes could be read */
 
    } else {    /* Skip nd bytes on the input stream */
        return (f_lseek(fil, f_tell(fil) + nd) == FR_OK) ? nd : 0;
    }
}

UINT output_func ( JDEC *jd, void *bitmap, JRECT *rect )
{
uint16_t *bmp = (uint16_t *)bitmap;
uint16_t x, y, i = 0;

for( y = rect->top; y <= rect->bottom; y++)
{
  for(x = rect->left; x <= rect->right; x++)
    {
      uTFT_SetPixel(x, y, bmp [ i ++ ] );
    } 
} 

return 1;   // Continue to desompress
} // output_func

typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;

RGB_typedef *RGB_matrix;
uint8_t _aucLine[2048];
uint32_t line_counter = 0;
uint16_t Xpos = 0;
uint16_t Ypos = 0;

#define IMAGE_HEIGHT 220
#define IMAGE_WIDTH  176

static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);

void Jpeg(char * Name)
{     
	res = f_open(&JpegEncFile, Name, FA_READ);
  if (res != FR_OK)
	{
		
	}
  else
  {
    load_jpg(&JpegEncFile,jdwork,4096);
    f_close(&JpegEncFile);
  }	
}

void load_jpg (
    FIL *fp,        /* Pointer to the open file object to load */
    void *work,     /* Pointer to the working buffer (must be 4-byte aligned) */
    UINT sz_work    /* Size of the working buffer (must be power of 2) */
)
{
    JDEC jd;        /* Decoder object (124 bytes) */
    JRESULT rc;
    BYTE scale;
 
    /* Prepare to decompress the file */
    rc = jd_prepare(&jd, input_func, work, sz_work, fp);
    if (rc == JDR_OK) {
 
        /* Determine scale factor */
        for (scale = 0; scale < 3; scale++) {
            if ((jd.width >> scale) <= 176 && (jd.height >> scale) <= 220) break;
        }
 
        /* Display size information at bottom of screen */
//        sprintf(str1,"%ux%u 1/%u", jd.width, jd.height, 1 << scale);
//        HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
        /* Start to decompress the JPEG file */
        rc = jd_decomp(&jd, output_func, scale); /* Start to decompress */
 
    } else {
 
        /* Display error code */
//        sprintf(str1, "Error: %d\n", rc);  //error return code, furthur searching. JDR_MEM1
//		   	HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
    }
}