#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#pragma region Dateizugriff

#define BUFFER_SIZE 1024

#define SUCCESS 0
#define FOPEN_ERROR 1
#define FCLOSE_ERRROR 2
#define READ_ERROR 4
#define WRITE_ERROR 8
#define FSEEK_ERROR 16
#define FTELL_ERROR 32
#define MALLOC_ERROR 128

int bin_copy(const char* input_filename, const char* output_filename)
{
    int status;
    size_t bytes_read;
    char buffer[BUFFER_SIZE];
    FILE* input;
    FILE* output;
    input = fopen(input_filename, "rb");
    if (input == NULL) {
        return FOPEN_ERROR;
    }
    output = fopen(output_filename, "wb");
    if (output == NULL) {
        fclose(input);
        return FOPEN_ERROR;
    }
    while (!feof(input)) {
        bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, input);
        if (bytes_read < BUFFER_SIZE && !feof(input)) {
            fclose(input);
            fclose(output);
            return READ_ERROR;
        }
        if (fwrite(buffer, sizeof(char), bytes_read, output) < bytes_read) {
            fclose(input);
            fclose(output);
            return WRITE_ERROR;
        }
    }
    status = fclose(input);
    if (status == EOF) {
        fclose(output);
        return FCLOSE_ERRROR;
    }
    status = fclose(output);
    if (status == EOF) {
        return FCLOSE_ERRROR;
    }
    return SUCCESS;
}

size_t calc_file_size_seek(const char* filename, int* error)
{
    int status;
    size_t size;
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        *error = FOPEN_ERROR;
        return -1;
    }
    status = fseek(file, 0, SEEK_END);
    if (status != 0) {
        *error = FSEEK_ERROR;
        fclose(file);
        return -1;
    }
    size = ftell(file);
    status = fclose(file);
    if (status == EOF) {
        *error = FCLOSE_ERRROR;
        return -1;
    }
    if (size == -1) {
        *error = FTELL_ERROR;
    } else {
        *error = SUCCESS;
    }
    return size;
}

char* line_from_file(FILE* input, int* status)
{
    int c, size = 1;
    char* temp;
    char* line = malloc(sizeof(char));
    if (line == NULL) {
        *status = MALLOC_ERROR;
        return NULL;
    }
    while ((c = getc(input)) != '\n') {
        if (c == EOF && !feof(input)) {
            free(line);
            *status = READ_ERROR;
            return NULL;
        }
        if (feof(input)) break;
        line[size - 1] = c;
        temp = realloc(line, ++size * sizeof(char));
        if (temp == NULL) {
            free(line);
            *status = MALLOC_ERROR;
            return NULL;
        }
        line = temp;
    }
    line[size - 1] = '\0';
    *status = SUCCESS;
    return line;
}

int count_string(const char* filename, const char* search, int* error)
{
    int count = 0, status;
    char* line;
    char* pos;
    FILE* input;
    input = fopen(filename, "r");
    if (input == NULL) {
        *error = FOPEN_ERROR;
        return -1;
    }

    line = line_from_file(input, &status);
    while (status == SUCCESS) {
        pos = line;
        while ((pos = strstr(pos, search)) != NULL) {
            count++;
            pos++;
        }
        free(line);
        if (feof(input)) break;
        line = line_from_file(input, &status);
    }
    if (status != SUCCESS) {
        *error = status;
        return -1;
    }
    status = fclose(input);
    if (status == EOF) {
        *error = FCLOSE_ERRROR;
        return -1;
    }
    *error = SUCCESS;
    return count;
}

int count_string_alt(const char* filename, const char* search, int* error)
{
    int count = 0, size = 1, status, c;
    char* text;
    char* pos;
    FILE* input;
    input = fopen(filename, "r");
    if (input == NULL) {
        *error = FOPEN_ERROR;
        return -1;
    }
    text = malloc(size * sizeof(char));
    if (text == NULL) {
        fclose(input);
        *error = MALLOC_ERROR;
        return -1;
    }
    while ((c = getc(input)) != EOF) {
        text[size - 1] = c;
        pos = realloc(text, ++size * sizeof(char));
        if (pos == NULL) {
            *error = MALLOC_ERROR;
            free(text);
            fclose(input);
            return -1;
        }
        text = pos;
    }
    if (!feof(input)) {
        free(text);
        fclose(input);
        *error = READ_ERROR;
        return -1;
    }
    text[size - 1] = '\0';
    pos = text;
    while ((pos = strstr(pos, search)) != NULL) {
        count++;
        pos++;
    }

    status = fclose(input);
    if (status == EOF) {
        free(text);
        *error = FCLOSE_ERRROR;
        return -1;
    }
    free(text);
    *error = SUCCESS;
    return count;
}

int delete_char(const char* filename, const char* copy_name, const char del)
{
    int c, status;
    FILE* input;
    FILE* output;
    input = fopen(filename, "r");
    if (input == NULL) {
        return FOPEN_ERROR;
    }
    output = fopen(copy_name, "w");
    if (output == NULL) {
        fclose(input);
        return FOPEN_ERROR;
    }

    while ((c = getc(input)) != EOF) {
        if (c == del) continue;
        status = fputc(c, output);
        if (status == EOF) {
            fclose(input);
            fclose(output);
            return WRITE_ERROR;
        }
    }

    if (!feof(input)) {
        fclose(input);
        fclose(output);
        return READ_ERROR;
    }

    status = fclose(input);
    if (status == EOF) {
        fclose(output);
        return FCLOSE_ERRROR;
    }
    status = fclose(output);
    if (status == EOF) {
        return FCLOSE_ERRROR;
    }
    return SUCCESS;
}

int delete_string(const char* filename, const char* copy_name, const char* del)
{
    int size = 1, status, c;
    char* text;
    char* pos;
    char* result;
    FILE* input;
    FILE* output;
    input = fopen(filename, "r");
    if (input == NULL) {
        return FOPEN_ERROR;
    }
    text = malloc(size * sizeof(char));
    if (text == NULL) {
        fclose(input);
        return MALLOC_ERROR;
    }
    while ((c = getc(input)) != EOF) {
        text[size - 1] = c;
        pos = realloc(text, ++size * sizeof(char));
        if (pos == NULL) {
            free(text);
            fclose(input);
            return MALLOC_ERROR;
        }
        text = pos;
    }
    if (!feof(input)) {
        free(text);
        fclose(input);
        return READ_ERROR;
    }
    text[size - 1] = '\0';

    status = fclose(input);
    if (status == EOF) {
        free(text);
        return FCLOSE_ERRROR;
    }

    output = fopen(copy_name, "w");
    if (output == NULL) {
        free(text);
        return FOPEN_ERROR;
    }

    pos = text;
    result = strstr(pos, del);
    if (pos == result) {
        pos += strlen(del);
        result = strstr(pos, del);
    }
    while (*pos != '\0') {
        status = fputc(*pos, output);
        if (status == EOF) {
            free(text);
            fclose(output);
            return WRITE_ERROR;
        }
        pos++;
        while (pos == result) {
            pos += strlen(del);
            result = strstr(pos, del);
        }
    }
    free(text);
    status = fclose(output);
    if (status == EOF) {
        return FCLOSE_ERRROR;
    }
    return SUCCESS;
}

int replace_string(const char* filename, const char* copy_name, const char* search, const char* replacement)
{
    int status, c;
    char* text;
    char* temp;
    char* pos;
    char* search_result;
    int size = 2;
    int actual_size = 0;
    int search_len = (int)strlen(search);
    FILE* f;
    f = fopen(filename, "r");
    if (f == NULL) return FOPEN_ERROR;
    text = malloc(size * sizeof(char));
    if (text == NULL) {
        fclose(f);
        return MALLOC_ERROR;
    }
    while (1) {
        if (actual_size == size) {
            size *= 2;
            temp = realloc(text, size * sizeof(char));
            if (temp == NULL) {
                free(text);
                fclose(f);
                return MALLOC_ERROR;
            }
            text = temp;
        }
        c = fgetc(f);
        if (c == EOF) break;
        text[actual_size++] = c;
    }
    if (size != actual_size + 1) {
        size = actual_size + 1;
        temp = realloc(text, size * sizeof(char));
        if (temp == NULL) {
            free(text);
            fclose(f);
            return MALLOC_ERROR;
        }
    }
    text[actual_size] = '\0';
    if (!feof(f)) {
        free(text);
        fclose(f);
        return READ_ERROR;
    }
    status = fclose(f);
    if (status == EOF) {
        free(text);
        return FCLOSE_ERRROR;
    }
    f = fopen(copy_name, "w");
    if (f == NULL) {
        free(text);
        return FOPEN_ERROR;
    }
    search_result = strstr(text, search);
    pos = text;
    for (pos = text; *pos != '\0'; pos++) {
        while (pos == search_result) {
            status = fputs(replacement, f);
            if (status == EOF) {
                free(text);
                fclose(f);
                return WRITE_ERROR;
            }
            pos += search_len;
            search_result = strstr(pos, search);
        }
        if (*pos == '\0') break;
        status = fputc(*pos, f);
        if (status == EOF) {
            free(text);
            fclose(f);
            return WRITE_ERROR;
        }
    }
    free(text);
    status = fclose(f);
    if (status == EOF) {
        return FCLOSE_ERRROR;
    }
    return SUCCESS;
}

#pragma endregion

#pragma region Bildbearbeitung

int convert_to_gray(const bitmapRGB* input, bitmapGray *output)
{
    long_t height = input->height, width = input->width;
    long_t i;
    output->height = height;
    output->width = width;
    output->pixel = malloc(height * width * sizeof(pixelGray));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height * width; i++) {
        output->pixel[i].luminance = convert_luminance(&input->pixel[i]);
    }
    return SUCCESS;
}

/*set 2 to 0 and 1 to 1 to convert image into Colorscale*/
int scale_colors_luminance(const bitmapRGB* input, bitmapRGB* output, float r, float g, float b)
{
    long_t height = input->height, width = input->width;
    long_t i;
    byte_t luminance;
    output->height = height;
    output->width = width;
    output->pixel = malloc(width * height * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height * width; i++) {
        luminance = convert_luminance(&input->pixel[i]);
        output->pixel[i].red = r * luminance;
        output->pixel[i].green = g * luminance;
        output->pixel[i].blue = b * luminance;
    }
    return SUCCESS;
}

int invert_colors(const bitmapRGB* input, bitmapRGB* output)
{
    long_t height = input->height, width = input->width;
    long_t i;
    output->height = height;
    output->width = width;
    output->pixel = malloc(width * height * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height * width; i++) {
        output->pixel[i].red = 255 - input->pixel[i].red;
        output->pixel[i].green = 255 - input->pixel[i].green;
        output->pixel[i].blue = 255 - input->pixel[i].blue;
    }
    return 0;
}

int rotate_90_counter(const bitmapRGB* input, bitmapRGB* output)
{
    long_t oh = input->height, ow = input->width;
    long_t i, j;
    output->height = ow;
    output->width = oh;
    output->pixel = malloc(oh * ow * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < oh; i++) {
        for (j = 0; j < ow; j++) {
            output->pixel[j * oh + i] = input->pixel[i * ow + ow - j];
        }
    }
    return SUCCESS;
}

int mirror_vertical(const bitmapRGB* input, bitmapRGB* output)
{
    long_t height = input->height, width = input->width;
    long_t i, j;
    output->height = height;
    output->width = width;
    output->pixel = malloc(height * width * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            output->pixel[i * width + j] = input->pixel[i * width + width - j];
        }
    }
    return SUCCESS;
}

int mirror_horizontal(const bitmapRGB* input, bitmapRGB* output)
{
    long_t height = input->height, width = input->width;
    long_t i, j;
    output->height = height;
    output->width = width;
    output->pixel = malloc(height * width * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            output->pixel[i * width + j] =  input->pixel[(height - i) * width + j];
        }
    }
    return SUCCESS;
}

int adjust_contrast(const bitmapRGB* input, bitmapRGB* output, double k)
{
    long_t height = input->height, width = input->width;
    long_t i;
    output->height = height;
    output->width = width;
    output->pixel = malloc(height * width * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height * width; i++) {
        output->pixel[i] = adjust_pixel_contrast(&input->pixel[i], k);
    }
    return SUCCESS;
}

pixelRGB adjust_pixel_contrast(pixelRGB* pixel, double k)
{
    pixelRGB adjusted;
    adjusted.red = adjust_color_contrast(pixel->red, k);
    adjusted.green = adjust_color_contrast(pixel->green, k);
    adjusted.blue = adjust_color_contrast(pixel->blue, k);
    return adjusted;
}

byte_t adjust_color_contrast(byte_t value, double k)
{
    int result = value * k;
    if (result > 255) return 255;
    if (result < 0) return 0;
    return result;
}

int adjust_gamma(const bitmapRGB* input, bitmapRGB* output, double g)
{
    long_t height = input->height, width = input->width;
    long_t i;
    output->height = height;
    output->width = width;
    output->pixel = malloc(height * width * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < height * width; i++) {
        output->pixel[i] = adjust_pixel_gamma(&input->pixel[i], g);
    }
    return SUCCESS;
}

pixelRGB adjust_pixel_gamma(pixelRGB* pixel, double g)
{
    pixelRGB adjusted;
    adjusted.red = adjust_color_gamma(pixel->red, g);
    adjusted.green = adjust_color_gamma(pixel->green, g);
    adjusted.blue = adjust_color_gamma(pixel->blue, g);
    return adjusted;
}

byte_t adjust_color_gamma(byte_t value, double g)
{
    double res = pow(value, g);
    if (res > 255) return 255;
    if (res < 0) return 0;
    return res;
}

int double_size(const bitmapRGB* input, bitmapRGB* output)
{
    long_t height = input->height, width = input->width;
    long_t i, j;
    output->height = 2 * height;
    output->width = 2 * width;
    output->pixel = malloc(4 * height * width * sizeof(pixelRGB));
    if (output->pixel == NULL) return 1;
    for (i = 0; i < 2 * height; i += 2) {
        for (j = 0; j < 2 * width; j += 2) {
            output->pixel[i * 2 * width + j] = input->pixel[(i / 2) * width + j / 2];
            output->pixel[i * 2 * width + j + 1] = input->pixel[(i / 2) * width + j / 2];
            output->pixel[(i + 1) * 2 * width + j] = input->pixel[(i / 2) * width + j / 2];
            output->pixel[(i + 1) * 2 * width + j + 1] = input->pixel[(i / 2) * width + j / 2];
        }
    }
    return SUCCESS;
}

byte_t convert_luminance(pixelRGB* pixel)
{
    return (0.299 * pixel->red + 0.587 * pixel->green + 0.114 * pixel->blue);
}

#pragma endregion

#pragma region Kryptologie

#define LEN 26

#pragma region cdecode24.c
void caesar_decode(char key, const char *code, char *text, int len) {
	int i;
	for (i = 0; i < len; i++) {
		text[i] = caesar_decode_char(code[i], key);
	}
	text[i] = '\0';
}

char caesar_decode_char(char c, char key)
{
	int r;
	r = c - key + 'A';
	if (r < 'A') r += LEN;
	return (char) r;
}

int main(void)
{
    const char* code = "IKHZKTFFBXKDNKLBGV";
    char key = 'T';
    char* decode = malloc((strlen(code) + 1) * sizeof(char));
    if (decode == NULL) {
        puts("malloc error");
        return EXIT_FAILURE;
    }
    caesar_decode(key, code, decode, strlen(code));
    printf("%s\ndecoded with key '%c':\n%s\n", code, key, decode);
    free(decode);
    return EXIT_SUCCESS;
}

#pragma endregion

#pragma region cencode24.c

int read_letter() {
	int c;
	c = getchar();
	while (getchar() != '\n');
	assert(c >= 'A' && c <= 'Z');
	return c;
}

void caesar_encode(char key, char *code, const char *text, int len) {
	/* freiwillig zu implementieren */
	int i;
	for (i = 0; i < len; i++) {
		code[i] = caesar_encode_char(text[i], key);
	}
	code[i] = '\0';
}

char caesar_encode_char(char c, char key)
{
	int r;
	r = c + key - 'A';
	if (r > 'Z') r -= LEN;
	return (char)r;
}

int main(void)
{
    const char* text = "PROGRAMMIERKURSINC";
    char key;
    char* code = malloc((strlen(text) + 1) * sizeof(char));
    if (code == NULL) {
        puts("malloc error");
        return EXIT_FAILURE;
    }
    printf("Text: \"%s\"\nEnter key: ", text);
    key = read_letter();
    caesar_encode(key, code, text, strlen(text));
    printf("Encoded with '%c': \"%s\"\n", key, code);
    free(code);
    return EXIT_SUCCESS;
}

#pragma endregion

#pragma region vencode24.c

void viginere_encode(const char *key, char *code, const char *text, int len) {
	/* freiwillig zu implementieren */
	int i;
	int key_len = strlen(key);
	for (i = 0; i < len; i++) {
		code[i] = caesar_encode_char(text[i], key[i % key_len]);
	}
	code[i] = '\0';
}

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* read_key()
{
    int key_len, i, c;
    char* key;
    printf("Enter key length: ");
    while (scanf("%i", &key_len) != 1) {
        printf("Enter key length: ");
        flush();
    }
    flush();
    key = malloc((key_len + 1) * sizeof(char));
    if (key == NULL) {
        puts("malloc error");
        return NULL;
    }
    printf("Please enter key:");
    for (i = 0; i < key_len; i++) {
        c = getchar();
        if (c == EOF) {
            puts("EOF error");
            free(key);
            return NULL;
        }
        if (!isupper(c)) {
            puts("Please only enter uppercase letters");
            i--;
            continue;
        }
        key[i] = c;
    }

    return key;
}

int main(void)
{
    char* key;
    const char* text = "INDERVORLESUNGWURDEEINEIDEEVONKASISKIZURERMITTLUNGDERSCHLUESSELLAENGEEINESVIGINERECODESVORGESTELLTEINWICHTIGERTEILDAVONISTDIEBESTIMMUNGDERABSTAENDEZWISCHENSUBSTRINGSFESTERLAENGEIMVORLIEGENDENCODE";
    char* code = malloc((strlen(text) + 1) * sizeof(char));
    if (code == NULL) {
        puts("malloc error");
        return EXIT_FAILURE;
    }
    printf("Text: \"%s\"\n", text);
    key = read_key();
    if (key == NULL) {
        return EXIT_FAILURE;
    }

    printf("key: \"%s\"\n", key);
    viginere_encode(key, code, text, strlen(text));
    printf("Code: \"%s\"\n", code);

    free(code);
    free(key);
    return EXIT_SUCCESS;
}

#pragma endregion

#pragma region vfreq24.c

int read_letter() {
	int c;
	c = getchar();
	while (getchar() != '\n');
	assert(c >= 'A' && c <= 'Z');
	return c;
}

void generate_subcode(const char *code, char *subcode, int keylen, int keypos) {
	int i, pos = keypos;
	int code_len = strlen(code);
	for (i = 0; keypos < code_len; i++, keypos += keylen) {
		subcode[i] = code[keypos];
	}
	subcode[i] = '\0';
}

void freq_table(int count[], const char *code) {
	/* zu implementieren */
	char c;
	int i, code_len;
	code_len = strlen(code);
	for (i = 0; i <= LEN; i++) {
		count[i] = 0;
	}
	for (i = 0; i < code_len; i++) {
		c = code[i];
		count[c - 'A']++;
	}
}

void print_table(int count[]) {
	int j = 0;
	while (j < LEN) {
		printf("%c: %i\n", 'A' + j, count[j]);
		++j;
	}
}

char freq_analysis_caesar(char max) {
	int key = max - 'E' + 'A';
	if (key < 'A') key + LEN;
	return key;
}

void viginere_decode(const char *key, const char *code, char *text, int len) {
	int i;
	int key_len = strlen(key);
	for (i = 0; i < len; i++) {
		text[i] = caesar_decode_char(code[i], key[i % key_len]);
	}
	text[i] = '\0';
}

void flush()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void)
{
    const char* code = "KBFSTJQFNSUIPUYITRGSKBGWFSGJQBMOUWUYKNWFGFOWVHNIPUFSTGEVNIGGUSNZCSPUGSKBGGXWIWPSTSECFSUJQFISUHGZNHGWPKKQJHKUGFVSKZFOXCPWUHFWGPGGVWOAWBIRGFCPUHCSPRGNYWUQJSPGWPUHTWPUUTGGVSTZCSPUGWOJQFNWGUGBFSPQQRG";
    char* sub_code;
    char* key; /*should be CO*/
    char* decode;
    int* count;
    int key_len, status, i;
    printf("Code: %s\n", code);
    printf("Please enter key length: ");
    while (scanf("%i", &key_len) != 1) {
        printf("Please enter key length: ");
        flush();
    }
    flush();
    printf("%i\n", key_len);
    key = malloc((key_len + 1) * sizeof(char));
    if (key == NULL) {
        puts("malloc error");
        return EXIT_FAILURE;
    }
    sub_code = malloc((strlen(code) / key_len + 1) * sizeof(char));
    if (sub_code == NULL) {
        puts("malloc error");
        free(key);
        return EXIT_FAILURE;
    }
    count = malloc(LEN * sizeof(char));
    if (count == NULL) {
        puts("malloc error");
        free(key);
        free(sub_code);
        return EXIT_FAILURE;
    }

    decode = malloc((strlen(code) + 1) * sizeof(char));
    if (decode == NULL) {
        puts("malloc error");
        free(key);
        free(sub_code);
        free(count);
        return EXIT_FAILURE;
    }

    for (i = 0; i < key_len; i++) {
        generate_subcode(code, sub_code, key_len, i);
        printf("subcode: %s\n", sub_code);
        freq_table(count, sub_code);
        print_table(count);
        printf("Please enter one of the most common letters: ");
        key[i] = freq_analysis_caesar(read_letter());
    }
    key[i] = '\0';

    viginere_decode(key, code, decode, strlen(code));

    printf("Decoded with key \"%s\":\n%s\n", key, decode);

    free(key);
    free(sub_code);
    free(count);
    free(decode);
    return EXIT_SUCCESS;
}

#pragma endregion

#pragma region cbruteforce24.c

char caesar_decode_char(char c, char key)
{
	int r;
	r = c - key + 'A';
	if (r < 'A') r += LEN;
	return (char) r;
}

void caesar_decode(char key, const char *code, char *text, int len) {
	int i;
	for (i = 0; i < len; i++) {
		text[i] = caesar_decode_char(code[i], key);
	}
	text[i] = '\0';
}

void brute_caesar(const char *code, int len_decode) {
	/* freiwillig zu implementieren */
	int key;
	char* decode = malloc((len_decode + 1) * sizeof(char));
	assert(decode != NULL);
	for (key = 'A'; key <= 'Z'; key++) {
		caesar_decode(key, code, decode, len_decode);
		printf("'%c': \"%s\"\n", key, decode);
	}
	free(decode);
}

void flush()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int main(void)
{
    const char* code = "OHGWXFZXEWWTLPBKGBVAMATUXGDTNYXGPBKWBGZXWBXPBKGBVAMUKTNVAXGNFEXNMXGSNBFIHGBXKXGWBXPBKGBVAMFHXZXG";
    char* decode;
    int len_decode;
    char key;
    decode = malloc((strlen(code) + 1) * sizeof(char));
    if (decode == NULL) {
        puts("malloc error");
        return EXIT_FAILURE;
    }
    printf("Enter how much should be decoded: ");
    while (scanf("%i", &len_decode) != 1) {
        printf("Enter how much should be decoded: ");
        flush();
    }
    flush();
    if (len_decode > (int)strlen(code)) {
        printf("Code is only %i long\nlen_decode has been set to this value\n", (int)strlen(code));
        len_decode = (int)strlen(code);
    }
    brute_caesar(code, len_decode);
    printf("Enter key: ");
    key = read_letter();
    caesar_decode(key, code, decode, strlen(code));
    printf("key: '%c'\n", key);
    printf("Decoded:\n\"%s\"\n", decode);

    free(decode);
    return EXIT_SUCCESS;
}

#pragma endregion

#pragma endregion

#pragma region Intervallschachtelung

#pragma region A1
/*n/m*/
int iquot(int n, int m)
{
    int u = 0;
    assert(u == 0 && n > 0 && m > 0); /*V: Vorbedingung*/
    assert(u * m <= n); /*I: Schleifeninvariante*/
    while ((u + 1) * m <= n) {
        assert((u + 1) * m <= n); /*B: Schleifenbedingung*/
        assert(u * m <= n); /*I: Schleifeninvariante*/
        u++;
        assert(u * m <= n); /*I: Schleifeninvariante*/
    }
    assert(u * m <= n && (u + 1) * m > n); /*N: Nachbedingung*/
    return u;
}

int iquot_fortschaltung(int n, int m)
{
    int u = 0;
    int h = m;
    assert(u == 0 && n > 0 && m > 0); /*V: Vorbedingung*/
    assert(h == (u + 1) * m && h <= n && u * m <= n); /*I: Schleifeninvariante*/
    while (h <= n) {
        assert(h <= n); /*B: Schleifenbedingung*/
        assert(u * m <= n && h == (u + 1) * m); /*I: Schleifeninvariante*/
        u++;
        assert(u * m <= n && h == u * m);
        h += m;
        assert(u * m <= n && h == (u + 1) * m); /*I: Schleifeninvariante*/
    }
    assert(u * m <= n && h == (u + 1) * m && h > n); /*I && !B*/
    assert(u * m <= n && (u + 1) * m > n); /*N: Nachbedingung*/
    return u;
}

int iquot_halbierung(int n, int m)
{
    int u = 0;
    int o = n + 1;
    int a;
    assert(u == 0 && o == n + 1 && n > 0 && m > 0); /*V: Vorbedingung*/
    assert(u * m <= n && o * m > n && u <= o - 1); /*I: Schleifeninvariante*/
    while (u < o - 1) {
        assert(u < o - 1); /*B: Schleifenbedingung*/
        assert(u * m <= n && o * m > n && u <= o - 1); /*I: Schleifeninvariante*/
        a = (u + o) / 2;
        if (a * m <= n) {
            u = a;
        } else {
            o = a;
        }
        assert(u * m <= n && o * m > n && u <= o - 1); /*I: Schleifeninvariante*/
    }
    assert(u * m <= n && o * m > n && u <= o - 1 && u >= o - 1); /*I && !B*/
    assert(u * m <= n && (u + 1) * m > n); /*N: Nachbedingung*/
    return u;
}
#pragma endregion

#pragma region A2
int isqrt(int n)
{
    int u = 0;
    assert(u == 0 && n >= 0); /*V: Vorbedingung*/
    assert(u * u <= n); /*I: Schleifeninvariante*/
    while ((u + 1) * (u + 1) <= n) {
        assert((u + 1) * (u + 1) <= n); /*B: Schleifenbedingung*/
        assert(u * u <= n); /*I: Schleifeninvariante*/
        u++;
        assert(u * u <= n); /*I: Schleifeninvariante*/
    }
    assert(u * u <= n && (u + 1) * (u + 1) > n); /*N: Nachbedingung*/
    return u;
}

int isqrt_fortschaltung(int n)
{
    int u = 0;
    int h = 1; /*= (u + 1) * (u + 1) = u^2 + 2u + 1*/
    assert(u == 0 && h == 1 && n >= 0); /*V: Vorbedingung*/
    assert(u * u <= n && h == (u + 1) * (u + 1)); /*I: Schleifeninvariante*/
    while (h <= n) {
        assert(h <= n); /*B: Schleifenbedingung*/
        assert(u * u <= n && h == (u + 1) * (u + 1)); /*I: Schleifeninvariante*/
        u++;
        assert(h == u * u);
        h += u + u + 1;
        assert(u * u <= n && h == (u + 1) * (u + 1)); /*I: Schleifeninvariante*/
    }
    assert(u * u <= n && h == (u + 1) * (u + 1) && h > n); /*I && !B*/
    assert(u * u <= n && (u + 1) * (u + 1) > n); /*N: Nachbedingung*/
    return u;
}

int isqrt_halbierung(int n)
{
    int u = 0;
    int o = n + 1;
    int a;
    assert(u == 0 && o == n + 1 && n >= 0); /*V: Vorbedingung*/
    assert(u * u <= n && o * o > n); /*I: Schleifeninvariante*/
    while (u < o - 1) {
        assert(u < o - 1); /*B: Schleifenbedingung*/
        assert(u * u <= n && o * o > n); /*I: Schleifeninvariante*/
        a = (u + o) / 2;
        if (a * a > n) {
            o = a;
        } else {
            u = a;
        }
        assert(u * u <= n && o * o > n); /*I: Schleifeninvariante*/
    }
    assert(u * u <= n && o * o > n && u >= o - 1); /*I && !B*/
    assert(u * u <= n && (u + 1) * (u + 1) > n); /*N: Nachbedingung*/
    return u;
}
#pragma endregion

#pragma region A3
int ilog(int a, int n)
{
    int u = 0;
    assert(u == 0 && n >= 1 && a > 1); /*V: Vorbedingung*/
    assert(pow(a, u) <= n); /*I: Schleifeninvariante*/
    while (pow(a, u + 1) <= n) {
        assert(pow(a, u + 1) <= n); /*B: Schleifenbedingung*/
        assert(pow(a, u) <= n); /*I: Schleifeninvariante*/
        u++;
        assert(pow(a, u) <= n); /*I: Schleifeninvariante*/
    }
    assert(pow(a, u) <= n && pow(a, u + 1) > n); /*N: Nachbedingung*/
    return u;
}

int ilog_fortschaltung(int a, int n)
{
    int u = 0;
    int h = a; /*pow(a, u + 1) = pow(a, 1) = a*/
    assert(u == 0 && h == a && a > 1 && n > 0); /*V: Vorbedingung*/
    assert(pow(a, u) <= n && h == pow(a, u + 1)); /*I: Schleifeninvariante*/
    while (h <= n) {
        assert(h <= n); /*B: Schleifenbedingung*/
        assert(pow(a, u) <= n && h == pow(a, u + 1)); /*I: Schleifeninvariante*/
        u++;
        assert(h == pow(a, u));
        h *= a;
        assert(pow(a, u) <= n && h == pow(a, u + 1)); /*I: Schleifeninvariante*/
    }
    assert(pow(a, u) <= n && h == pow(a, u + 1) && h > n); /*I && !B*/
    assert(pow(a, u) <= n && pow(a, u + 1) > n); /*N: Nachbedingung*/
    return u;
}

int ilog_halbierung(int a, int n)
{
    int u = 0;
    int o = n + 1;
    int k;
    assert(u == 0 && o == n + 1 && a > 1 && n > 0); /*V: Vorbedingung*/
    assert(pow(a, u) <= n && pow(a, o) > n); /*I: Schleifeninvariante*/
    while (u < o - 1) {
        assert(u < o - 1); /*B: Schleifenbedingung*/
        assert(pow(a, u) <= n && pow(a, o) > n); /*I: Schleifeninvariante*/
        k = (u + o) / 2;
        if (pow(a, k) > n) {
            o = k;
        } else {
            u = k;
        }
        assert(pow(a, u) <= n && pow(a, o) > n); /*I: Schleifeninvariante*/
    }
    assert(pow(a, u) <= n && pow(a, o) > n && u >= o - 1); /*I && !B*/
    assert(pow(a, u) <= n && pow(a, u + 1) > n); /*N: Nachbedingung*/
    return u;
}
#pragma endregion

double euler(double eps)
{
    int k = 1;
    double e = 1;
    while (2 / factorial(k + 1) > eps) {
        e += 1 / factorial(++k);
    }
    return e;
}

double my_pi(double eps)
{
    int k = 0;
    double pi = 4;
    while (4. / (2 * k + 1) > eps) {
        k++;
        pi += (k % 2 == 0 ? 4. : -4.) / (2 * k + 1);
    }
    return pi;
}

#pragma region cos(x)
double cosinus_helper(double x, int k)
{
    int i;
    double res = 1;
    for (i = 0; i < k; i++) {
        res *= x / (k - i); 
    }
    return res;
}

double cosinus(double x, double eps)
{
    double cosinus_result = 0;
    int k = 0;
    do {
        cosinus_result += (k % 2 ? -1 : 1) * cosinus_helper(x, 2 * k);
        k++;
    } while (2 * cosinus_helper(x, 2 * k + 2) > eps);
    return cosinus_result;
}
#pragma endregion

double newton_root(int n, double a, double eps)
{
    double r = (a + 1) / 2, r_old;
    do {
        r_old = r;
        r = ((n - 1) * r + (a / pow(r, n - 1))) / n;
    } while (fabs(r - r_old) > eps);
    return r;
}
#pragma endregion
