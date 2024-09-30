#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <io.h>
#include "functions.h"


int32_t main(int32_t argc, char* argv[]) {
	FILE* in;
	FILE* out;
	FILE* kv;
	errno_t err;
	char* tmp;
	
	if (argc == 1) {
		printf("Specify flags, -e to encode, -d to decode, and a file name\n");
		exit(0);
	}

	if (charcmp(*(argv + 1), "-e\0") || charcmp(*(argv + 1), "-d\0")) {
		err = freopen_s(&in, *(argv + 2), "rb", stdin);

		if (err != 0) {
			printf("Error opening %s\n", *(argv + 2));
			exit(0);
		}

		if (charcmp(*(argv + 1), "-e\0")) {
			tmp = tobin(*(argv + 2));

			if (_access(tmp, 0)) {

				err = fopen_s(&kv, concat(*(argv + 2), "_out.bin"), "wb");
				if (err != 0) {
					printf("Can not create output file\n");
					exit(0);
				}

				err = fopen_s(&out, concat(*(argv + 2), "_out.txt"), "wb");
				if (err != 0) {
					printf("Can not create output file\n");
					exit(0);
				}

				encode(stdin, out, kv);
			}
			else {
				printf("Seems like file is already encoded\n");
				exit(0);
			}

			free(tmp);
		}
		else {

			tmp = tobin(*(argv + 2));
			if (_access(tmp, 0)) {
				printf("Code file doesn't exits\n");
				free(tmp);
				exit(0);
			}

			err = fopen_s(&kv, tmp, "rb");
			free(tmp);
			if (err != 0) {
				printf("Can not open code file\n");
				exit(0);
			}

			err = fopen_s(&out, concat(*(argv + 2), "_out.txt"), "wb");
			if (err != 0) {
				printf("Can not create output file\n");
				exit(0);
			}

			decode(stdin, out, kv);
		}
			
	}

	else {
		printf("Specify flags, -e to encode, -d to decode\n");
		exit(0);
	}
	
	fclose(out);
	fclose(stdin);
	return 0;
}