#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/x25519_ref.h>


static void print_b64u8(const char *s, const uint8 *p) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)p, X25519_LEN, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex2u8(const char *s, const uint8 *p) {
	char buf[1024];
	for (int32 i = 0; i < X25519_LEN; i++)
		snprintf(&buf[i * 2], sizeof(buf), "%02x", p[i]);
	printf("%s%s\n", s, buf);
}

static void hex2u8(uint8 *p, const char *s, uint32 len) {
	uint32 n = X25519_LEN;
	XSYMBOL(memset)(p, 0, X25519_LEN);
	FSYMBOL(base16_dec)(s, len, (char *)p, &n);
}

int main(int argc, char *argv[]) {
	const char *s = NULL;

	/* A */
	s = argv[1];
	uint8 a_pri[X25519_LEN], a_pub[X25519_LEN], a_key[X25519_LEN];
	hex2u8(a_pri, s, XSYMBOL(strlen)(s));
	print_hex2u8("a_priS: ", a_pri);
	print_b64u8("a_priS: ", a_pri);

	/* B */
	s = argv[2];
	uint8 b_pri[X25519_LEN], b_pub[X25519_LEN], b_key[X25519_LEN];
	hex2u8(b_pri, s, XSYMBOL(strlen)(s));
	print_hex2u8("b_priS: ", b_pri);
	print_b64u8("b_priS: ", b_pri);

	/* A */
	FSYMBOL(x25519_private_key)(a_pri);
	print_hex2u8("a_pri: ", a_pri);
	print_b64u8("a_pri: ", a_pri);

	/* B */
	FSYMBOL(x25519_private_key)(b_pri);
	print_hex2u8("b_pri: ", b_pri);
	print_b64u8("b_pri: ", b_pri);

	/* A */
	FSYMBOL(x25519_public_key)(a_pri, a_pub);
	print_hex2u8("a_pub: ", a_pub);
	print_b64u8("a_pub: ", a_pub);

	/* B */
	FSYMBOL(x25519_public_key)(b_pri, b_pub);
	print_hex2u8("b_pub: ", b_pub);
	print_b64u8("b_pub: ", b_pub);

	/* A */
	FSYMBOL(x25519_shared_key)(a_pri, b_pub, a_key);
	print_hex2u8("a_key: ", a_key);
	print_b64u8("a_key: ", a_key);

	/* B */
	FSYMBOL(x25519_shared_key)(b_pri, a_pub, b_key);
	print_hex2u8("b_key: ", b_key);
	print_b64u8("b_key: ", b_key);

	return 0;
}
