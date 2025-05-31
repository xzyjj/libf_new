#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_1024.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/x448.h>


static void print(const char *s, const bn_int1024_t v) {
	char buf[1024];
	FSYMBOL(bn_int1024_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

static void print_b64(const char *s, const bn_int1024_t v) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)v, 56, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex(const char *s, const bn_int1024_t v) {
	char buf[1024];
	for (int32_t i = 0; i < 56; i++) {
		snprintf(&buf[i * 2], 1024, "%02x", ((uint8 *)v)[i]);
	}
	printf("%s%s\n", s, buf);
}

static void hex2bn(bn_int1024_t t, const char *s, uint32 len) {
	char buf[1024];
	uint32 n = sizeof(buf);
	FSYMBOL(base16_dec)(s, len, buf, &n);
	FSYMBOL(bn_int1024_zero)(t);
	XSYMBOL(memcpy)(t, buf, sizeof(buf) - n);
}

int main(int argc, char *argv[]) {
	clock_t start, end;
	double time;

	X448_ECDH_NEW(a_ctx);
	X448_ECDH_NEW(b_ctx);

	FSYMBOL(x448_ecdh_init)(&a_ctx);
	FSYMBOL(x448_ecdh_init)(&b_ctx);

	print("P: ", a_ctx.p);
	print("A: ", a_ctx.a);
	print("B: ", a_ctx.b);

/*	const char *a_pri = "9a8f4925d1519f5775cf46b04b5800d4ee9ee8bae8bc5565d498c28dd9c9baf574a9419744897391006382a6f127ab1d9ac2d8c0a598726b";
	const char *b_pri = "1c306a7ac2a0e2e0990b294470cba339e6453772b075811d8fad0d1d6927c120bb5ee8972b0d3e21374c9c921b09d1b0366f10b65173992d";
*/
	const char *a_pri = argv[1];
	const char *b_pri = argv[2];

	bn_uint1024_t pri, pub, key;

	/* A */
	hex2bn(pri, a_pri, XSYMBOL(strlen)(a_pri));
	print("a_priS: ", pri);
	print_hex("a_priS: ", pri);
	print_b64("a_priS: ", pri);

	start = clock();
	FSYMBOL(x448_ecdh_private)(&a_ctx, (uint8 *)pri);
	FSYMBOL(x448_ecdh_public)(&a_ctx);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f\n", time);

	FSYMBOL(bn_int1024_zero)(pri);
	XSYMBOL(memcpy)(pri, &X448_PRI(&a_ctx, 0), X448_LEN);
	print("a_pri: ", pri);
	print_hex("a_pri: ", pri);
	print_b64("a_pri: ", pri);

	FSYMBOL(bn_int1024_zero)(pub);
	XSYMBOL(memcpy)(pub, &X448_PUB(&a_ctx, 0), X448_LEN);
	print("a_pub: ", pub);
	print_hex("a_pub: ", pub);
	print_b64("a_pub: ", pub);

	/* B */
	hex2bn(pri, b_pri, XSYMBOL(strlen)(b_pri));
	print("b_priS: ", pri);
	print_hex("b_priS: ", pri);
	print_b64("b_priS: ", pri);

	FSYMBOL(x448_ecdh_private)(&b_ctx, (uint8 *)pri);
	FSYMBOL(x448_ecdh_public)(&b_ctx);

	FSYMBOL(bn_int1024_zero)(pri);
	XSYMBOL(memcpy)(pri, &X448_PRI(&b_ctx, 0), X448_LEN);
	print("b_pri: ", pri);
	print_hex("b_pri: ", pri);
	print_b64("b_pri: ", pri);

	FSYMBOL(bn_int1024_zero)(pub);
	XSYMBOL(memcpy)(pub, &X448_PUB(&b_ctx, 0), X448_LEN);
	print("b_pub: ", pub);
	print_hex("b_pub: ", pub);
	print_b64("b_pub: ", pub);

	/* key */
	FSYMBOL(x448_ecdh_exchange)(&a_ctx,
		(uint8 *)&X448_PUB(&b_ctx, 0));

	FSYMBOL(x448_ecdh_exchange)(&b_ctx,
		(uint8 *)&X448_PUB(&a_ctx, 0));

	FSYMBOL(bn_int1024_zero)(key);
	XSYMBOL(memcpy)(key, &X448_KEY(&a_ctx, 0), X448_LEN);
	print("a_key: ", key);
	print_hex("a_key: ", key);
	print_b64("a_key: ", key);

	FSYMBOL(bn_int1024_zero)(key);
	XSYMBOL(memcpy)(key, &X448_KEY(&b_ctx, 0), X448_LEN);
	print("b_key: ", key);
	print_hex("b_key: ", key);
	print_b64("b_key: ", key);

	return 0;
}
