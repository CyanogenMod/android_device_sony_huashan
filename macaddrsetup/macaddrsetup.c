#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cutils/properties.h>

#define LOG_TAG "macaddrsetup"
#include <cutils/log.h>

extern const char *__progname;
extern int ta_open(uint8_t p, uint8_t m, uint8_t c);
extern int ta_close(void);
extern int ta_getsize(uint32_t id, uint32_t *size);
extern int ta_read(uint32_t id, void *buf, uint32_t size);

int main(int argc, char **argv)
{
	uint32_t size;
	char buf[6];
	FILE *fpb, *fpw;
	char record[PROPERTY_VALUE_MAX];
	int ret, err, bt_addr, wl_addr;

	// Sony had a check for ro.hardware here, but since ours is set to qcom, that failed.
	wl_addr=2560;
	bt_addr=2568;

	for (;;) {
		err = ta_open(2, 0x1, 1);
		if (!err)
			break;

		SLOGI( "failed to open misc ta: %d\n",err);
		sleep(5);
	}

	fpb = fopen("/data/etc/bluetooth_bdaddr", "w");
	if (!fpb) {
		SLOGI("failed to open /data/etc/bluetooth_bdaddr for writing:\n");
		ta_close();
		exit(1);
	}

	fpw = fopen(argv[1], "w");
	if (!fpw) {
		SLOGI("failed to open %s for writing: %s\n", argv[1], strerror(errno));
		ta_close();
		exit(1);
	}

	err = ta_getsize(bt_addr, &size);
	if (size != 6) {
		SLOGI("BT mac address have wrong size (%d) in miscta", size);
		ta_close();
		fclose(fpb);
		exit(1);
	}

	err = ta_read(bt_addr, buf, size);
	if (err) {
		SLOGI("failed to read BT mac address from miscta");
		ta_close();
		fclose(fpb);
		exit(1);
	}

	ret = fprintf(fpb, "%02x:%02x:%02x:%02x:%02x:%02x\n", buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
	if (ret != 18) {
		SLOGI("failed to write BT mac address\n");
		ta_close();
		fclose(fpb);
		exit(1);
	}

	err = ta_getsize(wl_addr, &size);
	if (size != 6) {
		SLOGI("mac address have wrong size (%d) in miscta", size);
		ta_close();
		fclose(fpw);
		exit(1);
	}

	err = ta_read(wl_addr, buf, size);
	if (err) {
		SLOGI("failed to read mac address from miscta");
		ta_close();
		fclose(fpw);
		exit(1);
	}

	ret = fprintf(fpw, "%02x:%02x:%02x:%02x:%02x:%02x\n", buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
	if (ret != 18) {
		SLOGI("failed to write WLAN mac address\n");
		ta_close();
		fclose(fpw);
		exit(1);
	}

	ta_close();
	fclose(fpb);
	fclose(fpw);

	return 0;
}
