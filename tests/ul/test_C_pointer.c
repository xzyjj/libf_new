#include <stdio.h>


void foo0(double **a, double **b) {
	printf("foo0: a: %p, b: %p\n", a, b);

	double *t = *a;
	*a = *b;
	*b = t;
}

union double_i {
	double f;
	long i;
};

void foo1(double *a, double *b) {
	printf("foo1: a: %p, b: %p\n", a, b);

	double t = *a; /* The pointer object of 'a' */
	/* 't' is the object address of 'a' */

	*a = *b;
	*b = t;

	printf("t: %p\n", (long *)(*((long *)&t)));

	union double_i u = { t }; /* non-UB */
	printf("t: %p\n", (long *)u.i);
}

void foo2(long a, long b) { /* address is 8byte, long is also 8byte */
	printf("foo2: a: %p, b: %p\n", (long *)a, (long *)b);

	long t = *((long *)a);
	*((long *)a) = *((long *)b);
	*((long *)b) = t;
}

int main(void) {
	double _a = 1.2, _b = 2.1;

	double *a = &_a, *b = &_b;

	printf("_a: %p, _b: %p\n", &_a, &_b);

	printf("object: a: %p, b: %p\n", a, b);
	printf("itself: a: %p, b: %p\n", &a, &b);

	/* The address of 'a' & 'b' itself */

	/* Swap the object of 'a' and 'b' */
	foo0(&a, &b);
	foo1((double *)&a, (double *)&b);
	foo2((long)&a, (long)&b);	

	printf("%f %f\n", *a, *b);

	printf("a: %p, b: %p\n", a, b);

	return 0;
}

/* ---- output ----
* _a: 0x7fc8dd4e70, _b: 0x7fc8dd4e68
* object: a: 0x7fc8dd4e70, b: 0x7fc8dd4e68
* itself: a: 0x7fc8dd4e60, b: 0x7fc8dd4e58
* foo0: a: 0x7fc8dd4e60, b: 0x7fc8dd4e58
* foo1: a: 0x7fc8dd4e60, b: 0x7fc8dd4e58
* t: 0x7fc8dd4e68
* t: 0x7fc8dd4e68
* foo2: a: 0x7fc8dd4e60, b: 0x7fc8dd4e58
* 2.100000 1.200000
* a: 0x7fc8dd4e68, b: 0x7fc8dd4e70
*/
