#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/x25519_fast.h>


static void print(const char *s, const bn_int512_t v) {
	char buf[1024];
	FSYMBOL(bn_int512_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

static void print_b64(const char *s, const bn_int512_t v) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)v, 32, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex(const char *s, const bn_int512_t v) {
	char buf[1024];
	for (int32_t i = 0; i < 32; i++) {
		snprintf(&buf[i * 2], 512, "%02x", ((uint8 *)v)[i]);
	}
	printf("%s%s\n", s, buf);
}

static void hex2bn(bn_int512_t t, const char *s, uint32 len) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base16_dec)(s, len, buf, &n);
	FSYMBOL(bn_int512_zero)(t);
	XSYMBOL(memcpy)(t, buf, sizeof(buf) - n);
}

int main(int argc, char *argv[]) {
	clock_t start, end;
	double time;

	X25519_FAST_ECDH_NEW(a_ctx);
	X25519_FAST_ECDH_NEW(b_ctx);

	FSYMBOL(x25519_fast_ecdh_init)(&a_ctx);
	FSYMBOL(x25519_fast_ecdh_init)(&b_ctx);

/*	const char *a_pri = "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a";
	const char *b_pri = "5dab087e624a8a4b79e17f8b83800ee66f3bb1292618b6fd1c2f8b27ff88e0eb";
*/
	const char *a_pri = argv[1];
	const char *b_pri = argv[2];

	bn_uint512_t pri, pub, key;

	/* A */
	hex2bn(pri, a_pri, XSYMBOL(strlen)(a_pri));
	print("a_priS: ", pri);
	print_hex("a_priS: ", pri);
	print_b64("a_priS: ", pri);

	start = clock();
	FSYMBOL(x25519_fast_ecdh_private)(&a_ctx, (uint8 *)pri);
	FSYMBOL(x25519_fast_ecdh_public)(&a_ctx);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f\n", time);

	FSYMBOL(bn_int512_zero)(pri);
	XSYMBOL(memcpy)(pri, &X25519_FAST_PRI(&a_ctx, 0), X25519_FAST_LEN);
	print("a_pri: ", pri);
	print_hex("a_pri: ", pri);
	print_b64("a_pri: ", pri);

	FSYMBOL(bn_int512_zero)(pub);
	XSYMBOL(memcpy)(pub, &X25519_FAST_PUB(&a_ctx, 0), X25519_FAST_LEN);
	print("a_pub: ", pub);
	print_hex("a_pub: ", pub);
	print_b64("a_pub: ", pub);

	/* B */
	hex2bn(pri, b_pri, XSYMBOL(strlen)(b_pri));
	print("b_priS: ", pri);
	print_hex("b_priS: ", pri);
	print_b64("b_priS: ", pri);

	FSYMBOL(x25519_fast_ecdh_private)(&b_ctx, (uint8 *)pri);
	FSYMBOL(x25519_fast_ecdh_public)(&b_ctx);

	FSYMBOL(bn_int512_zero)(pri);
	XSYMBOL(memcpy)(pri, &X25519_FAST_PRI(&b_ctx, 0), X25519_FAST_LEN);
	print("b_pri: ", pri);
	print_hex("b_pri: ", pri);
	print_b64("b_pri: ", pri);

	FSYMBOL(bn_int512_zero)(pub);
	XSYMBOL(memcpy)(pub, &X25519_FAST_PUB(&b_ctx, 0), X25519_FAST_LEN);
	print("b_pub: ", pub);
	print_hex("b_pub: ", pub);
	print_b64("b_pub: ", pub);

	/* key */
	FSYMBOL(x25519_fast_ecdh_exchange)(&a_ctx,
		(uint8 *)&X25519_FAST_PUB(&b_ctx, 0));

	FSYMBOL(x25519_fast_ecdh_exchange)(&b_ctx,
		(uint8 *)&X25519_FAST_PUB(&a_ctx, 0));

	FSYMBOL(bn_int512_zero)(key);
	XSYMBOL(memcpy)(key, &X25519_FAST_KEY(&a_ctx, 0), X25519_FAST_LEN);
	print("a_key: ", key);
	print_hex("a_key: ", key);
	print_b64("a_key: ", key);

	FSYMBOL(bn_int512_zero)(key);
	XSYMBOL(memcpy)(key, &X25519_FAST_KEY(&b_ctx, 0), X25519_FAST_LEN);
	print("b_key: ", key);
	print_hex("b_key: ", key);
	print_b64("b_key: ", key);

	return 0;
}
