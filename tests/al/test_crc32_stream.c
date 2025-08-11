#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/crc32.h>


int main(void) {
	uint64 read_size = 0;
	char buf[1 << 16];

	uint32 c = 0;
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_DEFAULT_MSB_TYPE);
		printf("crc32msb: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_DEFAULT_LSB_TYPE);
		printf("crc32lsb: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_CASTAGNOLI_MSB_TYPE);
		printf("crc32msb-c: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_CASTAGNOLI_LSB_TYPE);
		printf("crc32lsb-c: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_KOOPMAN_MSB_TYPE);
		printf("crc32msb-k: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_KOOPMAN_LSB_TYPE);
		printf("crc32lsb-k: %u (0x%08x)\n", c, c);

		c = FSYMBOL(crc32)((uint8 *)buf, read_size,
			CRC32_CKSUM_MSB_TYPE);
		printf("crc32msb-cksum: %u (0x%08x)\n", c, c);

		break;
	}

	return 0;
}
