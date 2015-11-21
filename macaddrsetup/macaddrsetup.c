/*
 * Copyright (C) 2015 Sony Mobile Communications Inc.
 *
 *   Nov 2015 : Rewritten by Adrian DC for CyanogenMod / Huashan
 */

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

/* TA Misc Data Constants */
#include "tamiscdata.h"
#define TYPE_MAC 6
#define TYPE_SERIAL 10
#define BUF_SIZE 10

/* TA Misc Opened Handle */
unsigned int ta_opened = 0;

extern const char *__progname;
extern int ta_open(uint8_t p, uint8_t m, uint8_t c);
extern int ta_close(void);
extern int ta_getsize(uint32_t id, uint32_t *size);
extern int ta_read(uint32_t id, void *buf, uint32_t size);

int read_mac(unsigned int ta_id, int val_addr, char* val_file, unsigned int val_size)
{
    uint32_t size;
    char buf[BUF_SIZE];
    FILE* fp;
    int i, ret;

    /* If the ta_id partition wasn't opened */
    if (ta_id != ta_opened) {
        if (ta_opened != 0) {
            ta_close();
        }
        /* Try to open the TA Misc partition "ta_id" */
        for (i = 0;; ++i) {
            ret = ta_open(ta_id, 0x1, 1);
            if (!ret) {
                ta_opened = ta_id;
                break;
            }
            else if (i > 100) {
                return 1;
            }

            SLOGI("failed to open misc ta %d : %d\n", ta_id, ret);
            ta_close();
            sleep(5);
        }
    }

    /* Open the output file */
    fp = fopen(val_file, "w");
    if (!fp) {
        SLOGI("failed to open %s for writing : %s\n", val_file, strerror(errno));
        return 1;
    }

    /* Read the size of the object */
    ta_getsize(val_addr, &size);
    if (size != val_size) {
        SLOGI("Value %d has wrong size (%d) in miscta %d\n", val_addr, size, ta_id);
        fclose(fp);
        return 1;
    }

    /* Read the object data */
    ret = ta_read(val_addr, buf, size);
    if (ret) {
        SLOGI("failed to read %d mac address from miscta %d\n", val_addr, ta_id);
        fclose(fp);
        return 1;
    }

    /* Handle the various output formats */
    switch (val_size) {
        /* Output a MAC file format */
        case TYPE_MAC:
            ret = fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\n", buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
            if (ret != 18) {
                SLOGI("failed to write mac address in %s\n", val_file);
                fclose(fp);
                return 1;
            }
            break;
        /* Output a SERIAL file format */
        case TYPE_SERIAL:
            ret = fprintf(fp, "%02x%02x%02x%02x\n", buf[0], buf[1], buf[2], buf[3]);
            if (ret != 9) {
                SLOGI("failed to write serial number in %s\n", val_file);
                fclose(fp);
                return 1;
            }
            break;
        /* Output error */
        default:
            SLOGI("unknown format output for %s\n", val_file);
            break;
    }

    /* Close the output file */
    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{
    int err = 0;
    SLOGD("importation starting\n");

    /* Unused parameters */
    if (argc > 1) {
        SLOGD("parameters unneeded %d : %.50s\n", argc - 1, argv[1]);
        return 1;
    }

    /* WiFi Serial Number importation */
    err |= read_mac(WLAN_SERIAL_TAID, WLAN_SERIAL_PNTR, WLAN_SERIAL_FILE, TYPE_SERIAL);

    /* WiFi MAC importation */
    err |= read_mac(WLAN_MAC_TAID, WLAN_MAC_PNTR, WLAN_MAC_FILE, TYPE_MAC);

    /* Bluetooth MAC importation */
    err |= read_mac(BT_MAC_TAID, BT_MAC_PNTR, BT_MAC_FILE, TYPE_MAC);

    /* Set correct permissions for Bluetooth */
    chown(BT_MAC_FILE, AID_BLUETOOTH, AID_BLUETOOTH);
    chmod(BT_MAC_FILE, S_IRUSR | S_IWUSR | S_IRGRP); /* 640 */

    /* Close the TA Partition */
    if (ta_opened != 0) {
        ta_close();
    }

    /* End of MACAddrSetup */
    if (!err) {
        SLOGD("successfully imported WiFi and Bluetooth parameters\n");
    }
    return err;
}
