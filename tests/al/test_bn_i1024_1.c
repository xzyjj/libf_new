#include <stdio.h>
#include <libf/al/bn_1024.h>


void print(char *s, bn_int1024_t v) {
	char buf[1 << 16];
	FSYMBOL(bn_int1024_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

int main(void) {
	bn_int1024_t a, b, r, quo, rem;

	FSYMBOL(bn_int1024_strtonum)(a, "1234567890123456789", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "123456789", NULL, 10);

	FSYMBOL(bn_int1024_add)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("add: ", r);

	FSYMBOL(bn_int1024_sub)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("sub: ", r);

	FSYMBOL(bn_int1024_sub)(r, b, a);
	print("b: ", b);
	print("a: ", a);
	print("sub: ", r);

	FSYMBOL(bn_int1024_mul)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("mul: ", r);

	FSYMBOL(bn_int1024_div)(quo, rem, a, b);
	print("a: ", a);
	print("b: ", b);
	print("div-quo: ", quo);
	print("div-rem: ", rem);

	FSYMBOL(bn_int1024_strtonum)(a, "100000000000000000", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "123456789", NULL, 10);

	FSYMBOL(bn_int1024_div)(quo, rem, a, b);
	print("a: ", a);
	print("b: ", b);
	print("div-quo: ", quo);
	print("div-rem: ", rem);

	FSYMBOL(bn_int1024_strtonum)(a, "100000000000000000", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "0", NULL, 10);

	FSYMBOL(bn_int1024_div)(quo, rem, a, b);
	print("a: ", a);
	print("b: ", b);
	print("div-quo: ", quo);
	print("div-rem: ", rem);

	FSYMBOL(bn_int1024_strtonum)(a, "0x10000000000000000000", NULL, 16);
	FSYMBOL(bn_int1024_strtonum)(b, "0x120000000000000000", NULL, 16);

	FSYMBOL(bn_int1024_or)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("or: ", r);

	print("r: ", r);
	FSYMBOL(bn_int1024_and)(r, r, b);
	print("b: ", b);
	print("and: ", r);

	FSYMBOL(bn_int1024_not)(r, b);
	print("b: ", b);
	print("not: ", r);

	print("r: ", r);
	FSYMBOL(bn_int1024_not)(r, r);
	print("not: ", r);

	FSYMBOL(bn_int1024_xor)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("xor: ", r);

	print("r: ", r);
	FSYMBOL(bn_int1024_lsh)(r);
	print("lsh: ", r);

	print("r: ", r);
	FSYMBOL(bn_int1024_rsh)(r);
	print("rsh: ", r);

	FSYMBOL(bn_int1024_strtonum)(a, "-100000000000000000", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "7", NULL, 10);

	FSYMBOL(bn_int1024_div)(quo, rem, a, b);
	print("a: ", a);
	print("b: ", b);
	print("div-quo: ", quo);
	print("div-rem: ", rem);

	FSYMBOL(bn_int1024_divmod)(quo, rem, a, b);
	print("a: ", a);
	print("b: ", b);
	print("divmod-quo: ", quo);
	print("divmod-rem: ", rem);

	FSYMBOL(bn_int1024_strtonum)(a, "-10", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "-7", NULL, 10);
	print("a: ", a);
	print("b: ", b);
	printf("cmp: %d\n", FSYMBOL(bn_int1024_cmp)(a, b));

	FSYMBOL(bn_int1024_strtonum)(a, "10", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "7", NULL, 10);
	print("a: ", a);
	print("b: ", b);
	printf("cmp: %d\n", FSYMBOL(bn_int1024_cmp)(a, b));

	FSYMBOL(bn_int1024_strtonum)(a, "-10", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "7", NULL, 10);
	print("a: ", a);
	print("b: ", b);
	printf("cmp: %d\n", FSYMBOL(bn_int1024_cmp)(a, b));

	FSYMBOL(bn_int1024_strtonum)(a, "10", NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, "7", NULL, 10);
	FSYMBOL(bn_int1024_mul)(r, a, b);
	print("a: ", a);
	print("b: ", b);
	print("mul: ", r);

	return 0;
}
