#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/base64.h>


int main(void) {
	uint64 read_size = 0;
	char ibuf[1 << 16];
	char obuf[BASE64_ENC_OALIGN(1 << 16)];

	while ((read_size = fread(ibuf, 1, sizeof(ibuf), stdin)) > 0) {
		uint32 n = sizeof(obuf);
		FSYMBOL(base64_enc)(ibuf, read_size, obuf, &n);
		fwrite(obuf, 1, sizeof(obuf) - n, stdout);
	}

	return 0;
}
