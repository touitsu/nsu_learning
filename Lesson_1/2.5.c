#include <stdio.h>
#include <stdlib.h>


int main() {
	int N, K, M;
	scanf_s("%d %d %d", &N, &K, &M);
	M = abs(M - K);
	K = 0;
	if (M > N / 2) {
		M = N - M;
	}
	printf("%d", M - 1);
	return 0;
}