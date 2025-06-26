#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ds/bfifo.h>


int main(void) {
	clock_t start, end;
	double time;
	char bbuf[1024], buf[4096];
	BFIFO_NEW(ctx, bbuf, sizeof(bbuf));

	uint32 n = 0, t, t1, r;
	start = clock();
	for (; n < 806596; n++) {
		t = sizeof(buf);
		t1 = sizeof(buf) / 2;
		do {
			do {
				t -= r = FSYMBOL(bfifo_in)(&ctx, buf, t);	
			} while (r && t);

			do {
				r = FSYMBOL(bfifo_out)(&ctx, buf, t1);
			} while (r);
		} while (t);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.6fs (%.2fMiB/s)\n", time,
		((double)(sizeof(buf) * n) / time) / 1024 / 1024);

	return 0;
}
