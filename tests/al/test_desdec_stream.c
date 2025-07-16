#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/des.h>


struct T_ctx {
	struct des_ctx des_ctx;
	uint8 buf[DES_BLOCKSIZE];
	uint32 count;
};

static int32 _des_decrypt_process(struct T_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len) {
	int32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == DES_BLOCKSIZE) {
			FSYMBOL(des_crypto)(&ctx->des_ctx, ctx->buf,
				DES_DECRYPT);
			if (call(ctx->buf, arg))
				return -1;
			n = 0;
		}
	}
	ctx->count = n;

	return 0;
}

int32 _call(const uint8 *buf, void *arg) {
	fwrite(buf, DES_BLOCKSIZE, 1, stdout);

	return 0;
}

int main(void) {
	uint64 read_size = 0;
	struct T_ctx ctx = { .count = 0 };

	uint8 key[DES_KEYLEN];
	XSYMBOL(memset)(key, 0, DES_KEYLEN);
	key[0] = 0x12;
	key[1] = 0x23;
	key[2] = 0x45;
	FSYMBOL(des_init)(&ctx.des_ctx, key);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		_des_decrypt_process(&ctx, _call, NULL,
			(uint8 *)buf, read_size);
	}

	return 0;
}
