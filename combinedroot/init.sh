#!/sbin/busybox sh
set +x
_PATH="$PATH"
export PATH=/sbin

# leds paths
LED1_R_BRIGHTNESS_FILE="/sys/class/leds/LED1_R/brightness"
LED2_R_BRIGHTNESS_FILE="/sys/class/leds/LED2_R/brightness"
LED3_R_BRIGHTNESS_FILE="/sys/class/leds/LED3_R/brightness"
LED1_R_CURRENT_FILE="/sys/class/leds/LED1_R/led_current"
LED2_R_CURRENT_FILE="/sys/class/leds/LED2_R/led_current"
LED3_R_CURRENT_FILE="/sys/class/leds/LED3_R/led_current"
LED1_G_BRIGHTNESS_FILE="/sys/class/leds/LED1_G/brightness"
LED2_G_BRIGHTNESS_FILE="/sys/class/leds/LED2_G/brightness"
LED3_G_BRIGHTNESS_FILE="/sys/class/leds/LED3_G/brightness"
LED1_G_CURRENT_FILE="/sys/class/leds/LED1_G/led_current"
LED2_G_CURRENT_FILE="/sys/class/leds/LED2_G/led_current"
LED3_G_CURRENT_FILE="/sys/class/leds/LED3_G/led_current"
LED1_B_BRIGHTNESS_FILE="/sys/class/leds/LED1_B/brightness"
LED2_B_BRIGHTNESS_FILE="/sys/class/leds/LED2_B/brightness"
LED3_B_BRIGHTNESS_FILE="/sys/class/leds/LED3_B/brightness"
LED1_B_CURRENT_FILE="/sys/class/leds/LED1_B/led_current"
LED2_B_CURRENT_FILE="/sys/class/leds/LED2_B/led_current"
LED3_B_CURRENT_FILE="/sys/class/leds/LED3_B/led_current"

busybox cd /
busybox date >>boot.txt
exec >>boot.txt 2>&1
busybox rm /init

# include device specific vars
source /sbin/bootrec-device

# create directories
busybox mkdir -m 755 -p /dev/block
busybox mkdir -m 755 -p /dev/input
busybox mkdir -m 555 -p /proc
busybox mkdir -m 755 -p /sys

# create device nodes
busybox mknod -m 600 /dev/block/mmcblk0 b 179 0
busybox mknod -m 600 ${BOOTREC_EVENT_NODE}
busybox mknod -m 666 /dev/null c 1 3

# mount filesystems
busybox mount -t proc proc /proc
busybox mount -t sysfs sysfs /sys

# cmdline warmboots
MULTIROM_BOOT=$(busybox grep mrom_kexecd=1 /proc/cmdline)
RECOVERY_BOOT=$(busybox grep warmboot=0x77665502 /proc/cmdline)

# MultiROM booting
if [ ! -z "$MULTIROM_BOOT" ]; then
	RECOVERY_BOOT=
fi

# normal boot
if [ -z "$RECOVERY_BOOT" ] && [ -z "$MULTIROM_BOOT" ]; then
	# keycheck
	busybox cat ${BOOTREC_EVENT} > /dev/keycheck&
	busybox echo '50' > /sys/class/timed_output/vibrator/enable
	# LEDs activated
	echo '255' > $LED1_G_BRIGHTNESS_FILE
	echo '255' > $LED2_G_BRIGHTNESS_FILE
	echo '255' > $LED3_G_BRIGHTNESS_FILE
	echo '255' > $LED1_R_BRIGHTNESS_FILE
	echo '255' > $LED2_R_BRIGHTNESS_FILE
	echo '255' > $LED3_R_BRIGHTNESS_FILE
	# LEDs starting animation
	echo '16' > $LED1_G_CURRENT_FILE
	echo '16' > $LED2_G_CURRENT_FILE
	echo '16' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_G_CURRENT_FILE
	echo '32' > $LED2_G_CURRENT_FILE
	echo '32' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '64' > $LED1_G_CURRENT_FILE
	echo '64' > $LED2_G_CURRENT_FILE
	echo '64' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '92' > $LED1_G_CURRENT_FILE
	echo '92' > $LED2_G_CURRENT_FILE
	echo '92' > $LED3_G_CURRENT_FILE
	busybox sleep 1
	echo '64' > $LED1_G_CURRENT_FILE
	echo '64' > $LED2_G_CURRENT_FILE
	echo '64' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_G_CURRENT_FILE
	echo '32' > $LED2_G_CURRENT_FILE
	echo '32' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '0' > $LED1_G_BRIGHTNESS_FILE
	echo '0' > $LED2_G_BRIGHTNESS_FILE
	echo '0' > $LED3_G_BRIGHTNESS_FILE
	echo '0' > $LED1_G_CURRENT_FILE
	echo '0' > $LED2_G_CURRENT_FILE
	echo '0' > $LED3_G_CURRENT_FILE
	echo '16' > $LED1_R_CURRENT_FILE
	echo '16' > $LED2_R_CURRENT_FILE
	echo '16' > $LED3_R_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_R_CURRENT_FILE
	echo '32' > $LED2_R_CURRENT_FILE
	echo '32' > $LED3_R_CURRENT_FILE
	busybox sleep 0.05
	echo '64' > $LED1_R_CURRENT_FILE
	echo '64' > $LED2_R_CURRENT_FILE
	echo '64' > $LED3_R_CURRENT_FILE
	busybox sleep 0.05
	echo '92' > $LED1_R_CURRENT_FILE
	echo '92' > $LED2_R_CURRENT_FILE
	echo '92' > $LED3_R_CURRENT_FILE
	busybox sleep 1
	echo '64' > $LED1_R_CURRENT_FILE
	echo '64' > $LED2_R_CURRENT_FILE
	echo '64' > $LED3_R_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_R_CURRENT_FILE
	echo '32' > $LED2_R_CURRENT_FILE
	echo '32' > $LED3_R_CURRENT_FILE
	busybox sleep 0.05
	echo '0' > $LED1_R_BRIGHTNESS_FILE
	echo '0' > $LED2_R_BRIGHTNESS_FILE
	echo '0' > $LED3_R_BRIGHTNESS_FILE
	echo '0' > $LED1_R_CURRENT_FILE
	echo '0' > $LED2_R_CURRENT_FILE
	echo '0' > $LED3_R_CURRENT_FILE
fi

# android ramdisk
load_image=/sbin/ramdisk.cpio

# boot decision
if [ ! -z "$RECOVERY_BOOT" ] || [ -s /dev/keycheck ]; then
	busybox echo 'RECOVERY BOOT' >>boot.txt
	# LEDs for recovery
	busybox echo '100' > /sys/class/timed_output/vibrator/enable
	echo '255' > $LED1_B_BRIGHTNESS_FILE
	echo '255' > $LED2_B_BRIGHTNESS_FILE
	echo '255' > $LED3_B_BRIGHTNESS_FILE
	echo '32' > $LED1_B_CURRENT_FILE
	echo '32' > $LED2_B_CURRENT_FILE
	echo '32' > $LED3_B_CURRENT_FILE
	busybox sleep 0.05
	echo '64' > $LED1_B_CURRENT_FILE
	echo '64' > $LED2_B_CURRENT_FILE
	echo '64' > $LED3_B_CURRENT_FILE
	busybox sleep 0.05
	echo '128' > $LED1_B_CURRENT_FILE
	echo '128' > $LED2_B_CURRENT_FILE
	echo '128' > $LED3_B_CURRENT_FILE
	busybox sleep 1
	echo '64' > $LED1_B_CURRENT_FILE
	echo '64' > $LED2_B_CURRENT_FILE
	echo '64' > $LED3_B_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_B_CURRENT_FILE
	echo '32' > $LED2_B_CURRENT_FILE
	echo '32' > $LED3_B_CURRENT_FILE
	busybox sleep 0.05
	echo '0' > $LED1_B_BRIGHTNESS_FILE
	echo '0' > $LED2_B_BRIGHTNESS_FILE
	echo '0' > $LED3_B_BRIGHTNESS_FILE
	echo '0' > $LED1_B_CURRENT_FILE
	echo '0' > $LED2_B_CURRENT_FILE
	echo '0' > $LED3_B_CURRENT_FILE
	# recovery ramdisk
	if [ ! -z "$RECOVERY_BOOT" ] || [ -z $(busybox grep $'\x72' /dev/keycheck) ]; then
		busybox mknod -m 600 ${BOOTREC_FOTA_NODE}
		busybox mount -o remount,rw /
		busybox ln -sf /sbin/busybox /sbin/sh
		extract_elf_ramdisk -i ${BOOTREC_FOTA} -o /sbin/ramdisk-recovery.cpio -t / -c
		busybox rm /sbin/sh
	fi
	load_image=/sbin/ramdisk-recovery.cpio
else
	busybox echo 'ANDROID BOOT' >>boot.txt
	# LEDs for Android
	echo '255' > $LED1_G_BRIGHTNESS_FILE
	echo '255' > $LED2_G_BRIGHTNESS_FILE
	echo '255' > $LED3_G_BRIGHTNESS_FILE
	echo '32' > $LED1_G_CURRENT_FILE
	echo '32' > $LED2_G_CURRENT_FILE
	echo '32' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '64' > $LED1_G_CURRENT_FILE
	echo '64' > $LED2_G_CURRENT_FILE
	echo '64' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '128' > $LED1_G_CURRENT_FILE
	echo '128' > $LED2_G_CURRENT_FILE
	echo '128' > $LED3_G_CURRENT_FILE
	busybox sleep 1
	echo '64' > $LED1_G_CURRENT_FILE
	echo '64' > $LED2_G_CURRENT_FILE
	echo '64' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '32' > $LED1_G_CURRENT_FILE
	echo '32' > $LED2_G_CURRENT_FILE
	echo '32' > $LED3_G_CURRENT_FILE
	busybox sleep 0.05
	echo '0' > $LED1_G_BRIGHTNESS_FILE
	echo '0' > $LED2_G_BRIGHTNESS_FILE
	echo '0' > $LED3_G_BRIGHTNESS_FILE
	echo '0' > $LED1_G_CURRENT_FILE
	echo '0' > $LED2_G_CURRENT_FILE
	echo '0' > $LED3_G_CURRENT_FILE
fi

# kill the keycheck process
busybox pkill -f "busybox cat ${BOOTREC_EVENT}"
busybox echo '0' > /sys/class/timed_output/vibrator/enable

# unpack the ramdisk image
busybox cpio -i < ${load_image}

busybox umount /proc
busybox umount /sys

busybox rm -fr /dev/*
busybox date >>boot.txt
export PATH="${_PATH}"
exec /init
