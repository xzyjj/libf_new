#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/bn_4096.h>


int main(void) {
	bn_int4096_t a, b, r, rem;
	char buf[1 << 16];

	FSYMBOL(bn_int4096_zero)(a);
	a[0] = 10;
	a[BN_4096_SIG] = 1;

	FSYMBOL(bn_int4096_numtostr)(a, buf);
	printf("%s\n", buf);

	FSYMBOL(bn_int4096_zero)(b);
	b[0] = 50;

	FSYMBOL(bn_int4096_numtostr)(b, buf);
	printf("%s\n", buf);

	FSYMBOL(bn_int4096_mul)(r, a, b);
	FSYMBOL(bn_int4096_numtostr)(r, buf);
	printf("%s\n", buf);

	FSYMBOL(bn_int4096_div)(r, rem, a, b);
	FSYMBOL(bn_int4096_numtostr)(r, buf);
	printf("%s\n", buf);

	return 0;
}
