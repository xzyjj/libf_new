#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_4096.h>


int extended_gcd(int a, int b, int *x, int *y) {
	if (b == 0) {
		*x = 1;
		*y = 0;
		return a;
	}

	int x1, y1;
	int gcd = extended_gcd(b, a % b, &x1, &y1);

	*x = y1;
	*y = x1 - ((a / b) * y1);

	printf("g:%d\n", gcd);
	printf("gx:%d\n", *x);
	printf("gy:%d\n", *y);

	return gcd;
}

int mod_inverse(int a, int m) {
	int x, y;
	int gcd = extended_gcd(a, m, &x, &y);

	printf("%d %d\n", x, m);

	if (gcd != 1) {
		return -1;
	} else {
		return (x % m + m) % m;
	}
}

char buf[1 << 16];
void print(bn_int4096_t a, char *s) {
	FSYMBOL(bn_int4096_numtostr)(a, buf);
	printf("%s%s\n", s, buf);
}

void bn_extended_gcd(bn_int4096_t a, bn_int4096_t b,
		bn_int4096_t x, bn_int4096_t y, bn_int4096_t r) {
	bn_int4096_t _a, _b, _x, _y, _quo, _rem;

	if (!FSYMBOL(bn_int4096_cmp_1)(b, 0)) {
		FSYMBOL(bn_int4096_zero)(x);
		FSYMBOL(bn_int4096_zero)(y);
		x[0] = 1;
		y[0] = 0;
		FSYMBOL(bn_int4096_move)(r, a);
		return;
	}

	FSYMBOL(bn_int4096_move)(_b, b);
	FSYMBOL(bn_int4096_div)(_quo, _rem, a, b);
	bn_extended_gcd(_b, _rem, _x, _y, r);

	FSYMBOL(bn_int4096_move)(x, _y);

	FSYMBOL(bn_int4096_div)(_quo, _rem, a, b);
	FSYMBOL(bn_int4096_mul)(_quo, _quo, _y);
	FSYMBOL(bn_int4096_sub)(y, _x, _quo);

	print(r, "gcd: ");
	print(x, "gx: ");
	print(y, "gy: ");
}

int32 bn_mod_inverse(bn_int4096_t a, bn_int4096_t m, bn_int4096_t r) {
	bn_int4096_t x, y, _quo, _rem;
	bn_extended_gcd(a, m, x, y, r);

	print(x, "x: ");
	print(r, "r: ");

	if (FSYMBOL(bn_int4096_cmp_1)(r, 1)) {
		return -1;
	} else {
		FSYMBOL(bn_int4096_div)(_quo, _rem, x, m);
		FSYMBOL(bn_int4096_add)(_rem, _rem, m);
		FSYMBOL(bn_int4096_div)(_quo, r, _rem, m);
	}

	return 0;
}

int main(void) {
	int a = 3, m = 11;
	int inv = mod_inverse(a, m);

	if (inv == -1) {
		printf("%d mod %d has no inverse\n", a, m);
	} else {
		printf("%d mod %d is %d\n", a, m, inv);
		printf("(%d * %d) mod %d = %d\n", a, inv, m, (a * inv) % m);
	}


	bn_int4096_t x, y, r;
	FSYMBOL(bn_int4096_zero)(x);
	FSYMBOL(bn_int4096_zero)(y);

/*	for (int i = 0; i < 128; i++)
		x[i] = 0xffffffff;*/

	x[0] = 3;
	y[0] = 11;
	print(x, "- x: ");
	print(y, "- y: ");

	bn_mod_inverse(x, y, r);

	print(r, "- r: ");

	return 0;
}
