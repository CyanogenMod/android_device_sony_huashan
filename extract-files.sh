#!/bin/bash

export DEVICE=${PWD##*/}
export BOARDCONFIGVENDOR=true

../common/extract-files.sh $@

../common/setup-makefiles.sh
