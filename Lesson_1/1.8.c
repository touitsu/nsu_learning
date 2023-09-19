#include <stdio.h>
#include <math.h>


int main() {
	int floors, flats, flat_to_find;
	scanf_s("%d %d %d", &floors, &flats, &flat_to_find);
	int entrance_number = (int)ceil(((float)flat_to_find / (floors * flats)));
	printf("Entrance: %d Floor: %d", entrance_number, (int)ceil((float)(flat_to_find - floors * flats * (entrance_number - 1 )) / 10));
	return 0;
}