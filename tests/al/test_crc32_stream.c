#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/crc32.h>


int main(void) {
	uint64 read_size = 0;
	char buf[1 << 16];

	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		printf("crc32msb: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_DEFAULT_MSB_TYPE));
		printf("crc32lsb: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_DEFAULT_LSB_TYPE));
		printf("crc32msb-c: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_CASTAGNOLI_MSB_TYPE));
		printf("crc32lsb-c: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_CASTAGNOLI_LSB_TYPE));
		printf("crc32msb-k: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_KOOPMAN_MSB_TYPE));
		printf("crc32lsb-k: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_KOOPMAN_LSB_TYPE));
		printf("crc32msb-cksum: %u\n",
			FSYMBOL(crc32)(buf, read_size, CRC32_CKSUM_MSB_TYPE));

		break;
	}

	return 0;
}
