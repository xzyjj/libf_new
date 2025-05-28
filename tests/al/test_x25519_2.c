#include <stdio.h>
#include <libf/al/bn_512.h>
#include <libf/al/base64.h>
#include <libf/al/x25519.h>


static void print(char *s, bn_int512_t v) {
	char buf[1 << 16];
	FSYMBOL(bn_int512_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

static void print_b64(char *s, bn_int512_t v) {
	char buf[1 << 16];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)v, 32, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex(char *s, bn_int512_t v) {
	char buf[1 << 16];
	for (int32_t i = 0; i < 32; i++) {
		snprintf(&buf[i * 2], 512, "%02x", ((uint8 *)v)[i]);
	}
	printf("%s%s\n", s, buf);
}

int main(void) {
	bn_int512_t p, a;
	FSYMBOL(bn_int512_strtonum)(p, X25519_P, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(a, X25519_A, NULL, 10);

	print("P: ", p);
	print_b64("P-base64: ", p);
	print_hex("P-bytes: ", p);

	print("A: ", a);
	print_b64("A-base64: ", a);
	print_hex("A-bytes: ", a);

	bn_int512_t k, b, o, C;
	char buf_k[512], buf_b[512], buf_C[512];

	int32 ret = 0;
	while ((ret = scanf("%s %s %s", buf_k, buf_b, buf_C)) >= 2) {
		FSYMBOL(bn_int512_strtonum)(k, buf_k, NULL, 0);
		FSYMBOL(bn_int512_strtonum)(b, buf_b, NULL, 0);
		FSYMBOL(bn_int512_zero)(C);
		if (ret > 2)
			FSYMBOL(bn_int512_strtonum)(C, buf_C, NULL, 0);

		FSYMBOL(x25519_clamp_key)(k);
		FSYMBOL(x25519_base_mask)(b);
		FSYMBOL(x25519_scalar_mul)(k, b, p, a, o);

		print("======\nk: ", k);
		print_b64("k-base64: ", k);
		print_hex("k-bytes: ", k);

		print("b: ", b);
		print_b64("b-base64: ", b);
		print_hex("b-bytes: ", b);

		print("o: ", o);
		print_b64("o-base64: ", o);
		print_hex("o-bytes: ", o);

		print("C: ", C);
		print_b64("C-base64: ", C);
		print_hex("C-bytes: ", C);
		if (FSYMBOL(bn_int512_cmp)(o, C)) {
			printf("o != C\n");
		} else {
			printf("o == C\n");
		}
	}

	return 0;
}
