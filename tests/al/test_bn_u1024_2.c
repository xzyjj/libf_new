#include <stdio.h>
#include <libf/al/bn_4096.h>


int main(void) {
	char buf[1 << 17];
	bn_uint4096_t a, b, r, quo, rem;
	FSYMBOL(bn_uint4096_zero)(a);
	FSYMBOL(bn_uint4096_zero)(b);
	a[BN_4096_SIZE - 1] = 1;

	FSYMBOL(bn_uint4096_numtostr)(buf, a);
	printf("%s\n", buf);

	return 0;
}
