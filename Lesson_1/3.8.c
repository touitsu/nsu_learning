#include <stdio.h>

int main() {
	int i, j, k, l;
	float s = 1.0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				for (l = 0; l < 4; l++) {
					s = 1.0;
					if (i == 0) {
						s += 2;
					}
					else if (i == 1) {
						s -= 2;
					}
					else if (i == 2) {
						s *= 2;
					}
					else {
						s /= 2;
					}
					if (j == 0) {
						s += 3;
					}
					else if (j == 1) {
						s -= 3;
					}
					else if (j == 2) {
						s *= 3;
					}
					else {
						s /= 3;
					}
					if (k == 0) {
						s += 4;
					}
					else if (k == 1) {
						s -= 4;
					}
					else if (k == 2) {
						s *= 4;
					}
					else {
						s /= 4;
					}
					if (l == 0) {
						s += 5;
					}
					else if (l == 1) {
						s -= 5;
					}
					else if (l == 2) {
						s *= 5;
					}
					else {
						s /= 5;
					}
					if (s >= 34) {
						printf("%d %d %d %d %f\n", i, j, k, l, s);
					}
				}
			}
		}
	}
	return 0;
}