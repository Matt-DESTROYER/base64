#include <stdio.h>
#include <string.h>
#include "base64.h"
void dump_bin(uchar_t data) {
	printf("0b");
	for (char i = 7; i > 0; i--) {
		printf("%d", (data >> i) & 1);
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* input_string = argv[1];

	size_t input_length = strlen(input_string);
	char* encoded_string = base64_encode(input_string, input_length);
	if (encoded_string == NULL) {
		fprintf(stderr, "Error encoding string to Base64.\n");
		return EXIT_FAILURE;
	}

	printf("Encoded Base64: %s\n", encoded_string);

	size_t encoded_length = strlen(encoded_string);
	size_t expected_length = base64_encoded_size(input_length);
	if (encoded_length != expected_length) {
		fprintf(stderr, "Error: Encoded length does not match expected length.\n");
		free(encoded_string);
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < encoded_length; i++) {
		printf("%d ", encoded_string[i]);
	}
	printf("\n");

	for (size_t i = 0; i < encoded_length; i++) {
		dump_bin(encoded_string[i]);
		printf(" ");
	}
	printf("\n");

	free(encoded_string);

	return EXIT_SUCCESS;
}
