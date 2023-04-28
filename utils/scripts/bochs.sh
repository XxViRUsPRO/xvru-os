#!/bin/bash

# check if image is given
if [ -z "$1" ]; then
    echo "Usage: $0 <image>"
    exit 1
fi

# check if image exists
if [ ! -f "$1" ]; then
    echo "Image $1 not found"
    exit 1
fi

# check if bochs is installed
if [ -z "$(which bochs)" ]; then
    echo "Bochs is not installed"
    exit 1
fi

user=$(whoami)

cat << EOF > /tmp/bochsrc
megs: 128
display_library: x, options="gui_debug"
# romimage: file=/usr/share/bochs/BIOS-bochs-latest
# vgaromimage: file=/usr/share/bochs/VGABIOS-lgpl-latest
romimage: file=/home/$user/bochs-2.7/bios/BIOS-bochs-latest
vgaromimage: file=/home/$user/bochs-2.7/bios/VGABIOS-lgpl-latest
floppya: 1_44=$1, status=inserted
boot: floppy
# ata0-master: type=disk, path=$1, mode=flat, cylinders=262, heads=16, spt=32
# boot: disk
mouse: enabled=0
# log: bochsout.txt
EOF

bochs -qf /tmp/bochsrc
rm /tmp/bochsrc