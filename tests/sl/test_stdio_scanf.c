#include <stdio.h>
#include <errno.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstdio.h>
#include <libf/sl/xstdio_sys.h>


int main(void) {
	int32 r = 0, i = 0, j, k = -1;
	float64 f = 0;
	char buf[32], buf2[32];

	r = sscanf("-99999999999999999999999999999999 123"
		" 0777 Hello, world!",
		"%u %o %i %3[^,]%3[^!]", &i, &j, &k, &buf, &buf2);
	printf("libc: %d: %d %d %d '%s' '%s'\n", r, i, j, k, buf, buf2);

	r = XSYMBOL(sscanf)("-99999999999999999999999999999999 123"
		" 0777 Hello, world!",
		"%u %o %i %2[^,]%2[^!]", &i, &j, &k, &buf, &buf2);
	printf("libf: %d: %d %d %d '%s' '%s'\n", r, i, j, k, buf, buf2);

	r = sscanf("123.314 Hello 0xff00",
		"%f %4s %i", &f, &buf, &i);
	printf("libc: %d: %f '%s' %x\n", r, f, buf, i);

	r = XSYMBOL(sscanf)("123.314e+10 Hello 0xff00",
		"%f %4s %i", &f, &buf, &i);
	printf("libf: %d: %f '%s' %x\n", r, f, buf, i);

	return 0;
}
