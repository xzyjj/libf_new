#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/sl/xstdio.h>


int main(void) {
	/* snprintf */
	char buf[128];
	XSYMBOL(memset)(buf, 'A', 128);

	snprintf(buf, 108, "h%99dh ωω\xe3\n", 123);
	printf("libc: %s\n", buf);

	XSYMBOL(snprintf)(buf, 108, "h%99dh ωω\xe3\n", 123);
	printf("libf: %s\n", buf);

	return 0;
}
