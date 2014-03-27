#!/bin/sh
# hw_config.sh.
# Used to set special parameters.

# Proximity sensor configuration
dev=/sys/bus/i2c/devices/12-0054/
val_cycle=0
val_nburst=2
val_freq=2
val_threshold=0
val_filter=0

#===>tempory solution
hwid0=12
hwid1=13
hwid2=14
hwid3=15
echo $hwid0 > /sys/class/gpio/export
echo $hwid1 > /sys/class/gpio/export
echo $hwid2 > /sys/class/gpio/export
echo $hwid3 > /sys/class/gpio/export
hwidvalue=`cat /sys/class/gpio/gpio$hwid3/value`
hwidvalue=$hwidvalue`cat /sys/class/gpio/gpio$hwid2/value`
hwidvalue=$hwidvalue`cat /sys/class/gpio/gpio$hwid1/value`
hwidvalue=$hwidvalue`cat /sys/class/gpio/gpio$hwid0/value`
case $hwidvalue in
#DP
 "0001")
#  echo "in DP" > /tmp/abcdef.txt
  val_cycle=0
  val_nburst=4
  val_freq=3
  val_threshold=0
  val_filter=0
  ;;
#SP
 "0010")
#  echo "in SP" > /tmp/abcdef.txt
  val_cycle=0
  val_nburst=2
  val_freq=2
  val_threshold=0
  val_filter=0
  ;;
#others
  *)
#  echo $hwidvalue > /tmp/abcdef.txt
  ;;
esac
echo $hwid0 > /sys/class/gpio/unexport
echo $hwid1 > /sys/class/gpio/unexport
echo $hwid2 > /sys/class/gpio/unexport
echo $hwid3 > /sys/class/gpio/unexport
#<===tempory solution

ta_param_loader 60240 prox_cal
val_calibrated=$?
case $val_calibrated in
 1)
  ta_param_loader 60240 threshold
  val_threshold=$?
  ta_param_loader 60240 rfilter
  val_filter=$?
  ;;
esac

echo $val_cycle > $dev/cycle    # Duration Cycle. Valid range is 0 - 3.
echo $val_nburst > $dev/nburst  # Number of pulses in burst. Valid range is 0 - 15.
echo $val_freq > $dev/freq      # Burst frequency. Valid range is 0 - 3.
echo $val_threshold > $dev/threshold # sensor threshold. Valid range is 0 - 15 (0.12V - 0.87V)
echo $val_filter > $dev/filter  # RFilter. Valid range is 0 - 3.

# ALS configuration
dev=/sys/bus/i2c/devices/10-0040/
echo 0x91=0x40 > $dev/debug
