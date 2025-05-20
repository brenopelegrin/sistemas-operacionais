#!/bin/bash
echo "Showing the input data (/usr/app/data/input.txt)...";
printf "\n";

# show input data
cat /usr/app/data/input.txt;
printf "\n";

echo "Starting the checkpoint3 main program (/usr/app/bin/main)...";
printf "\n";

# main program
/usr/app/bin/main;