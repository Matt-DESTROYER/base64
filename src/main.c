#include <stdio.h>
#include <string.h>
#include "base64.h"

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

	free(encoded_string);

	return EXIT_SUCCESS;
}
