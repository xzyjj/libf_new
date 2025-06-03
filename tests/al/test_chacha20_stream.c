#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/chacha20.h>


int main(void) {
	uint64 read_size = 0;
	uint8 key[CHACHA20_KEYLEN];
	uint8 ran[CHACHA20_RANLEN];
	XSYMBOL(memset)(key, 00, CHACHA20_KEYLEN);
	XSYMBOL(memset)(ran, 00, CHACHA20_RANLEN);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(chacha20_crypto)(key, ran, 0, (uint8 *)buf, read_size);
		fwrite(buf, 1, read_size, stdout);
	}

	return 0;
}
