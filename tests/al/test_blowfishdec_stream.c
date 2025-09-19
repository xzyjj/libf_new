#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/blowfish.h>


struct T_ctx {
	struct blowfish_ctx blf_ctx;
	uint8 buf[8];
	uint32 count;
};

static int32 _decrypt_process(struct T_ctx *ctx,
		int32 (*call)(const uint8 *, void *), void *arg,
		const uint8 *s, uint64 len) {
	int32 n = ctx->count;
	for (uint64 i = 0; i < len; i++) {
		ctx->buf[n++] = s[i];
		if (n == 8) {
			FSYMBOL(blowfish_decrypt)(&ctx->blf_ctx,
				(uint32 *)ctx->buf,
				((uint32 *)ctx->buf) + 1);
			if (call(ctx->buf, arg))
				return -1;
			n = 0;
		}
	}
	ctx->count = n;

	return 0;
}

int32 _call(const uint8 *buf, void *arg) {
	fwrite(buf, 8, 1, stdout);

	return 0;
}

int main(void) {
	char *key = "123456";

	uint64 read_size = 0;
	struct T_ctx ctx = { .count = 0 };
	FSYMBOL(blowfish_init)(&ctx.blf_ctx, (uint8 *)key,
		XSYMBOL(strlen)(key));

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		_decrypt_process(&ctx, _call, NULL,
			(uint8 *)buf, read_size);
	}

	return 0;
}
