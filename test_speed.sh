#!/bin/bash

filename=$(date +%s)
dest=$1
if [ -n "$1" ]; then
    echo "=== Full speed test ==="
else
    echo "Please, specify the directory for test file"
    exit 0
fi

echo "Write test..."
WRITE_OUTPUT=$(sudo dd if=/dev/zero of=${dest}/${filename}.zov bs=1M count=1024 oflag=direct 2>&1 | tail -1)
WRITE_SPEED=$(echo "$WRITE_OUTPUT" | awk '{print $10 $11}')

sleep 2
echo "Dropping caches"
sudo sync && sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'

echo "Read test..."
READ_OUTPUT=$(sudo dd if=${dest}/${filename}.zov of=/dev/null bs=1M iflag=direct 2>&1 | tail -1)
READ_SPEED=$(echo "$READ_OUTPUT" | awk '{print $10 $11}')

echo ""
echo "Writing $WRITE_SPEED, Reading $READ_SPEED"

sudo rm -f ${dest}/${filename}.zov