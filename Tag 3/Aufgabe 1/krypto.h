#ifndef KRYPTO_H_INCLUDED
#define KRYPTO_H_INCLUDED

#define LEN 26

/* Einlesefunktionen */
int read_letter();
void read_key_viginere(char *key, int keylen);
int read_len();

/* Viginere-Schluessel generieren */
void init_key(char *key, int keylen);
int is_end_key(char *key);
void increment_key(char *key);

/* Hilfsfunktionen */
int is_text_upper_case(const char *text);
void generate_subcode(const char *code, char *subcode, int keylen, int keypos);
int ggT(int a, int b);
int is_array_positive(int array[], int len);
int ggT_array(int array[], int len);

/* Verschuesseln */
void caesar_encode(char key, char *code, const char *text, int len);
void viginere_encode(const char *key, char *code, const char *text, int len);

/* Entschuesseln */
void caesar_decode(char key, const char *code, char *text, int len);
void viginere_decode(const char *key, const char *code, char *text, int len);

/* Brute-Force-Attacke */
void brute_caesar(const char *code, int len_decode);
void brute_viginere(const char *code, int keylen, int len_decode);

/* Haeufigkeitsanalyse */
void freq_table(int count[], const char *code);
void print_table(int count[]);
char freq_analysis_caesar(char max);
int find_distances(const char *code, int distances[], int sublen);
void collect_distances(int distances[], int len, int collect[]);

/*Eigene*/
int ceasar_decode_char(char c, char key);
void flush_stdin(void);
#endif
