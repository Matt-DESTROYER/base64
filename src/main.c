#include <stdio.h>
#include <string.h>
#include "base64.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage:\n1. %s <input_string>\n2. %s --file <path/to/input_file.ext>\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	uchar_t file_flag = 0;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) {
			if (file_flag) {
				fprintf(stderr, "Error: Multiple input files provided.\n");
				return EXIT_FAILURE;
			}
			
			file_flag = 1;
		}
	}

	char* input_string = argv[argc - 1];
	size_t input_length = 0;
	
	if (file_flag) {
		FILE* file = fopen(input_string, "rb");

		if (file == NULL) {
			fprintf(stderr, "Error opening file: %s\n", input_string);
			return EXIT_FAILURE;
		}

		fseek(file, 0, SEEK_END);
		input_length = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (input_length == 0) {
			fclose(file);
			fprintf(stderr, "Error: Input file is empty.\n");
			return EXIT_FAILURE;
		}

		input_string = malloc(input_length + 1);
		if (input_string == NULL) {
			fclose(file);
			fprintf(stderr, "Error allocating memory for input string.\n");
			return EXIT_FAILURE;
		}

		size_t bytes_read = fread(input_string, 1, input_length, file);
		fclose(file);

		if (bytes_read != input_length) {
			fprintf(stderr, "Error reading file: %s\n", input_string);
			free(input_string);
			return EXIT_FAILURE;
		}

		input_string[input_length] = '\0'; // Null-terminate the string
		input_length++; // Include the null terminator in the length
	} else {
		input_length = strlen(input_string);
	}

	char* encoded_string = base64_encode(input_string, input_length);
	if (encoded_string == NULL) {
		fprintf(stderr, "Error encoding string to Base64.\n");
		return EXIT_FAILURE;
	}

	size_t encoded_length = strlen(encoded_string);
	size_t expected_length = base64_encoded_size(input_length);
	if (encoded_length != expected_length) {
		free(encoded_string);
		fprintf(stderr, "Error: Encoded length does not match expected length.\n");
		return EXIT_FAILURE;
	}

	printf("%s\n", encoded_string);

	free(encoded_string);

	return EXIT_SUCCESS;
}
