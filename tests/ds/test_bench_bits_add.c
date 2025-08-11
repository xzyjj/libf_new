#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_add.h>


int main(void) {
	clock_t start, end;
	double time;
	BITS_ADD_NEW(ctx);

	start = clock();
	for (int32 i = 0; i < 10000000; i++) {
		while (FSYMBOL(bits_add)(&ctx, 1, (i & 11) + 1))
			BITS_ADD_FLUSH(&ctx);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;

	uint64 n = 0;
	for (int32 i = 0; i < 10000000; i++) {
		n += (i & 11) + 1;
	}
	printf("time: %.6f (%.2fMiB/s)\n", time,
		(n / time) / 8 / 1000 / 1000);

	return 0;
}
