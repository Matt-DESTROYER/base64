#ifndef BASE64_H
#define BASE64_H

#include <stdlib.h>

typedef unsigned char  uchar_t;  // unsigned char type

#define BYTE_BITS         8 // number of bits in a byte
#define BASE64_CHAR_BITS  6 // number of bits in a base64 character

#define BASE64_CHAR_TABLE "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_CHAR_COUNT 64
#define BASE64_PAD_CHAR   (uchar_t)'='

#define NULL_CHAR      '\0' // invalid character

uchar_t base64_encoded_padding(size_t size);
size_t base64_encoded_size(size_t size);
char* base64_encode(char* input_buffer, size_t size);
uchar_t base64_decoded_padding(char* buffer, size_t size);
size_t base64_decoded_size(char* buffer, size_t size);
char* base64_decode(char* input_buffer, size_t size);

#endif
