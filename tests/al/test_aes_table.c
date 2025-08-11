#include <stdio.h>
#include <libf/sl/xstdint.h>


static uint8 aes_table[256];

static uint8 _aes_xtime(uint8 n) {
	return ((n << 1) ^ (((n >> 7) & 1) * 0x1b));
}

static uint8 _aes_invmultiply(uint8 x, uint8 y) {
	uint8 tmp, a, b, c, d;
	a = _aes_xtime(x);
	b = _aes_xtime(a);
	c = _aes_xtime(b);
	d = _aes_xtime(c);

	tmp = (y & 1) * x;
	tmp ^= ((y >> 1) & 1) * a;
	tmp ^= ((y >> 2) & 1) * b;
	tmp ^= ((y >> 3) & 1) * c;
	tmp ^= ((y >> 4) & 1) * d;

	return tmp;
}

static void aes_xtime_table_init(void) {
	for (int32 i = 0; i < 256; i++) {
		aes_table[i] = _aes_xtime(i);
	}
}

static void aes_invmultiply_table_init(uint8 n) {
	for (int32 i = 0; i < 256; i++) {
		aes_table[i] = _aes_invmultiply(i, n);
	}
}

static void aes_table_print(char *s1, char *s2) {
	printf("/* %s */\n", s1);
	printf("static const uint8 aes_%s_table = {\n\t", s2);
	for (int32 i = 0; i < 256; ) {
		printf("0x%02x, ", aes_table[i++]);
		if (!(i % 12))
			printf("\n\t");
	}
	printf("};\n\n");
}

int main(void) {
	aes_xtime_table_init();
	aes_table_print("aes xtime", "xtime");

	aes_invmultiply_table_init(0x09);
	aes_table_print("aes invmul 0x09", "invmul_0");
	aes_invmultiply_table_init(0x0b);
	aes_table_print("aes invmul 0x0b", "invmul_1");
	aes_invmultiply_table_init(0x0d);
	aes_table_print("aes invmul 0x0d", "invmul_2");
	aes_invmultiply_table_init(0x0e);
	aes_table_print("aes invmul 0x0e", "invmul_3");

	return 0;
}
