// [DEBUG]
#include <stdio.h>
// [\DEBUG]
#include "./base64.h"

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

void dump_bin(uchar_t data) {
	printf("0b");
	for (char i = 7; i > 0; i--) {
		printf("%d", (data >> i) & 1);
	}
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
	size_t  input_index    = 0; // current index in input buffer
	size_t  output_index   = 0; // current index in output buffer
 	uchar_t temp_buffer    = 0; // temporary buffer to store bits as they are processed
	uchar_t remaining_bits = 0; // bits not consumed in last iteration
	uchar_t dropped_bits   = 0; // bits dropped (by shift) when loading next chunk
	printf("%d %d %d\n\n\n\n", 'M', 'a', 'n');
	while (input_index < size) {
		// [DEBUG]
		printf("Processing input[%zu]: %c\n", input_index, (uchar_t)input_buffer[input_index]);
		// [\DEBUG]
		dropped_bits = BYTE_BITS;
		while (dropped_bits > 0) {
			uchar_t shift = BYTE_BITS - dropped_bits;
			temp_buffer |= ((uchar_t)input_buffer[input_index] << shift) >> remaining_bits;

			// calculate how many dropped bits were consumed
			uchar_t consumable = BYTE_BITS - remaining_bits;
			if (dropped_bits < consumable) {
				consumable = dropped_bits; // do not drop more bits than available
			}

			dropped_bits -= consumable;
			remaining_bits += consumable;
			
			if (remaining_bits >= BASE64_CHAR_BITS) { // only try to encode the next char if we have enough bits
				output_buffer[output_index] = temp_buffer >> (BYTE_BITS - BASE64_CHAR_BITS); // encode the next 6 bits
				// [DEBUG]
				printf("temp_buffer: ");
				dump_bin(temp_buffer >> (BYTE_BITS - BASE64_CHAR_BITS));
				printf("\n");
				printf("%d -> %d\n", (char)(temp_buffer >> (BYTE_BITS - BASE64_CHAR_BITS)), output_buffer[output_index]);
				// [\DEBUG]

				output_index++;

				temp_buffer <<= BASE64_CHAR_BITS;
				remaining_bits = BYTE_BITS - BASE64_CHAR_BITS; // buffer was refilled and bits were consumed
			}
		}
		input_index++;
	}
	if (remaining_bits > 0) {
		output_buffer[output_index] = temp_buffer;
		output_index++;
	}
	
	return output_buffer;
}
