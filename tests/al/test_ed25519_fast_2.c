#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/base16.h>
#include <libf/al/base64.h>
#include <libf/al/ed25519_fast.h>


static void print_b64u8(const char *s, const uint8 *p) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)p, ED25519_LEN, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex2u8(const char *s, const uint8 *p) {
	char buf[1024];
	for (int32 i = 0; i < ED25519_LEN; i++)
		snprintf(&buf[i * 2], sizeof(buf), "%02x", p[i]);
	printf("%s%s\n", s, buf);
}

static void hex2u8(uint8 *p, const char *s, uint32 len) {
	uint32 n = ED25519_LEN;
	XSYMBOL(memset)(p, 0, ED25519_LEN);
	FSYMBOL(base16_dec)(s, len, (char *)p, &n);
}

int main(int argc, char *argv[]) {
	const char *s = NULL, *a = NULL, *b = NULL;

	a = argv[2];
	b = argv[3];
	uint8 a_sign[ED25519_SIGN_LEN];

	s = argv[1];
	uint8 a_pri[ED25519_LEN], a_pub[ED25519_LEN], a_key[ED25519_LEN];
	hex2u8(a_pri, s, XSYMBOL(strlen)(s));
	print_hex2u8("a_priS: ", a_pri);
	print_b64u8("a_priS: ", a_pri);

	FSYMBOL(ed25519_fast_private_key)(a_pri, (uint32 *)a_pub, (uint32 *)a_key);
	print_hex2u8("a_pri: ", a_pub);
	print_b64u8("a_pri: ", a_pub);
	print_hex2u8("a_ran: ", a_key);
	print_b64u8("a_ran: ", a_key);

	FSYMBOL(ed25519_fast_public_key)(a_pri, a_pub);
	print_hex2u8("a_pub: ", a_pub);
	print_b64u8("a_pub: ", a_pub);

	FSYMBOL(ed25519_fast_sign)(a_pri, (uint8 *)a, XSYMBOL(strlen)(a), a_sign);
	print_hex2u8("a_sign-r: ", a_sign);
	print_hex2u8("a_sign-s: ", a_sign + ED25519_LEN);
	print_b64u8("a_sign-r: ", a_sign);
	print_b64u8("a_sign-s: ", a_sign + ED25519_LEN);

	int32 r = FSYMBOL(ed25519_fast_verify)(a_pub, a_sign,
		(uint8 *)a, XSYMBOL(strlen)(a));
	printf("'%s' : '%s' = %s\n", a, a, r ? "No" : "Yes");

	r = FSYMBOL(ed25519_fast_verify)(a_pub, a_sign,
		(uint8 *)b, XSYMBOL(strlen)(b));
	printf("'%s' : '%s' = %s\n", a, b, r ? "No" : "Yes");

	return 0;
}
