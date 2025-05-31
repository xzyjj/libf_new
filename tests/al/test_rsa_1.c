#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/base64.h>
#include <libf/al/rsa.h>


static void print(const char *s, const bn_int8192_t v) {
	char buf[1 << 17];
	FSYMBOL(bn_int8192_numtostr)(buf, v);
	printf("%s%s\n", s, buf);
}

static void print_b64(const char *s, const bn_int8192_t v) {
	char buf[1 << 17];
	uint32 n = sizeof(buf);
	FSYMBOL(base64_enc)((char *)v, BN_8192_SIZE, buf, &n);
	buf[sizeof(buf) - n] = '\0';
	printf("%s%s\n", s, buf);
}

static void print_hex(const char *s, const bn_int8192_t v) {
	char buf[1 << 17];
	for (int32_t i = 0; i < 128; i++) {
		snprintf(&buf[i * 2], BN_8192_SIZE, "%02x", ((uint8 *)v)[i]);
	}
	printf("%s%s\n", s, buf);
}

int main(int argc, char *argv[]) {
	bn_int8192_t p, q, e, d, n, m;
	FSYMBOL(bn_int8192_zero)(p);
	FSYMBOL(bn_int8192_zero)(q);

	FSYMBOL(bn_int8192_zero)(m);
	m[0] = 123;

	FSYMBOL(bn_int8192_strtonum)(p,
/*		"3347807169895689878604416984821269081770479498371376856891"
		"2431388982883793878002287614711652531743087737814467999489"
*/
		argv[1]
		, NULL, 10);
	FSYMBOL(bn_int8192_strtonum)(q,
/*		"3674604366679959042824463379962795263227915816434308764267"
		"6032283815739666511279233373417143396810270092798736308917"
*/
		argv[2]
		, NULL, 10);

	FSYMBOL(rsa4096_genkey)(p, q, e, d, n);

	print("p: ", p);
	print("q: ", q);

	print("e: ", e);
	print_b64("e: ", e);

	print("d: ", d);
	print_b64("d: ", d);

	print("n: ", n);
	print_b64("n: ", n);

	print("m: ", m);
	print_b64("n: ", n);

	FSYMBOL(rsa4096_encrypt)(e, n, m);
	print("m: ", m);

	FSYMBOL(rsa4096_decrypt)(d, n, m);
	print("m: ", m);

	return 0;
}
