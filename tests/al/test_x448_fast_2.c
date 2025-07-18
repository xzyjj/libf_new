#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/x448_fast.h>


static void print_b64u8(const char *s, const uint8 *p) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)p, X448_LEN, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex2u8(const char *s, const uint8 *p) {
	char buf[1024];
	for (int32 i = 0; i < X448_LEN; i++)
		snprintf(&buf[i * 2], sizeof(buf), "%02x", p[i]);
	printf("%s%s\n", s, buf);
}

static void hex2u8(uint8 *p, const char *s, uint32 len) {
	uint32 n = X448_LEN;
	XSYMBOL(memset)(p, 0, X448_LEN);
	FSYMBOL(base16_dec)(s, len, (char *)p, &n);
}

int main(int argc, char *argv[]) {
	const char *s = NULL;

	s = argv[1];
	uint8 a_pri[X448_LEN], a_pub[X448_LEN], a_B[X448_LEN];
	hex2u8(a_pri, s, XSYMBOL(strlen)(s));
	print_hex2u8("a_priS: ", a_pri);
	print_b64u8("a_priS: ", a_pri);

	s = argv[2];
	hex2u8(a_B, s, XSYMBOL(strlen)(s));
	print_hex2u8("a_B: ", a_B);
	print_b64u8("a_B: ", a_B);

	FSYMBOL(x448_fast_private_key)(a_pri);
	print_hex2u8("a_pri: ", a_pri);
	print_b64u8("a_pri: ", a_pri);

	FSYMBOL(x448_fast_scalar_mul)((uint32 *)a_pri, (uint32 *)a_B,
		(uint32 *)a_pub);

	print_hex2u8("a_pub: ", a_pub);
	print_b64u8("a_pub: ", a_pub);

	return 0;
}
