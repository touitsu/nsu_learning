#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct coefs_t {
	double a;
	double b;
	double c;
	double d;
} coefs_t;

typedef struct roots_t {
	double first;
	double second;
	double third;
	int8_t root_num;
} roots_t;

union {
 	double d;
	int64_t i;
} toNaN;

static inline double abs_val(const double val) {
	return val >= 0? val : -val;
}

static inline double calc_disc(const coefs_t* coefs) {
	return 4 * coefs->b * coefs->b - 12 * coefs->a * coefs->c;
}

static inline double calc_diff_func(const double x, const coefs_t* coefs) {
	return coefs->a * 3 * x * x + coefs->b * 2 * x + coefs->c;
}

static inline double calc_func(const double x, const coefs_t* coefs) {
	return coefs->a * x * x * x + coefs->b * x * x + coefs->c * x + coefs->d; 
}

double find_root(double a, double b, const coefs_t* coefs, const double eps) {
	double res, func_res, a_func_value, b_func_value;

	res = (a + b) / 2.0f;
	func_res = calc_func(res, coefs);
	a_func_value = calc_func(a, coefs);
	b_func_value = calc_func(b, coefs);

	if (abs_val(a_func_value) < eps) return a;
	if (abs_val(b_func_value) < eps) return b;

	while (abs_val(func_res) >= eps) {
		res = (a + b) / 2.0f;
		func_res = calc_func(res, coefs);

		if ((a_func_value > 0 && func_res > 0) || (a_func_value <= 0 && func_res <= 0)) {
			a = res;
			a_func_value = func_res;
		}
		else {
			b = res;
			b_func_value = func_res;
		}
	}

	return res;
}

double herons_method(const double s, const double eps) {
	double res;

	res = s / 2.0f;

	for (int32_t i = 0; abs_val(res*res - s) > eps && i < (1 << 30); i++) {
		res = (res + s/res) / 2.0f;
	}

	return res;
}

double find_left(const double b, const double delta, const coefs_t* coefs) {
	double a, a_func_res, b_func_res;
	int32_t delta_mul;
	
	a = b - 1;
	a_func_res = calc_func(a, coefs);
	b_func_res = calc_func(b, coefs);

	for (int32_t i = 0; (b_func_res > 0? a_func_res >= 0 : a_func_res <= 0) && i < 31; i++, a_func_res = calc_func(a, coefs)) {
		a -= delta * (double)(1 << i);
	}

	return a;
}

double find_right(const double a, const double delta, const coefs_t* coefs) {
	double b, a_func_res, b_func_res;

	b = a + 1;
	a_func_res = calc_func(a, coefs);
	b_func_res = calc_func(b, coefs);

	for (int32_t i = 0; (a_func_res > 0? b_func_res >= 0 : b_func_res <= 0) && i < 31; i++, b_func_res = calc_func(b, coefs)) {
		b += delta * (double)(1 << i);
	}

	return b;
}


void calc_roots(const coefs_t* coefs, const double eps, const double delta, roots_t* roots) {
	double alpha, beta, disc, disc_sqrt;
	double func_alpha_value, func_beta_value;

	disc = calc_disc(coefs);

	roots->first = toNaN.d;
	roots->second = toNaN.d;
	roots->third = toNaN.d;

	if (disc > 0) {
		asm volatile ("sqrtsd %1, %0" : "=x" (disc_sqrt) : "x" (disc));

		alpha = (-coefs->b * 2 - disc_sqrt) / (2 * 3 * coefs->a);
		beta = alpha + disc_sqrt * 2 / (2 * 3 * coefs->a);

		func_alpha_value = calc_func(alpha, coefs);
		func_beta_value = calc_func(beta, coefs);

		if (func_alpha_value > eps && func_beta_value > eps) {
			roots->first = find_root(find_left(alpha, delta, coefs), alpha, coefs, eps);
			roots->root_num = 1;
		}
		
		else if(func_alpha_value < -eps && func_beta_value < -eps) {
			roots->first = find_root(beta, find_right(beta, delta, coefs), coefs, eps);	
			roots->root_num = 1;
		}
		
		else if(func_alpha_value > eps && abs_val(func_beta_value) < eps) {
			roots->first = beta;
			roots->second = beta;
			roots->third = find_root(find_left(alpha, delta, coefs), alpha, coefs, eps);
			roots->root_num = 2;
		}

		else if(abs_val(func_alpha_value) < eps && func_beta_value < -eps) {
			roots->first = alpha;
			roots->second = alpha;
			roots->third = find_root(beta, find_right(beta, delta, coefs), coefs, eps);
			roots->root_num = 2;
		}
		
		else if(func_alpha_value > eps && func_beta_value < -eps) {
			roots->first = find_root(find_left(alpha, delta, coefs), alpha, coefs, eps);
			roots->second = find_root(alpha, beta, coefs, eps);
			roots->third = find_root(beta, find_right(beta, delta, coefs), coefs, eps);
			roots->root_num = 3;
		}
	
		else if(abs_val(func_alpha_value) < eps && abs_val(func_beta_value) < eps) {
			roots->first = (alpha + beta) / 2.0f;
			roots->root_num = 1;
		}

		else {
			printf("How did we get here?\n");
			printf("Disc: %lf, alpha: %lf, func_alpha_val: %lf, beta: %lf, func_beta_val: %lf\n", disc, alpha, func_alpha_value, beta, func_beta_value);
		}

	}
	else {
		if (coefs->d == 0) {
			roots->first = 0;
			roots->root_num = 1;
		}
		else if (coefs->d > 0) {
			roots->first = find_root(find_left(0.0f, delta, coefs), 0.0f, coefs, eps);
			roots->root_num = 1;
		}
		else {
			roots->first = find_root(find_right(0.0f, delta, coefs), 0.0f, coefs, eps);
			roots->root_num = 1;
		}
	}
}


int32_t main(int32_t argc, char** argv) {
	coefs_t coefs[1]; 
	roots_t roots[1];
	double eps, delta;

	eps = atof(*(argv + 1));
	delta = atof(*(argv + 2));
	coefs->a = 1;
	coefs->b = atof(*(argv + 3));
	coefs->c = atof(*(argv + 4));
	coefs->d = atof(*(argv + 5));

	toNaN.i = 0x7FF8000000000000;

	calc_roots(coefs, eps, delta, roots);

	printf("Roots: %hhd\n", roots->root_num); //NaN != NaN so I hate my life

	printf("-----------Roots value-----------\n");

	printf("First root: %lf\nSecond root: %lf\nThird root: %lf\n", roots->first, roots->second, roots->third);

	printf("-----Function absolute value-----\n");

	printf("First root: %lf\nSecond root: %lf\nThird root: %lf\n", abs_val(calc_func(roots->first, coefs)), abs_val(calc_func(roots->second, coefs)), abs_val(calc_func(roots->third, coefs)));

	return 0;
}

