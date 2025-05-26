#include <stdio.h>
#include <libf/al/bn_1024.h>
#include <libf/al/x25519.h>


void print(char *s, bn_int1024_t v) {
	char buf[1 << 16];
	FSYMBOL(bn_int1024_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

int main(void) {
	bn_int1024_t p, a, b;
	FSYMBOL(bn_int1024_strtonum)(p, X25519_P, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(a, X25519_A, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b, X25519_B, NULL, 10);

	print("P: ", p);
	print("A: ", a);
	print("B: ", b);

	bn_int1024_t a_in, a_out, b_in, b_out, a_key, b_key;
	FSYMBOL(bn_int1024_zero)(a_in);	
	FSYMBOL(bn_int1024_zero)(a_out);	

	FSYMBOL(bn_int1024_strtonum)(a_in,
		"129"
		, NULL, 10);
	FSYMBOL(bn_int1024_strtonum)(b_in,
		"127"
		, NULL, 10);

	FSYMBOL(x25519_clamp_key)(a_in);
	FSYMBOL(x25519_clamp_key)(b_in);
	FSYMBOL(x25519_scalarmult)(a_in, b, p, a, a_out);
	FSYMBOL(x25519_scalarmult)(b_in, b, p, a, b_out);

	FSYMBOL(x25519_shared_key)(a_in, b_out, p, a, a_key);
	FSYMBOL(x25519_shared_key)(b_in, a_out, p, a, b_key);

	print("a_in: ", a_in);
	print("a_out: ", a_out);

	print("b_in: ", b_in);
	print("b_out: ", b_out);

	print("a_key: ", a_key);
	print("b_key: ", b_key);

	return 0;
}
