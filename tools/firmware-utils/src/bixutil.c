#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#define RTL_PRODUCT_MAGIC1    0x59a0e842
#define SQUASH_MAGIC          0x73717368
#define RTL_IMAGE_VERSION     1
#define RTL_IMAGE_TYPE_RDIR   0xb162
#define RTL_IMAGE_TYPE_BOOT   0xea43
#define RTL_IMAGE_TYPE_RUN    0x8dc9
#define RTL_IMAGE_TYPE_KFS    0xd92f
#define RTL_IMAGE_TYPE_CCFG   0x2a05
#define RTL_IMAGE_TYPE_DCFG   0x6ce8
#define RTL_IMAGE_TYPE_LOG    0xc371

#define ENDIAN_SWITCH32(x) (((x) >> 24) |				\
			    (((x) >> 8) & 0xFF00) |			\
			    (((x) << 8) & 0xFF0000) |			\
			    (((x) << 24) &0xFF000000))
#define ENDIAN_SWITCH16(x) ((((x) >> 8) & 0xFF) |	\
			    (((x) << 8) & 0xFF00))


struct image_header {
	u_int32_t magic1;
	u_int16_t type;
	u_int8_t  ver;
	u_int8_t  align1;
	u_int16_t year;
	u_int8_t  mon;
	u_int8_t  day;
	u_int8_t  hour;
	u_int8_t  min;
	u_int8_t  sec;
	u_int8_t  align2;
	u_int32_t len;
	u_int16_t align3;
	u_int8_t  icsum;
	u_int8_t  hcsum;
};

void print_header(struct image_header *h)
{
	printf("Magic1: 0x%08x\n", ENDIAN_SWITCH32(h->magic1));
	printf("Type: 0x%04x\n", ENDIAN_SWITCH16(h->type));
	printf("Version: %d\n", h->ver);
	printf("Built: %04d-%02d-%02d %02d:%02d:%02d\n",
	       ENDIAN_SWITCH16(h->year), h->mon, h->day, h->hour,
	       h->min, h->sec); 
	printf("Length: %ld\n", ENDIAN_SWITCH32(h->len));
	printf("Checksums: 0x%02x (header), 0x%02x (image)\n",
	       h->hcsum, h->icsum);
}

void *do_mmap(char *filename, int size, int readonly)
{
	int fd;
	int openflags = O_RDWR | O_CREAT | O_TRUNC;
	int protflags = PROT_READ | PROT_WRITE;
	void *ptr;
	struct stat st;

	if (readonly) {
		openflags = O_RDONLY;
		protflags = PROT_READ;
	}

	if (!size) {
		if (!readonly) 
			goto failed;
		if (stat(filename, &st) != 0) {
			perror("stat failed");
			goto failed;
		}
		size = st.st_size;
	}

	if (!size) 
		goto failed;

	if ((fd = open(filename, openflags, 0644)) == -1) {
		perror("open failed");
		goto failed;
	}

	if (!readonly) 
		ftruncate(fd, size);

	ptr = mmap(0, size, protflags, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap failed");
		goto mmap_failed;
	}

	return ptr;

mmap_failed:
	close(fd);
failed:
	return NULL;
}

int write_image(char *kernel, char *outfile)
{
	unsigned char type;
	unsigned long ksize, padded;
	unsigned long i;
	unsigned char ch, *src, *dst;
	struct stat st;
	struct image_header *h;
	time_t tt;
	struct tm *ut;

	tt = time(NULL);
	ut = localtime(&tt);

	if (stat(kernel, &st) != 0)
		error(-1, "stat %s failed", kernel);
	ksize = st.st_size;

	padded = sizeof(struct image_header) + 
		ksize + ksize % 4;  /* fs starts 32-bit aligned (?check) */

	h = (struct image_header *)do_mmap(outfile, padded, 0);
	if (!h) 
		return -1;

	memset(h, 0, sizeof(struct image_header));

	dst = (unsigned char *)((unsigned)h + sizeof(struct image_header));
	src = (unsigned char *)do_mmap(kernel, ksize, 1);
	if (!src)
		return -1;

	for (i = 0; i < ksize; i++) {
		dst[i] = src[i];
		h->icsum ^= src[i];
	}

	for (i = 0; i < ksize % 4; i++)
		dst[ksize + i] = 0;

	munmap(src, ksize);

	h->magic1 = ENDIAN_SWITCH32(RTL_PRODUCT_MAGIC1);
	h->type = ENDIAN_SWITCH16(RTL_IMAGE_TYPE_RUN);
	h->ver = RTL_IMAGE_VERSION;
	h->year = ENDIAN_SWITCH16(ut->tm_year + 1900);
	h->mon = ut->tm_mon + 1;
	h->day = ut->tm_mday;
	h->hour = ut->tm_hour;
	h->min = ut->tm_min;
	h->sec = ut->tm_sec;
	h->len = ENDIAN_SWITCH32(padded - sizeof(struct image_header));

	for (i = 0; i < (sizeof(struct image_header) - 1); i++)
		h->hcsum ^= ((unsigned char *)&h)[i];
	munmap(h, padded);
	return 0;
}

void usage(void) 
{
	exit(1);
}

int main(int argc, char *argv[]) 
{
	if (argc < 3) usage();

	printf("%d\n", sizeof(struct image_header));
	if (!strncmp(argv[1], "info", 4)) {
		print_header((struct image_header *)do_mmap(argv[2],
							    0, 1));
	} else if (!strncmp(argv[1], "pack", 4)) {
		write_image(argv[2], argv[3]);
	} else {
		usage();
	}

	return 0;
}
