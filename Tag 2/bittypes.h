#ifndef _BITTYPES_H_INCLUDED
#define _BITTYPES_H_INCLUDED

typedef unsigned char byte_t;

typedef struct _byte
{
	byte_t bits;
} BYTE;

extern const byte_t BYTE__MAX;

byte_t valueOfBYTE(BYTE raw);
int BYTEfromValue(byte_t value, BYTE * raw);


typedef unsigned short int word_t;

typedef struct _word
{
	BYTE loByte;
	BYTE hiByte;
} WORD;

extern const word_t WORD__MAX;

word_t valueOfWORD(WORD raw);
int WORDfromValue(word_t value, WORD * raw);


typedef unsigned long int dword_t;

typedef struct _dword
{
	WORD loWord;
	WORD hiWord;
} DWORD;

extern const dword_t DWORD__MAX;

dword_t valueOfDWORD(DWORD raw);
int DWORDfromValue(dword_t value, DWORD * raw);


typedef long int long_t;

typedef struct _long
{
	WORD loWord;
	WORD hiWord;
} LONG;

extern const long_t LONG__MAX;
extern const long_t LONG__MIN;

long_t valueOfLONG(LONG raw);
int LONGfromValue(long_t value, LONG * raw);



extern const int CONV_OK;
extern const int CONV_VIOLATES_UPPER_BOUND;
extern const int CONV_VIOLATES_LOWER_BOUND;



#define byte(v) { v }
#define word(lo, hi) { byte(lo), byte(hi) }


extern const BYTE  bNull;
extern const WORD  wNull;
extern const DWORD dNull;
extern const LONG  lNull;


int eqBYTEs (const BYTE * b1, const BYTE * b2);
int eqWORDs (const WORD * w1, const WORD * w2);
int eqDWORDs(const DWORD * d1, const DWORD * d2);
int eqLONGs (const LONG * l1, const LONG * l2);


#endif /* _BITTYPES_H_INCLUDED */
