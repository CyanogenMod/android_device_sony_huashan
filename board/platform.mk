# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8960

# Vendor platform
BOARD_VENDOR := sony
BOARD_VENDOR_PLATFORM := viskan

# Legacy blobs
TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true

# Dumpstate
BOARD_LIB_DUMPSTATE := libdumpstate.sony

# Images
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true
