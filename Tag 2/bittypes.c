#include <limits.h>

#if (CHAR_BIT != 8)
#error No 8 Bit unsigned char!
#endif

#if (LONG_MIN > -2147483648)
#error long int cannot represent -2147483648!
#endif


#include "bittypes.h"

const int CONV_OK = 0;
const int CONV_VIOLATES_LOWER_BOUND = -1;
const int CONV_VIOLATES_UPPER_BOUND =  1;


const byte_t BYTE__MAX = 255;

byte_t valueOfBYTE(BYTE raw)
{
	return raw.bits;
}

int BYTEfromValue(byte_t value, BYTE * raw)
{
	raw->bits = value;

	return CONV_OK;
}


const word_t WORD__MAX = 65535;

#define hiByteFactor ((word_t) BYTE__MAX + 1)

word_t valueOfWORD(WORD raw)
{
	return   (word_t) valueOfBYTE(raw.hiByte) * hiByteFactor
	       + (word_t) valueOfBYTE(raw.loByte);
}

int WORDfromValue(word_t value, WORD * raw)
{
	if (value > WORD__MAX) return CONV_VIOLATES_UPPER_BOUND;

	BYTEfromValue(value % hiByteFactor, &(raw->loByte));
	BYTEfromValue(value / hiByteFactor, &(raw->hiByte));

	return CONV_OK;
}


const dword_t DWORD__MAX = 4294967295Lu;

#define hiWordFactor ((dword_t) WORD__MAX + 1)

dword_t valueOfDWORD(DWORD raw)
{
	return   (dword_t) valueOfWORD(raw.hiWord) * hiWordFactor
	       + (dword_t) valueOfWORD(raw.loWord);
}

int DWORDfromValue(dword_t value, DWORD * raw)
{
	if (value > DWORD__MAX) return CONV_VIOLATES_UPPER_BOUND;

	WORDfromValue(value % hiWordFactor, &(raw->loWord));
	WORDfromValue(value / hiWordFactor, &(raw->hiWord));

	return CONV_OK;
}


const long_t LONG__MAX =  2147483647;
const long_t LONG__MIN = -2147483648Lu;

long_t valueOfLONG(LONG raw)
{
	dword_t value = valueOfDWORD(*(DWORD *) &raw);

	if (valueOfBYTE(raw.hiWord.hiByte) & 128)
		return (long_t) (value - (dword_t) LONG__MAX - 1);
	else
		return (long_t) value;
}

int LONGfromValue(long_t value, LONG * raw)
{
	if (value > LONG__MAX) return CONV_VIOLATES_UPPER_BOUND;
	if (value < LONG__MIN) return CONV_VIOLATES_LOWER_BOUND;

	if (value >= 0)
		DWORDfromValue((dword_t) value,
		               (DWORD *) raw);
	else
		DWORDfromValue((dword_t) LONG__MAX + (dword_t) (-value) + 1,
		               (DWORD *) raw);

	return CONV_OK;
}


const BYTE  bNull = byte(0);
const WORD  wNull = word(0, 0);
const DWORD dNull = { word(0, 0), word(0, 0) };
const LONG  lNull = { word(0, 0), word(0, 0) };


#include <string.h>

int eqBYTEs(const BYTE * b1, const BYTE * b2)
{
	return memcmp(b1, b2, sizeof(BYTE));
}

int eqWORDs(const WORD * w1, const WORD * w2)
{
	return memcmp(w1, w2, sizeof(WORD));
}

int eqDWORDs(const DWORD * d1, const DWORD * d2)
{
	return memcmp(d1, d2, sizeof(DWORD));
}

int eqLONGs(const LONG * l1, const LONG * l2)
{
	return memcmp(l1, l2, sizeof(LONG));
}

