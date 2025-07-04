#include "./base64.h"
 
/**
 * char base64_char_to_ascii_char(uchar_t chr);
 * 
 * Converts a Base64 character (0-63) to its ASCII representation.
 */
char base64_char_to_ascii_char(uchar_t chr) {
	// handle invalid characters
	if (chr > BASE64_CHAR_COUNT) {
		return NULL_CHAR;
	}
	return BASE64_CHAR_TABLE[chr];
}

uchar_t ascii_char_to_base64_char(char chr) {
	// handle invalid characters
	if ((chr < 'A' || chr > 'Z') && 
		(chr < 'a' || chr > 'z') && 
		(chr < '0' || chr > '9') && 
		chr != '+' && 
		chr != '/') {
		return NULL_CHAR; // invalid character
	}
	for (uchar_t i = 0; i < BASE64_CHAR_COUNT; i++) {
		if (BASE64_CHAR_TABLE[i] == chr) {
			return i; // return the index of the character in the Base64 table
		}
	}
	return NULL_CHAR; // character not found in Base64 table
}

/**
 * uchar_t base64_encoded_padding(size_t size);
 *
 * Calculates the number of padding characters needed for some content to be
 * encoded in Base64, given its `size` in bytes.
 */
uchar_t base64_encoded_padding(size_t size) {
	size_t bits = size * BYTE_BITS;

	uchar_t remainder = 6 - (uchar_t)(bits % 6);
	if (remainder == 6) {
		remainder = 0;
	}

	return remainder / 2;
}

/*
 * size_t base64_encoded_size(size_t size);
 *
 * Calculates the output size of encoding input of some `size` of bytes to
 * Base64.
 */
size_t base64_encoded_size(size_t size) {
	size_t bits = size * BYTE_BITS;

	uchar_t remainder = 6 - (uchar_t)(bits % 6);
	if (remainder == 6) {
		remainder = 0;
	}

	uchar_t padding = remainder / 2;

	return ((bits + remainder) / 6) + padding;
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

	char* output_buffer = (char*)malloc((output_size + 1) * sizeof(char));
	if (output_buffer == NULL) {
		return NULL;
	}
	output_buffer[output_size] = NULL_CHAR; // null-terminate the string

	// apply padding
	uchar_t padding = base64_encoded_padding(size);
	for (uchar_t i = 0; i < padding; i++) {
		output_buffer[output_size - i - 1] = BASE64_PAD_CHAR;
	}

	// process each input ASCII char and write to output buffer
	size_t  input_index    = 0; // current index in input buffer
	size_t  output_index   = 0; // current index in output buffer
 	uchar_t temp_buffer    = 0; // temporary buffer to store bits as they are processed
	uchar_t remaining_bits = 0; // bits not consumed in last iteration
	uchar_t dropped_bits   = 0; // bits dropped (by shift) when loading next chunk
	while (input_index < size) {
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

			// only try to encode the next char if we have enough bits
			if (remaining_bits >= BASE64_CHAR_BITS) {
				// encode the next 6 bits
				output_buffer[output_index] = base64_char_to_ascii_char(temp_buffer >> (BYTE_BITS - BASE64_CHAR_BITS));

				output_index++;

				temp_buffer <<= BASE64_CHAR_BITS;
				remaining_bits -= BASE64_CHAR_BITS; // buffer was refilled and bits were consumed
			}
		}
		input_index++;
	}

	if (padding > 0) {
		output_buffer[output_index] = base64_char_to_ascii_char(temp_buffer >> (BYTE_BITS - BASE64_CHAR_BITS));
		output_index++;
	}
	
	return output_buffer;
}

/**
 * uchar_t base64_decoded_padding(char* buffer, size_t size);
 *
 * Calculates the number of padding characters in a Base64 encoded string
 * of given `size` in bytes.
 */
uchar_t base64_decoded_padding(char* buffer, size_t size) {
	// calculate the number of padding characters
	uchar_t padding;
	// is `(BYTE_BITS - BASE64_CHAR_BITS)` coincidentally the right number or does this make sense???
	for (padding = 0; padding < (BYTE_BITS - BASE64_CHAR_BITS); padding++) {
		if (buffer[size - padding - 1] != BASE64_PAD_CHAR) {
			break;
		}
	}

	return padding;
}

/**
 * size_t base64_decoded_size(char* buffer, size_t size);
 *
 * Calculates the output size of decoding a Base64 encoded string of given
 * `size` in bytes.
 */
size_t base64_decoded_size(char* buffer, size_t size) {
	uchar_t padding = base64_decoded_padding(buffer, size);

	size_t bits = (size - padding) * BASE64_CHAR_BITS;

	return bits / BYTE_BITS;
}

/**
 * char* base64_decode(char* input_buffer, size_t size);
 * 
 * Decodes a Base64 encoded string from `input_buffer` of given `size`.
 */
char* base64_decode(char* input_buffer, size_t size) {
	// allocate memory for output buffer
	size_t output_size = base64_decoded_size(input_buffer, size);

	char* output_buffer = (char*)malloc((output_size + 1) * sizeof(char));
	if (output_buffer == NULL) {
		return NULL;
	}
	output_buffer[output_size] = NULL_CHAR; // null-terminate the string

	// process each input Base64 char and write to output buffer
	size_t  input_index    = 0; // current index in input buffer
	size_t  output_index   = 0; // current index in output buffer
 	uchar_t temp_buffer    = 0; // temporary buffer to store bits as they are processed
	uchar_t remaining_bits = 0; // bits not consumed in last iteration
	while (input_index < size) {
		temp_buffer |= ascii_char_to_base64_char(input_buffer[input_index]) << remaining_bits;

		remaining_bits += BASE64_CHAR_BITS;

		if (remaining_bits >= BYTE_BITS) {
			output_buffer[output_index] = temp_buffer >> (remaining_bits - BYTE_BITS);
			output_index++;

			temp_buffer <<= BYTE_BITS;
			remaining_bits -= BYTE_BITS; // buffer was refilled and bits were consumed
		}

		input_index++;
	}

	return output_buffer;
}
