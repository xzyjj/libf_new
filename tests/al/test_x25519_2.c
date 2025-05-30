#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/x25519.h>


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
	X25519_ECDH_NEW(a_ctx);
	FSYMBOL(x25519_ecdh_init)(&a_ctx);

	print("P: ", a_ctx.p);
	print("A: ", a_ctx.a);

	const char *a_pri = argv[1];
	const char *a_U = argv[2];

	hex2bn(a_ctx.b, a_U, XSYMBOL(strlen)(a_U));
	FSYMBOL(x25519_base_mask)(a_ctx.b);
	print("B: ", a_ctx.b);
	print_hex("B: ", a_ctx.b);
	print_b64("B: ", a_ctx.b);

	bn_uint512_t pri, pub;

	hex2bn(pri, a_pri, XSYMBOL(strlen)(a_pri));
	print("priS: ", pri);
	print_hex("priS: ", pri);
	print_b64("priS: ", pri);

	FSYMBOL(x25519_ecdh_private)(&a_ctx, (uint8 *)pri);
	FSYMBOL(x25519_ecdh_public)(&a_ctx);

	FSYMBOL(bn_int512_zero)(pri);
	XSYMBOL(memcpy)(pri, &X25519_PRI(&a_ctx, 0), X25519_LEN);
	print("pri: ", pri);
	print_hex("pri: ", pri);
	print_b64("pri: ", pri);

	FSYMBOL(bn_int512_zero)(pub);
	XSYMBOL(memcpy)(pub, &X25519_PUB(&a_ctx, 0), X25519_LEN);
	print("pub: ", pub);
	print_hex("pub: ", pub);
	print_b64("pub: ", pub);

	return 0;
}
