#include <sys/param.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int
main(int argc, char *argv[])
{
	uintmax_t size, cnt;
	volatile uintmax_t *p, *end;
	void *scratch;
	const uintmax_t page_size = getpagesize();

	if (argc != 2) {
		fprintf(stderr, "usage: %s megabytes", argv[0]);
		return 64;
	}
	size = strtoull(argv[1], NULL, 0) * 1024 * 1024;
	scratch = mmap(NULL, (size_t) size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (scratch == MAP_FAILED) {
		perror("mmap");
		return 72;
	}
	
	fprintf(stderr, "size: %ju, psz: %ju, cnt: %ju\n", size, page_size, size / page_size);
#define STEP (page_size / sizeof *p)
	for (cnt = 0, p = scratch, end = p + size / sizeof *p; p < end; p += STEP) {
		fprintf(stderr, "%c %ju\r", "|/-\\"[cnt % 4], cnt);
		*p = ++cnt;
	}
	fprintf(stderr, "* %ju, waiting for signal\n", cnt);
	pause();
}
