#ifndef BASE64_H
#define BASE64_H

#include <stdlib.h>

typedef unsigned char  uchar_t;  // unsigned char type
typedef unsigned short ushort_t; // unsigned short type

#define BYTE_BITS        8 // number of bits in a byte
#define BASE64_CHAR_BITS 6 // number of bits in a base64 character

char encode_ascii_char(char chr);
char decode_base64_char(char chr);
size_t base64_encoded_size(size_t size);
ushort_t base64_encoded_padding(size_t size);
char* base64_encode(char* input_buffer, size_t size);

#endif
