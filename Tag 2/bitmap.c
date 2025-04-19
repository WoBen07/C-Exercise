#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct _bmpFileHeader
{
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved_1;
	WORD  bfReserved_2;
	DWORD bfOffs;
	DWORD biSize;
} bmpFileHeader;

const WORD bfType = word('B', 'M');

const DWORD biSizeV3 = { word( 40, 0), word(0, 0) };
const DWORD biSizeV4 = { word(108, 0), word(0, 0) };
const DWORD biSizeV5 = { word(124, 0), word(0, 0) };

typedef struct _bitFields
{
	DWORD redChannel;
	DWORD greenChannel;
	DWORD blueChannel;
} bitFields;

typedef struct _RGBQuad
{
	pixelRGB colour;
	BYTE reserved;
} RGBQuad;

typedef struct _bmpInfoBlockV3
{
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCnt;
	DWORD biCompression;
	DWORD biSizeIm;
	DWORD biXPelsPM;
	DWORD biYPelsPM;
	DWORD biClrUsed;
	DWORD biClrImp;
} bmpInfoBlockV3;

const WORD  biPlanes = word(1, 0);

const WORD  bpp1  = word( 1, 0);
const WORD  bpp4  = word( 4, 0);
const WORD  bpp8  = word( 8, 0);
const WORD  bpp16 = word(16, 0);
const WORD  bpp24 = word(24, 0);
const WORD  bpp32 = word(32, 0);

const DWORD biRGB      = { word(0, 0), word(0, 0) };
const DWORD biRLE8     = { word(1, 0), word(0, 0) };
const DWORD biRLE4     = { word(2, 0), word(0, 0) };
const DWORD biBitfield = { word(3, 0), word(0, 0) };

typedef struct _bmpInfoBlockV4
{
	bmpInfoBlockV3 bi;
	bitFields      masks;

	DWORD alphaChannel;
	DWORD colourSpace;
	DWORD xEndRed;
	DWORD yEndRed;
	DWORD zEndRed;
	DWORD xEndGreen;
	DWORD yEndGreen;
	DWORD zEndGreen;
	DWORD xEndBlue;
	DWORD yEndBlue;
	DWORD zEndBlue;
	DWORD gammaRed;
	DWORD gammaGreen;
	DWORD gammeBlue;
} bmpInfoBlockV4;

typedef struct _bmpInfoBlockV5
{
	bmpInfoBlockV4 bi;

	DWORD intent;
	DWORD ICCdata;
	DWORD ICCsize;
	DWORD reserved;
} bmpInfoBlockV5;

#define failure(callReturn, call) ((callReturn = call) != BMP_OK)

typedef union _bmpInfoBlock
{
	bmpInfoBlockV3 bi;
	bmpInfoBlockV4 biV4;
	bmpInfoBlockV5 biV5;
} bmpInfoBlock;


static int loadBitmapFileHeader(FILE *, bmpFileHeader *);

static int loadBitmapInfoBlockV3(FILE *, bmpInfoBlockV3 *);
static int loadBitmapInfoBlockV4(FILE *, bmpInfoBlockV4 *);
static int loadBitmapInfoBlockV5(FILE *, bmpInfoBlockV5 *);

static int loadColourTable(FILE *, RGBQuad **, DWORD, WORD);

static int loadPixels(FILE *, bitmapRGB *, int,
                      bitFields *, RGBQuad *,
                      DWORD, WORD);

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) { free(table);\
                                free(bitmap->pixel);\
                              	bitmap->pixel = NULL;\
                                bitmap->width = 0;\
                                bitmap->height = 0;\
                                fclose(bmp);\
                                return ret; }

int loadBitmapRGB(const char * filename,
                  bitmapRGB * bitmap)
{
	FILE * bmp;

	bmpFileHeader bm;
	bmpInfoBlock  bi;
	bitFields *   masks = &(bi.biV4.masks);
	int           bottomUp = 0;
	dword_t       pixel;
	RGBQuad *     table = NULL;

	int ret_val;

	bitmap->width  = 0;
	bitmap->height = 0;
	bitmap->pixel  = NULL;

	bmp = fopen(filename, "rb");
	if (!bmp) return BMP_FILE_ERROR;

 	if failure(ret_val, loadBitmapFileHeader(bmp, &bm))
		cleanUpAndReturn(ret_val);

	if (eqDWORDs(&(bm.biSize), &(biSizeV3)) == 0)
	{
		if failure(ret_val, loadBitmapInfoBlockV3(bmp, &(bi.bi)))
			cleanUpAndReturn(ret_val);

		if (eqDWORDs(&(bi.bi.biCompression), &biBitfield) == 0)
		{
			if (fread(masks, sizeof(bitFields), 1, bmp) != 1)
				cleanUpAndReturn(BMP_READ_ERROR);
		}
	}
	else if (eqDWORDs(&(bm.biSize), &(biSizeV4)) == 0)
	{
		if failure(ret_val, loadBitmapInfoBlockV4(bmp, &(bi.biV4)))
			cleanUpAndReturn(ret_val);
	}
	else if (eqDWORDs(&(bm.biSize), &(biSizeV5)) == 0)
	{
		if failure(ret_val, loadBitmapInfoBlockV5(bmp, &(bi.biV5)))
			cleanUpAndReturn(ret_val);
	}
	else cleanUpAndReturn(BMP_FORMAT_NOT_IMPLEMENTED);

	bitmap->width  = valueOfLONG(bi.bi.biWidth);
	bitmap->height = valueOfLONG(bi.bi.biHeight);

	if (bitmap->height < 0)
	{
		bitmap->height = -(bitmap->height);
		bottomUp = 1;
	}

	pixel = bitmap->height * bitmap->width;
	bitmap->pixel = (pixelRGB *) malloc(sizeof(pixelRGB) * pixel);
	if (!(bitmap->pixel))
		cleanUpAndReturn(BMP_MEMORY_ERROR);

	if failure(ret_val, loadColourTable(bmp, &table,
	                                    bi.bi.biClrUsed,
	                                    bi.bi.biBitCnt))
		cleanUpAndReturn(ret_val);

	if (fseek(bmp, valueOfDWORD(bm.bfOffs), SEEK_SET) != 0)
		cleanUpAndReturn(BMP_FILE_ERROR);

	if failure(ret_val, loadPixels(bmp, bitmap, bottomUp,
	                               masks, table,
	                               bi.bi.biCompression,
	                               bi.bi.biBitCnt))
		cleanUpAndReturn(ret_val);

	if (table) free(table);

	fclose(bmp);

	return BMP_OK;
}

int loadBitmapFileHeader(FILE * bmp, bmpFileHeader * bm)
{
	if (fread(bm, sizeof(bmpFileHeader), 1, bmp) != 1)
		return BMP_READ_ERROR;

	if (eqWORDs(&(bm->bfType), &bfType) != 0)
		return BMP_FORMAT_ERROR;

	if (eqWORDs(&(bm->bfReserved_1), &wNull) != 0)
		return BMP_FORMAT_ERROR;

	if (eqWORDs(&(bm->bfReserved_2), &wNull) != 0)
		return BMP_FORMAT_ERROR;

	return BMP_OK;
}

int loadBitmapInfoBlockV3(FILE * bmp, bmpInfoBlockV3 * bi)
{
	if (fread(bi, sizeof(bmpInfoBlockV3), 1, bmp) != 1)
		return BMP_READ_ERROR;

	if (eqWORDs(&(bi->biPlanes), &biPlanes) != 0)
		return BMP_FORMAT_ERROR;

	return BMP_OK;
}

int loadBitmapInfoBlockV4(FILE * bmp, bmpInfoBlockV4 * bi)
{
	if (fread(bi, sizeof(bmpInfoBlockV4), 1, bmp) != 1)
		return BMP_READ_ERROR;

	if (eqWORDs(&(bi->bi.biPlanes), &biPlanes) != 0)
		return BMP_FORMAT_ERROR;

	return BMP_OK;
}

int loadBitmapInfoBlockV5(FILE * bmp, bmpInfoBlockV5 * bi)
{
	if (fread(bi, sizeof(bmpInfoBlockV5), 1, bmp) != 1)
		return BMP_READ_ERROR;

	if (eqWORDs(&(bi->bi.bi.biPlanes), &biPlanes) != 0)
		return BMP_FORMAT_ERROR;

	return BMP_OK;
}

int loadColourTable(FILE * bmp, RGBQuad ** table, DWORD biClrUsed, WORD biBitCnt)
{
	dword_t entries = valueOfDWORD(biClrUsed);

	if (entries == 0)
	{
		if (eqWORDs(&biBitCnt, &bpp1) == 0) entries =   2;
		if (eqWORDs(&biBitCnt, &bpp4) == 0) entries =  16;
		if (eqWORDs(&biBitCnt, &bpp8) == 0) entries = 256;
	}

	if (entries == 0)
		return BMP_OK;

	if (!(*table = malloc(sizeof(RGBQuad) * entries)))
		return BMP_MEMORY_ERROR;

	if (fread(*table, sizeof(RGBQuad), entries, bmp) != entries)
	{
		free(*table);
		*table = NULL;
		return BMP_READ_ERROR;
	}

	return BMP_OK;
}

static int loadTablePixels(FILE *, bitmapRGB *, int, RGBQuad *, WORD);
static int loadBytePixels(FILE *, bitmapRGB *, int);
static int loadMaskPixels(FILE *, bitmapRGB *, int, bitFields *, WORD);

int loadPixels(FILE * bmp, bitmapRGB * bitmap, int bottomUp,
               bitFields * masks, RGBQuad * table,
               DWORD biCompression, WORD biBitCnt)
{
	if (eqDWORDs(&biCompression, &biRLE4) == 0)
		return BMP_FORMAT_NOT_IMPLEMENTED;

	if (eqDWORDs(&biCompression, &biRLE8) == 0)
		return BMP_FORMAT_NOT_IMPLEMENTED;

	if (eqDWORDs(&biCompression, &biRGB) == 0)
	{
		if (eqWORDs(&biBitCnt,  &bpp1) == 0)
			return loadTablePixels(bmp, bitmap, bottomUp, table, biBitCnt);
		if (eqWORDs(&biBitCnt,  &bpp4) == 0)
			return loadTablePixels(bmp, bitmap, bottomUp, table, biBitCnt);
		if (eqWORDs(&biBitCnt,  &bpp8) == 0)
			return loadTablePixels(bmp, bitmap, bottomUp, table, biBitCnt);
		if (eqWORDs(&biBitCnt, &bpp16) == 0)
		{
			DWORDfromValue(0x00007C00, &(masks->redChannel));
			DWORDfromValue(0x000003E0, &(masks->greenChannel));
			DWORDfromValue(0x0000001F, &(masks->blueChannel));
			DWORDfromValue(valueOfDWORD(biBitfield), &biCompression);
		}
		if (eqWORDs(&biBitCnt, &bpp24) == 0)
			return loadBytePixels(bmp, bitmap, bottomUp);
		if (eqWORDs(&biBitCnt, &bpp32) == 0)
		{
			DWORDfromValue(0x00FF0000, &(masks->redChannel));
			DWORDfromValue(0x0000FF00, &(masks->greenChannel));
			DWORDfromValue(0x000000FF, &(masks->blueChannel));
			DWORDfromValue(valueOfDWORD(biBitfield), &biCompression);
		}
	}

	if (eqDWORDs(&biCompression, &biBitfield) == 0)
	{
		if (eqWORDs(&biBitCnt, &bpp16) == 0 ||
		    eqWORDs(&biBitCnt, &bpp32) == 0)
			return loadMaskPixels(bmp, bitmap, bottomUp, masks, biBitCnt);
	}

	return BMP_FORMAT_ERROR;
}

#define establishBottomUpLoop begin = 0;\
                              step = 1;\
                              end = bitmap->height;

#define establishTopDownLoop  begin = bitmap->height - 1;\
                              step = -1;\
                              end = -1;

#define establishLoop if (bottomUp)\
                      {\
                      	establishBottomUpLoop;\
                      }\
                      else\
                      {\
                      	establishTopDownLoop;\
                      }

#define forEveryLine for (i = begin; i != end; i += step)

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) return ret;

#define readLine(type) if (fread(line, sizeof(type), width, bmp) != width)\
                       	cleanUpAndReturn(BMP_READ_ERROR);
#define skipFill if (fseek(bmp, fill, SEEK_CUR) != 0)\
                 	cleanUpAndReturn(BMP_FILE_ERROR);

int loadBytePixels(FILE * bmp, bitmapRGB * bitmap,
                   int bottomUp)
{
	dword_t width, fill;
	dword_t i, step, begin, end;
	pixelRGB * line;

	width = bitmap->width;
	fill = (4 - (width * 3 % 4)) % 4;

	establishLoop;

	forEveryLine
	{
		line = &(bitmap->pixel[i*width]);

		readLine(pixelRGB);
		skipFill;
	}

	return BMP_OK;
}

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) { free(line);\
                                return ret; }

#include <math.h>

int loadTablePixels(FILE * bmp, bitmapRGB * bitmap,
                    int bottomUp, RGBQuad * table,
                    WORD biBitCnt)
{
	dword_t width, fill;
	dword_t i, step, begin, end;
	BYTE * line;

	long_t j;
	word_t bpp, factor, top;
	byte_t index, mask;

	if (eqWORDs(&biBitCnt, &bpp1) == 0) mask = 0x01;
	if (eqWORDs(&biBitCnt, &bpp4) == 0) mask = 0x0F;
	if (eqWORDs(&biBitCnt, &bpp8) == 0) mask = 0xFF;

	width = ceil(bitmap->width * (valueOfWORD(biBitCnt) / 8.0));
	fill = (4 - (width % 4)) % 4;

	line = (BYTE *) malloc(width * sizeof(BYTE));
	if (!line) return BMP_MEMORY_ERROR;

	bpp = valueOfWORD(biBitCnt);
	factor =  8 / bpp;
	top = 8 - bpp;

	establishLoop;

	forEveryLine
	{
		readLine(BYTE);
		skipFill;

		for (j = 0; j < bitmap->width; ++j)
		{
			index = valueOfBYTE(line[j / factor]);
			index >>= top - (j % factor) * bpp;
			index &= mask;

			bitmap->pixel[i * bitmap->width + j] = table[index].colour;
		}
	}

	free(line);
	return BMP_OK;
}

typedef union __pixel
{
	BYTE  * b8;
	WORD  * b16;
	DWORD * b32;
} _pixel;

#define computeShiftAndRatioFor(clr) i = clr ## Mask = valueOfDWORD(masks->clr ## Channel);\
                                     while ((i & 0x00000001) == 0)\
                                     {\
                                     	++ clr ## Shift;\
                                     	i >>= 1;\
                                     }\
                                     clr ## Ratio = BYTE__MAX / (double) i;

#undef copyChannel
#define copyChannel(clr) bitmap->pixel[i*bitmap->width + j].clr =\
                         ((value & clr ## Mask) >> clr ## Shift) * clr ## Ratio;

int loadMaskPixels(FILE * bmp, bitmapRGB * bitmap,
                   int bottomUp, bitFields * masks,
                   WORD biBitCnt)
{
	dword_t width, fill;
	dword_t i, step, begin, end;
	BYTE * line;

	_pixel pixel;
	dword_t value;
	long_t j;

	dword_t redMask, greenMask, blueMask;
	byte_t redShift = 0, greenShift = 0, blueShift = 0;
	double redRatio, greenRatio, blueRatio;

	width = bitmap->width * (valueOfWORD(biBitCnt) / 8);
	fill = (4 - (width % 4)) % 4;

	line = (BYTE *) malloc(width * sizeof(BYTE));
	if (!line) return BMP_MEMORY_ERROR;

	computeShiftAndRatioFor(red);
	computeShiftAndRatioFor(green);
	computeShiftAndRatioFor(blue);

	establishLoop;

	forEveryLine
	{
		readLine(BYTE)
		skipFill;

		pixel.b8 = line;

		for (j = 0; j < bitmap->width; ++j)
		{
			if (eqWORDs(&biBitCnt, &bpp16) == 0)
			{
				value = valueOfWORD(*(pixel.b16));
				++(pixel.b16);
			}
			else
			{
				value = valueOfDWORD(*(pixel.b32));
				++(pixel.b32);
			}
			copyChannel(red);
			copyChannel(green);
			copyChannel(blue);
		}
	}

	free(line);
	return BMP_OK;
}

static int saveBitmapRGBFileHeader(FILE *, const bitmapRGB *);
static int saveBitmapRGBInfoBlock(FILE *, const bitmapRGB*);
static int saveBytePixels(FILE *, const bitmapRGB *);
#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) { fclose(bmp);\
                                return ret; }

int saveBitmapRGB(const char * filename, const bitmapRGB * bitmap)
{
	FILE * bmp;
	int ret_val;

	bmp = fopen(filename, "wb");
	if (!bmp) return BMP_FILE_ERROR;

	if failure(ret_val, saveBitmapRGBFileHeader(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	if failure(ret_val, saveBitmapRGBInfoBlock(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	if failure(ret_val, saveBytePixels(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	fclose(bmp);

	return BMP_OK;
}

static int saveBitmapFileHeader(FILE * bmp, bmpFileHeader * bm)
{
	bm->bfType = bfType;
	bm->bfReserved_1 = wNull;
	bm->bfReserved_2 = wNull;
	bm->biSize = biSizeV3;

	if (fwrite(bm, sizeof(bmpFileHeader), 1, bmp) != 1)
		return BMP_WRITE_ERROR;

	return BMP_OK;
}

int saveBitmapRGBFileHeader(FILE * bmp, const bitmapRGB * bitmap)
{
	bmpFileHeader bm;

	dword_t rowSize = (24 * bitmap->width + 31) / 32 * 4;

	DWORDfromValue(bitmap->height * rowSize +
	               sizeof(bmpFileHeader) +
	               sizeof(bmpInfoBlockV3), &(bm.bfSize));
	DWORDfromValue(sizeof(bmpFileHeader) +
	               sizeof(bmpInfoBlockV3), &(bm.bfOffs));

	return saveBitmapFileHeader(bmp, &bm);
}

static int saveBitmapInfoBlock(FILE * bmp, bmpInfoBlockV3 * bi)
{
	bi->biPlanes = biPlanes;
	bi->biSizeIm = dNull;
	bi->biXPelsPM = dNull;
	bi->biYPelsPM = dNull;
	bi->biClrUsed = dNull;
	bi->biClrImp = dNull;

	if (fwrite(bi, sizeof(bmpInfoBlockV3), 1, bmp) != 1)
		return BMP_WRITE_ERROR;

	return BMP_OK;
}

int saveBitmapRGBInfoBlock(FILE * bmp, const bitmapRGB * bitmap)
{
	bmpInfoBlockV3 bi;

	LONGfromValue(bitmap->width, &(bi.biWidth));
	LONGfromValue(bitmap->height, &(bi.biHeight));
	bi.biBitCnt = bpp24;
	bi.biCompression = biRGB;

	return saveBitmapInfoBlock(bmp, &bi);
}

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) return ret;

#define writeLine(type) if (fwrite(line, sizeof(type), width, bmp) != width)\
                        	cleanUpAndReturn(BMP_WRITE_ERROR);

#define writeFill if (fwrite(&dNull, sizeof(BYTE), fill, bmp) != fill)\
                  	cleanUpAndReturn(BMP_WRITE_ERROR);

int saveBytePixels(FILE * bmp, const bitmapRGB * bitmap)
{
	dword_t width, fill;
	dword_t i, step, begin, end;
	pixelRGB * line;

	width = bitmap->width;
	fill = (4 - (width * 3 % 4)) % 4;

	establishTopDownLoop;

	forEveryLine
	{
		line = &(bitmap->pixel[i * width]);

		writeLine(pixelRGB);
		writeFill;
	}

	return BMP_OK;
}

static int saveBitmapGrayFileHeader(FILE *, const bitmapGray *);
static int saveBitmapGrayInfoBlock(FILE *, const bitmapGray *);
static int saveTablePixels(FILE *, const bitmapGray *);

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) { fclose(bmp);\
                                return ret; }

int saveBitmapGray(const char * filename, const bitmapGray * bitmap)
{
	FILE * bmp;
	int ret_val;

	bmp = fopen(filename, "wb");
	if (!bmp) return BMP_FILE_ERROR;

	if failure(ret_val, saveBitmapGrayFileHeader(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	if failure(ret_val, saveBitmapGrayInfoBlock(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	if failure(ret_val, saveTablePixels(bmp, bitmap))
		cleanUpAndReturn(ret_val);

	fclose(bmp);

	return BMP_OK;
}

int saveBitmapGrayFileHeader(FILE * bmp, const bitmapGray * bitmap)
{
	bmpFileHeader bm;

	dword_t rowSize = (8 * bitmap->width + 31) / 32 * 4;

	DWORDfromValue(bitmap->height * rowSize + 
	               sizeof(bmpFileHeader) +
	               sizeof(bmpInfoBlockV3) +
	               256 * sizeof(RGBQuad), &(bm.bfSize));
	DWORDfromValue(sizeof(bmpFileHeader) +
	               sizeof(bmpInfoBlockV3) +
	               256 * sizeof(RGBQuad), &(bm.bfOffs));

	return saveBitmapFileHeader(bmp, &bm);
}

int saveBitmapGrayInfoBlock(FILE * bmp, const bitmapGray * bitmap)
{
	bmpInfoBlockV3 bi;

	LONGfromValue(bitmap->width, &(bi.biWidth));
	LONGfromValue(bitmap->height, &(bi.biHeight));
	bi.biBitCnt = bpp8;
	bi.biCompression = biRGB;

	return saveBitmapInfoBlock(bmp, &bi);
}

static int createGrayTable(RGBQuad **);

#undef cleanUpAndReturn
#define cleanUpAndReturn(ret) { free(table);\
                                return ret; }

int saveTablePixels(FILE * bmp, const bitmapGray * bitmap)
{
	dword_t width, fill;
	dword_t i, step, begin, end;
	pixelGray * line;

	RGBQuad * table = NULL;
	int ret_val;

	if failure(ret_val, createGrayTable(&table))
		cleanUpAndReturn(ret_val);

	if (fwrite(table, sizeof(RGBQuad), 256, bmp) != 256)
		cleanUpAndReturn(BMP_WRITE_ERROR);

	width = bitmap->width;
	fill = (4 - (width % 4)) % 4;

	establishTopDownLoop;

	forEveryLine
	{
		line = &(bitmap->pixel[i * width]);

		writeLine(pixelGray);
		writeFill;
	}
	
	free(table);

	return BMP_OK;
}

#undef copyChannel
#define copyChannel(clr) (*table)[luminance].colour.clr = luminance;
#define setNull (*table)[luminance].reserved = bNull;

int createGrayTable(RGBQuad ** table)
{
	byte_t luminance;

	*table = (RGBQuad *) malloc(256 * sizeof(RGBQuad));
	if (*table == NULL)
		return BMP_MEMORY_ERROR;

	luminance = 0;
	copyChannel(red);
	copyChannel(green);
	copyChannel(blue);
	setNull;
	do
	{
		++luminance;
		copyChannel(red);
		copyChannel(green);
		copyChannel(blue);
		setNull;
	} while (luminance < 255);

	return BMP_OK;
}


const int BMP_OK = 0;

const int BMP_MEMORY_ERROR = 1;

const int BMP_READ_ERROR = 2;
const int BMP_WRITE_ERROR = 4;
const int BMP_FILE_ERROR = 8;

const int BMP_FORMAT_NOT_IMPLEMENTED = 16;
const int BMP_FORMAT_ERROR = 32;
