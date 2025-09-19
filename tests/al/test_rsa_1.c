#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/rsa.h>
#include <libf/al/bn_8193.h>


static void _bn_print(const bn_8193_t n) {
	bn_8193_t q, r;

	FSYMBOL(bn_u8193_div1)(q, r, n, 1000000000);
	if (FSYMBOL(bn_8193_bits)(q)) {
		_bn_print(q);
	} else if (n[BN_8193_SIG]) {
		printf("-");
	}
	printf("%09u,", r[0]);
}

void bn_print(const char *s, const bn_8193_t n) {
	printf("%s", s);
	_bn_print(n);
	printf("\n");
}

int32 main(int argc, char *argv[]) {
	clock_t start, end;
	double time;
	const char *a = argv[1];
	const char *b = argv[2];

	bn_8193_t p, q, e, d, n, m, dq, dp, qinv;
	FSYMBOL(bn_8193_init)(p);
	FSYMBOL(bn_8193_init)(q);
	FSYMBOL(bn_8193_set1)(m, 123);

	FSYMBOL(bn_8193_strtonum)(p, a, NULL, 10);
	FSYMBOL(bn_8193_strtonum)(q, b, NULL, 10);

	FSYMBOL(rsa4096_genkey)(p, q, e, d, n);

	bn_print("p: ", p);
	bn_print("q: ", q);

	bn_print("e: ", e);
	bn_print("d: ", d);

	bn_print("n: ", n);

	FSYMBOL(rsa4096_gencrt)(p, q, d, dp, dq, qinv);

	bn_print("dq: ", dq);
	bn_print("dp: ", dp);
	bn_print("qinv: ", qinv);

	bn_print("m: ", m);

	start = clock();
	FSYMBOL(rsa4096_encrypt)(e, n, m);
	bn_print("m: ", m);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("en: %.6fs\n", time);

	start = clock();
	FSYMBOL(rsa4096_decrypt)(d, n, m);
	end = clock();
	bn_print("m: ", m);
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("de: %.6fs\n", time);

	FSYMBOL(rsa4096_encrypt)(e, n, m);

	start = clock();
	FSYMBOL(rsa4096_crt_decrypt)(q, p, dp, dq, qinv, m);
	end = clock();
	bn_print("-m: ", m);
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("crt-de: %.6fs\n", time);

	return 0;
}
