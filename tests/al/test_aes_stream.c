#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/aes.h>


struct T_ctx {
	struct aes_ctx aes_ctx;
	uint8 buf[AES_BLOCKSIZE];
	uint32 count;
};

static int32 _aes_encrypt_process(struct T_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len) {
	int32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == AES_BLOCKSIZE) {
			FSYMBOL(aes_encrypt)(&ctx->aes_ctx, ctx->buf);
			if (call(ctx->buf, arg))
				return -1;
			n = 0;
		}
	}
	ctx->count = n;

	return 0;
}

static int32 _aes_encrypt_finish(struct T_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg) {
	if (!ctx->count)
		return 0;

	XSYMBOL(memset)(&ctx->buf[ctx->count], 0, AES_BLOCKSIZE - ctx->count);
	FSYMBOL(aes_encrypt)(&ctx->aes_ctx, ctx->buf);
	if (call(ctx->buf, arg))
		return -1;

	return 0;
}

int32 _call(const uint8 *buf, void *arg) {
	fwrite(buf, AES_BLOCKSIZE, 1, stdout);

	return 0;
}

int main(void) {
	uint64 read_size = 0;
	struct T_ctx ctx = { .count = 0 };

	uint8 key[AES_128_KEYLEN];
	XSYMBOL(memset)(key, 0x3f, AES_128_KEYLEN);
	FSYMBOL(aes_init)(&ctx.aes_ctx, key, AES_128_TYPE);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		_aes_encrypt_process(&ctx, _call, NULL,
			(uint8 *)buf, read_size);
	}
	_aes_encrypt_finish(&ctx, _call, NULL);

	return 0;
}
