#ifndef TFT_CONVERT_H_
#define TFT_CONVERT_H_

#include <string.h>

extern void ConvertStringDosTo1251  ( char *str ); // Конвертирование строки из DOS(866) в Win1251
extern void ConvertString1251ToDos  ( char *str ); // Конвертирование строки из Win1251 в DOS(866)
extern void ConvertString1251ToUTF8 ( char *in_str, char *out_str );// Конвертирование строки из Win1251 в UTF8
extern int  ConvertStringUTF8to1251 ( const char* utf8, char* windows1251, size_t n);

#endif /* TFT_CONVERT_H_ */
