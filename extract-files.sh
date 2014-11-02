#!/bin/bash

# Use tradition sort
export LC_ALL=C

FP=$(dirname $(readlink -f $0))
export VENDOR=$(basename $(dirname $FP))
export DEVICE=$(basename $FP)
export BOARDCONFIGVENDOR=true

../common/extract-files.sh $@

../common/setup-makefiles.sh
