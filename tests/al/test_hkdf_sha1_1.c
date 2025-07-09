#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha1.h>
#include <libf/al/hkdf_sha1.h>


int main(void) {
	uint8 buf[SHA1_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_SHA1_NEW(ctx);
	FSYMBOL(hmac_sha1_init)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_sha1_process)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_sha1_finish)(&ctx, len);
	FSYMBOL(hkdf_sha1)(&(HMAC_SHA1_STATE(&ctx, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
