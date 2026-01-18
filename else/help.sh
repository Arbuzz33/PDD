#!/bin/bash

function write_info {
    disk_num=1
    info="some"
    df -h > info.txt
    mkdir disks
    
    while [ ! -z "$(awk "/disk$disk_num/" info.txt)" ]
    do
        awk "/disk$disk_num/" info.txt > "disks/$disk_num.txt"
        disk_num=$((disk_num+1))
    done
    echo "Done"
}

write_info
cat info.txt
ls disks

exit 0