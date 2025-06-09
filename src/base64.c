#include "./base64.h"

/**
 * char encode_ascii_char(char chr);
 *
 * Converts a given ASCII character to a Base64 character.
 */
char encode_ascii_char(char chr) {
	if (chr >= 'A' && chr <= 'Z') {
		return chr - 'A';
	} else if (chr >= 'a' && chr <= 'z') {
		return chr - 'a' + 26;
	} else if (chr >= '0' && chr <= '9') {
		return chr - '0' + 52;
	} else if (chr == '+') {
		return 62;
	} else if (chr == '/') {
		return 63;
	}
	
	return 0;
}

/*
 * char decode_base64_char(char chr);
 *
 * Converts a given Base64 character to an ASCII character.
 */
char decode_base64_char(char chr) {
	if (chr < 26) {
		return 'A' + chr;
	} else if (chr < 52) {
		return 'a' + (chr - 26);
	} else if (chr < 62) {
		return '0' + (chr - 52);
	} else if (chr == 62) {
		return '+';
	} else if (chr == 63) {
		return '/';
	} else {
		return '\0'; // invalid char
	}
}

/*
 * size_t base64_encoded_size(size_t size);
 *
 * Calculates the output size of encoding input of some `size` of bytes to
 * Base64.
 */
size_t base64_encoded_size(size_t size) {
	size_t bits = size * BYTE_BITS;

	ushort_t remainder = 6 - (bits % 6);
	if (remainder == 6)
		remainder = 0;

	ushort_t padding = remainder / 2;

	return ((bits + remainder) / 6) + padding;
}

/**
 * ushort_t base64_encoded_padding(size_t size);
 *
 * Calculates the number of padding characters needed for some content to be
 * encoded in Base64, given its `size` in bytes.
 */
ushort_t base64_encoded_padding(size_t size) {
	size_t bits = size * BYTE_BITS;

	ushort_t remainder = 6 - (bits % 6);
	if (remainder == 6)
		remainder = 0;

	return remainder / 2;
}

/**
 * char* base64_encode(char* input_buffer, size_t* size);
 *
 * Returns a pointer to a buffer containing the contents of the `input_buffer`
 * encoded in Base64.
 */
char* base64_encode(char* input_buffer, size_t size) {
	// allocate memory for output buffer
	size_t output_size = base64_encoded_size(size);

	char* output_buffer = (char*)calloc(output_size + 1, sizeof(char));
	if (output_buffer == NULL) {
		return NULL;
	}
	output_buffer[output_size] = '\0'; // Null-terminate the string

	// apply padding
	ushort_t padding = base64_encoded_padding(size);
	for (ushort_t i = 0; i < padding; i++) {
		output_buffer[output_size - i - 1] = '=';
	}

	// process each input ASCII char and write to output buffer
	size_t  input_index    = 0;
	size_t  output_index   = 0;
 	uchar_t temp_buffer    = 0;
	uchar_t remaining_bits = 0;
	uchar_t dropped_bits   = 0;
	while (input_index < size) {
		printf("input_index: %lu, output_index: %lu\n", input_index, output_index); // DEBUG
		// load next chunk of bits into temp buffer
		temp_buffer <<= BYTE_BITS - remaining_bits;
		temp_buffer |= input_buffer[input_index] >> remaining_bits;
		dropped_bits = remaining_bits;

		// reset remaining bits
		remaining_bits = BYTE_BITS - remaining_bits;

		// output the next chunk of bits
		output_buffer[output_index] = encode_ascii_char(temp_buffer >> 2);
		remaining_bits -= 6;
		printf("%d -> %c\n", (int)temp_buffer, output_buffer[output_index]); // DEBUG
		output_index++;

		if (dropped_bits != 0) {
			temp_buffer <<= dropped_bits;
			ushort_t shift = BYTE_BITS - dropped_bits;
			temp_buffer |= (input_buffer[input_index] << shift) >> shift;
		}
		input_index++;
	}
	
	return output_buffer;
}
