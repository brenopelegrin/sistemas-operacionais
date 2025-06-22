#!/bin/bash
# Run strace and time for each application
echo "Beginning syscalls and timing analysis for each application...";
printf "\n";

# cpu-bound
echo "Running 'strace' for 'cpu-bound':";
strace -c /usr/app/checkpoint1/bin/cpu-bound 2>&1;
printf "\n";
echo "Running 'time' for 'cpu-bound':";
{ /usr/bin/time -v /usr/app/checkpoint1/bin/cpu-bound; } 2>&1;
printf "\n";

# io-bound
echo "Running 'strace' for 'io-bound':";
strace -c /usr/app/checkpoint1/bin/io-bound 2>&1;
printf "\n";
echo "Running 'time' for 'io-bound':";
{ /usr/bin/time -v /usr/app/checkpoint1/bin/io-bound; } 2>&1;
printf "\n";

# memory
echo "Running 'strace' for 'memory':";
strace -c /usr/app/checkpoint1/bin/memory 2>&1;
printf "\n";
echo "Running 'time' for 'memory':";
{ /usr/bin/time -v /usr/app/checkpoint1/bin/memory; } 2>&1;
printf "\n";

# processes
echo "Running 'strace' for 'processes':";
strace -c /usr/app/checkpoint1/bin/processes 2>&1;
printf "\n";
echo "Running 'time' for 'processes':";
{ /usr/bin/time -v /usr/app/checkpoint1/bin/processes; } 2>&1;
printf "\n";

# io-and-files
echo "Running 'strace' for 'io-and-files':";
strace -c /usr/app/checkpoint1/bin/io-and-files 2>&1;
printf "\n";
echo "Running 'time' for 'io-and-files':";
{ /usr/bin/time -v /usr/app/checkpoint1/bin/io-and-files; } 2>&1;
printf "\n";

echo "Finished."
