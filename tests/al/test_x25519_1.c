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
	bn_int512_t p, a, b;
	FSYMBOL(bn_int512_strtonum)(p, X25519_P, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(a, X25519_A, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(b, X25519_B, NULL, 10);

	print("P: ", p);
	print_b64("P-base64: ", p);
	print_hex("P-bytes: ", p);

	print("A: ", a);
	print_b64("A-base64: ", a);
	print_hex("A-bytes: ", a);

	print("B: ", b);
	print_b64("B-base64: ", b);
	print_hex("B-bytes: ", b);

	bn_int512_t a_in, a_out, b_in, b_out, a_key, b_key;
	FSYMBOL(bn_int512_zero)(a_in);	
	FSYMBOL(bn_int512_zero)(a_out);

	/* rfc-7748-6.1 */

	FSYMBOL(bn_int512_strtonum)(a_in,
		"19076158533740022697853188432810029468508100820210985396154491514718125885303"
		, NULL, 10);
	FSYMBOL(bn_int512_strtonum)(b_in,
		"106690238676031959364154629127743819238817784511314751294525304279539011595101"
		, NULL, 10);

	print("a_in: ", a_in);
	print_b64("a_in-base64: ", a_in);
	print_hex("a_in-bytes: ", a_in);

	print("b_in: ", b_in);
	print_b64("b_in-base64: ", b_in);
	print_hex("b_in-bytes: ", b_in);

	FSYMBOL(x25519_clamp_key)(a_in);
	FSYMBOL(x25519_clamp_key)(b_in);
	FSYMBOL(x25519_scalar_mul)(a_in, b, p, a, a_out);
	FSYMBOL(x25519_scalar_mul)(b_in, b, p, a, b_out);

	print("a_out: ", a_out);
	print_b64("a_out-base64: ", a_out);
	print_hex("a_out-bytes: ", a_out);

	print("b_out: ", b_out);
	print_b64("b_out-base64: ", b_out);
	print_hex("b_out-bytes: ", b_out);

	FSYMBOL(x25519_base_mask)(a_out);
	FSYMBOL(x25519_base_mask)(b_out);
	FSYMBOL(x25519_shared_key)(a_in, b_out, p, a, a_key);
	FSYMBOL(x25519_shared_key)(b_in, a_out, p, a, b_key);

	print("a_key: ", a_key);
	print_b64("a_key-base64: ", a_key);
	print_hex("a_key-bytes: ", a_key);

	print("b_key: ", b_key);
	print_b64("b_key-base64: ", b_key);
	print_hex("b_key-bytes: ", b_key);

	return 0;
}
