#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_md5.h>
#include <libf/al/hkdf_md5.h>


int main(void) {
	uint8 buf[MD5_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_MD5_NEW(ctx);
	FSYMBOL(hmac_md5_init)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_md5_process)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_md5_finish)(&ctx, len);
	FSYMBOL(hkdf_md5)(&(HMAC_MD5_STATE(&ctx, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
