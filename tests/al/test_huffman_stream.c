#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/huffman.h>


void each(struct huf_node *node, int32 n, int32 c) {
	if (node) {
		each(node->right, n + 4, 1);

		for (int32 i = 0; i < n; i++)
			printf(" ");

		uint8 cc = node->data[0];
		cc = (cc < 0x20 || cc >= 0x7f) ? ' ' : cc;
		if (c == 1) {
			printf("/%u '%c' (0x%02x)\n", node->freq, cc,
				node->data[0]);
		} else if (c == -1) {
			printf("\\%u '%c' (0x%02x)\n", node->freq, cc,
				node->data[0]);
		} else{
			printf("=%u '%c' (0x%02x)\n", node->freq, cc,
				node->data[0]);
		}

		each(node->left, n + 4, -1);
	}
}

void each_gcode(struct huf_node *node, char *s, int32 n, uint32 *total) {
	if (!node)
		return;

	if (node->left) {
		s[n] = '0';
		each_gcode(node->left, s, n + 1, total);
	}
	if (node->right) {
		s[n] = '1';
		each_gcode(node->right, s, n + 1, total);
	}

	if (!node->left && !node->right) {
		if (!n)
			s[n++] = '0';
		s[n] = '\0';
		*total += node->freq * strlen(s);

		uint8 c = node->data[0];
		printf("'%c' (%u)(0x%02x): %s\n",
			(c < 0x20 || c >= 0x7f) ? ' ' : c,
			node->freq, c, s);
	}
}

int main(void) {
	clock_t start, end;
	double time;

	uint8 buf[1 << 16];
	uint64 read_size = 0, total_size = 0;

	void *array[256];
	struct huf_node node[256];
	struct huf_node parent[256];
	HUF_NEW(huf, array, 256, parent);
	XSYMBOL(memset)(node, 0, sizeof(struct huf_node) * 256);

	void *p = NULL;
	start = clock();
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(huf_u8freq)(node, buf, read_size);
		total_size += read_size;
	}
	p = FSYMBOL(huf_u8build)(&huf, node);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;

	each(p, 0, 0);

	char strbuf[128];
	uint32 total = 0;
	each_gcode(p, strbuf, 0, &total);
	printf("%d\n", total);

	printf("%.6fs (%.2fMiB/s)\n", time,
		(double)(total_size / time) / 1024 / 1024);

	return 0;
}
