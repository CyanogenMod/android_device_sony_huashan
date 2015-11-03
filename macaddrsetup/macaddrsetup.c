#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <private/android_filesystem_config.h>

#define LOG_TAG "macaddrsetup"
#include <cutils/log.h>

extern const char *__progname;
extern int ta_open(uint8_t p, uint8_t m, uint8_t c);
extern int ta_close(void);
extern int ta_getsize(uint32_t id, uint32_t *size);
extern int ta_read(uint32_t id, void *buf, uint32_t size);

int read_mac(int ta_id, int mac_addr, char* mac_file)
{
	uint32_t size;
	char buf[6];
	FILE* fp;
	int i, ret;

	for (i = 0; i < 1000; ++i) {
		ret = ta_open(ta_id, 0x1, 1);
		if (!ret) {
			break;
		}

		SLOGI("failed to open misc ta %d : %d\n", ta_id, ret);
		ta_close();
		sleep(5);
	}

	fp = fopen(mac_file, "w");
	if (!fp) {
		SLOGI("failed to open %s for writing : %s\n", mac_file, strerror(errno));
		ta_close();
		return 1;
	}

	ta_getsize(mac_addr, &size);
	if (size != 6) {
		SLOGI("Value %d has wrong size (%d) in miscta %d\n", mac_addr, size);
		ta_close();
		fclose(fp);
		return 1;
	}

	ret = ta_read(mac_addr, buf, size);
	if (ret) {
		SLOGI("failed to read %d mac address from miscta %d\n", mac_addr);
		ta_close();
		fclose(fp);
		return 1;
	}

	ret = fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\n", buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
	if (ret != 18) {
		SLOGI("failed to write mac address in %s\n", mac_file);
		ta_close();
		fclose(fp);
		return 1;
	}

	ta_close();
	fclose(fp);
	return 0;
}

int main(int argc, char **argv)
{
	int err;

	// Missing WLAN and BT file paths
	if (argc < 2) {
		return 1;
	}

	// SONY Huashan has the WLAN MACs in TA 2 : 2560, 2561, 2562, 2563
	err = read_mac(2, 2560, argv[1]);
	if (err) {
		return err;
	}

	// SONY Huashan has the Bluetooth MAC TA 1 : 447
	err = read_mac(1, 447, argv[2]);
	if (err) {
		return err;
	}

	// Set correct permissions for Bluetooth
	chown(argv[2], AID_BLUETOOTH, AID_BLUETOOTH);
	chmod(argv[2], S_IRUSR | S_IWUSR | S_IRGRP); // 640

	// End of MACAddrSetup
	SLOGD("successfully wrote WiFi and Bluetooth MACs");
	return 0;
}
