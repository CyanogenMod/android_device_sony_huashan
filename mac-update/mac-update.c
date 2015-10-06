/* To FreeXperia from a friend :) */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mac-update.h"

int read_mac(const char *filename, char *buf)
{
	int ret;

	FILE *f = fopen(filename, "r");
	if (!f)
		return -ENOENT;
	ret = fscanf(f, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &buf[0], &buf[1],
		     &buf[2], &buf[3], &buf[4], &buf[5]);
	fclose(f);
	if (ret != 6) {
		return -EINVAL;
	}
	return 0;
}

#define UPD_MAC(file, offset) \
do { \
	if (read_mac(file, buf + offset)) { \
		perror("Warning. Failed to read mac from " #file " - skipping"); \
	} \
} while(0)


int main(int argc, char **argv)
{
	char *buf;
	struct stat statbuf;
	int ret;
	size_t actual;
	FILE *f;

	ret = stat(NV_IN, &statbuf);
	if (ret) {
		perror("Failed to stat " NV_IN);
		exit(EINVAL);
	}
	f = fopen(NV_IN, "r");
	if (!f) {
		perror("Failed to open " NV_IN);
		exit(EINVAL);
	}

	if (statbuf.st_size < 100) {
		perror("nv file too small");
		exit(EINVAL);
	}

	if (statbuf.st_size != NV_SIZE)
		perror("Warning - size invalid");

	buf = malloc(statbuf.st_size);
	if (!buf) {
		perror("malloc failed");
		exit(ENOMEM);
	}

	actual = fread(buf, 1, statbuf.st_size, f);
	if (actual != statbuf.st_size) {
		perror("Failed to read from nv");
		exit(EINVAL);
	}
	fclose(f);

	UPD_MAC(MAC0_FILE, MAC0_OFFSET);

	f = fopen(NV_OUT, "w+");
	if (!f) {
		perror("Failed to open " NV_OUT);
		exit(EINVAL);
	}
	actual = fwrite(buf, 1, statbuf.st_size, f);
	if (actual != statbuf.st_size) {
		perror("Failed to write to nv");
		exit(EINVAL);
	}

	fclose(f);

	return 0;
}
