#include <stdio.h>
#include <math.h>


int main() {
	int a, b;
	scanf_s("%d %d", &a, &b);
	printf("%d", (int)(ceil(((float)(ceil((float)(a%b)/a) + ceil((float)(b%a)/b)+1))/2))); // ceil((float)(a%b)/a) - ����������������� ������� ������� � �� b (�.� ����� ���� ��� ������: 0 ��� a%b = 0, 1 ��� �%b = ������� �� �������.
	return 0;																			   //ceil(...+1))/2 - ����� ��� ���������� ������� ������ ��� a=b, �.�. ��� ����� �������� a%b = b%a = 0 => ��� ����� ��������� 1,����� ���������, ���� ����� ��� �!=b ��������� ����� 2, � �� 1. � ��� a%b != 0 ����� �� ������� �� 2 ����� ������ 2 => ������� ceil ������ ����� ������� 2.
}