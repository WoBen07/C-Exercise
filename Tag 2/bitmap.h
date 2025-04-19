#ifndef _BITMAP_H_INCLUDED
#define _BITMAP_H_INCLUDED

#include "bittypes.h"

typedef struct _pixelRGB {
	byte_t blue;
	byte_t green;
	byte_t red;
} pixelRGB;

typedef struct _bitmapRGB {
	long_t width;
	long_t height;
	pixelRGB *pixel;
} bitmapRGB;

int loadBitmapRGB(const char *filename, bitmapRGB *bitmap);

int saveBitmapRGB(const char *filename, const bitmapRGB *bitmap);


typedef struct _pixelGray {
	byte_t luminance;
} pixelGray;

typedef struct _bitmapGray {
	long_t width;
	long_t height;
	pixelGray *pixel;
} bitmapGray;

int saveBitmapGray(const char *filename, const bitmapGray *bitmap);



extern const int BMP_OK;

extern const int BMP_MEMORY_ERROR;

extern const int BMP_READ_ERROR;
extern const int BMP_WRITE_ERROR;
extern const int BMP_FILE_ERROR;

extern const int BMP_FORMAT_NOT_IMPLEMENTED;
extern const int BMP_FORMAT_ERROR;

#endif /* _BITMAP_H_INCLUDED */
