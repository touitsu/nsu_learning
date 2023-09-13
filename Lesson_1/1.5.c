#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>


int main(){
	float y, hours_angle;
	scanf_s("%f", &y);
	hours_angle = (y * 180 / M_PI);
	printf("Minutes angle: %f\n", (hours_angle - 30 * (int)(hours_angle / 30)) * 12);
	printf("Hours passed: %1.f\n", hours_angle / 30);
	printf("Minutes passed: %d\n", (int)((hours_angle - 30 * (int)(hours_angle / 30)) * 12)/6);
	return 0;
}