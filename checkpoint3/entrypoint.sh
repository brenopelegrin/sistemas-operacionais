#!/bin/bash
echo "Checkpoint3"
printf "\n";
echo "Showing the input data (/usr/app/checkpoint3/data/input.txt)...";
printf "\n";

# show input data
cat /usr/app/checkpoint3/data/input.txt;
printf "\n";

echo "Starting the checkpoint3 main program (/usr/app/checkpoint3/bin/main)...";
printf "\n";

# main program
/usr/app/checkpoint3/bin/main --page_size 4096 --frame_size 4096 --logic_address_size 16 --num_frames 8 /usr/app/checkpoint3/data/input.txt;