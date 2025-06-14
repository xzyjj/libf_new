#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/bn_512.h>
#include <libf/al/base64.h>
#include <libf/al/base16.h>
#include <libf/al/ed25519_ref.h>


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

	ED25519_SIGN_NEW(a_ctx);

	FSYMBOL(ed25519_sign_init)(&a_ctx);

	print("P: ", a_ctx.p);
	print("D: ", a_ctx.d);
	print("Q: ", a_ctx.q);

	print("B.x: ", a_ctx.xyz1.x);
	print("B.y: ", a_ctx.xyz1.y);
	print("B.z: ", a_ctx.xyz1.z);
	print("B.t: ", a_ctx.xyz1.t);

/*	const char *a_pri = "77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a"; */

	const char *a_pri = argv[1];
	const char *a_mesg = argv[2];
	const char *b_mesg = argv[3];

	bn_uint512_t pri, pub, sign;

	/* signature */
	hex2bn(pri, a_pri, XSYMBOL(strlen)(a_pri));
	print("a_priS: ", pri);
	print_hex("a_priS: ", pri);
	print_b64("a_priS: ", pri);

	FSYMBOL(ed25519_sign_private)(&a_ctx, (uint8 *)pri);
	FSYMBOL(ed25519_sign_public)(&a_ctx);

	FSYMBOL(bn_int512_zero)(pri);
	XSYMBOL(memcpy)(pri, &ED25519_PRI(&a_ctx, 0), ED25519_LEN);
	print("pri: ", pri);
	print_hex("pri: ", pri);
	print_b64("pri: ", pri);

	FSYMBOL(bn_int512_zero)(pub);
	XSYMBOL(memcpy)(pub, &ED25519_PUB(&a_ctx, 0), ED25519_LEN);
	print("pub: ", pub);
	print_hex("pub: ", pub);
	print_b64("pub: ", pub);

	start = clock();
	FSYMBOL(ed25519_sign)(&a_ctx, (uint8 *)a_mesg, XSYMBOL(strlen)(a_mesg));
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("time: %.6f\n", time);

	FSYMBOL(bn_int512_zero)(sign);
	XSYMBOL(memcpy)(sign, &ED25519_SIGN_1(&a_ctx, 0), ED25519_LEN);
	print("sign1: ", sign);
	print_hex("sign1: ", sign);
	print_b64("sign1: ", sign);

	FSYMBOL(bn_int512_zero)(sign);
	XSYMBOL(memcpy)(sign, &ED25519_SIGN_2(&a_ctx, 0), ED25519_LEN);
	print("sign2: ", sign);
	print_hex("sign2: ", sign);
	print_b64("sign2: ", sign);

	FSYMBOL(bn_int512_zero)(sign);
	XSYMBOL(memcpy)(sign, &ED25519_SIGN_1(&a_ctx, 0), ED25519_LEN);
	XSYMBOL(memcpy)(&sign[8], &ED25519_SIGN_2(&a_ctx, 0), ED25519_LEN);
	print("sign: ", sign);

	/* verification */
	printf("'%s':'%s' = %s\n", a_mesg, a_mesg,
		FSYMBOL(ed25519_sign_verify)(&a_ctx,
			(uint8 *)&ED25519_PUB(&a_ctx, 0), (uint8 *)sign,
			(uint8 *)a_mesg, XSYMBOL(strlen)(a_mesg))
		? "No" : "Yes");

	printf("'%s':'%s' = %s\n", a_mesg, b_mesg,
		FSYMBOL(ed25519_sign_verify)(&a_ctx,
			(uint8 *)&ED25519_PUB(&a_ctx, 0), (uint8 *)sign,
			(uint8 *)b_mesg, XSYMBOL(strlen)(b_mesg))
		? "No" : "Yes");

	return 0;
}
