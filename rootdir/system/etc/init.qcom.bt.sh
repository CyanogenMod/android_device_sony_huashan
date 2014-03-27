#!/system/bin/sh
# Copyright (c) 2009-2013, The Linux Foundation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of The Linux Foundation nor
#       the names of its contributors may be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

BLUETOOTH_SLEEP_PATH=/proc/bluetooth/sleep/proto
LOG_TAG="sony-bluetooth"
LOG_NAME="${0}:"

loge ()
{
  /system/bin/log -t $LOG_TAG -p e "$LOG_NAME $@"
}

logi ()
{
  /system/bin/log -t $LOG_TAG -p i "$LOG_NAME $@"
}

failed ()
{
  loge "$1: exit code $2"
  exit $2
}

# Note that "hci_qcomm_init -e" prints expressions to set the shell variables
# BTS_DEVICE, BTS_TYPE, BTS_BAUD, and BTS_ADDRESS.

# BR/EDR & LE power class configurations
POWER_CLASS=`getprop qcom.bt.dev_power_class`
LE_POWER_CLASS=`getprop qcom.bt.le_dev_pwr_class`

#find the transport type
TRANSPORT=`getprop ro.qualcomm.bt.hci_transport`
logi "Transport : $TRANSPORT"

#load bd addr
BDADDR=`/system/bin/cat /data/etc/bluetooth_bdaddr`

setprop bluetooth.status off

logi "BDADDR: $BDADDR"

case $POWER_CLASS in
  1) PWR_CLASS="-p 0" ;
     logi "Power Class: 1";;
  2) PWR_CLASS="-p 1" ;
     logi "Power Class: 2";;
  3) PWR_CLASS="-p 2" ;
     logi "Power Class: CUSTOM";;
  *) PWR_CLASS="";
     logi "Power Class: Ignored. Default(1) used (1-CLASS1/2-CLASS2/3-CUSTOM)";
     logi "Power Class: To override, Before turning BT ON; setprop qcom.bt.dev_power_class <1 or 2 or 3>";;
esac

case $LE_POWER_CLASS in
  1) LE_PWR_CLASS="-P 0" ;
     logi "LE Power Class: 1";;
  2) LE_PWR_CLASS="-P 1" ;
     logi "LE Power Class: 2";;
  3) LE_PWR_CLASS="-P 2" ;
     logi "LE Power Class: CUSTOM";;
  *) LE_PWR_CLASS="-P 1";
     logi "LE Power Class: Ignored. Default(2) used (1-CLASS1/2-CLASS2/3-CUSTOM)";
     logi "LE Power Class: To override, Before turning BT ON; setprop qcom.bt.le_dev_pwr_class <1 or 2 or 3>";;
esac

if [ -z "$BDADDR" ]
then
  logwrapper /system/bin/hci_qcomm_init -e $PWR_CLASS $LE_PWR_CLASS -vv
else
  logwrapper /system/bin/hci_qcomm_init -b $BDADDR -e $PWR_CLASS $LE_PWR_CLASS -vv
fi

case $? in
  0) logi "Bluetooth QSoC firmware download succeeded, $PWR_CLASS $BDADDR $TRANSPORT";;
  *) failed "Bluetooth QSoC firmware download failed" $?;
     setprop bluetooth.status off;
     exit $?;;
esac

setprop bluetooth.status on

logi "start bluetooth smd transport"

exit 0
