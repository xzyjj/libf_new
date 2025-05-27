#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/base64.h>


int main(void) {
	uint64 read_size = 0;
	char buf[1 << 16];
	char ibuf[BASE64_DEC_IALIGN(1 << 16)];
	char obuf[BASE64_DEC_OALIGN(1 << 16)];
	uint32 count = 0;

	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		for (uint32 i = 0; i < read_size; i++) {
			char c = buf[i];
			if (c == ' ' || c == '\n' || c == '\t')
				continue;
			ibuf[count++] = c;
			if (count == sizeof(ibuf)) {
				uint32 n = sizeof(obuf);
				if (FSYMBOL(base64_dec)(ibuf, count, obuf, &n))
					return 1;
				fwrite(obuf, 1, sizeof(obuf) - n, stdout);
				count = 0;
			}
		}
	}

	if (count) {
		uint32 n = sizeof(obuf);
		if (FSYMBOL(base64_dec)(ibuf, count, obuf, &n))
			return 1;
		fwrite(obuf, 1, sizeof(obuf) - n, stdout);
	}

	return 0;
}
