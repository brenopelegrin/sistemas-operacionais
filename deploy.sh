#!/bin/bash
echo "Starting deploy of the repository..."
echo "PLEASE MAKE SURE TO SETUP SSH KEYS PREVIOUSLY!"
printf "\n"
echo "[pre-deploy] showing versions of docker and git..."
git --version
docker --version
printf "\n"
echo "[1] Removind old data."
rm -rf ~/projeto-ssc0541-grupo-06
printf "\n"
echo "[2] Cloning repository into '~/projeto-ssc0541-grupo-06'."
printf "\n"
git clone git@github.com:ICMC-SSC0541-2025/projeto-ssc0541-grupo-06.git ~/projeto-ssc0541-grupo-06
printf "\n"
echo "[3] Resetting HEAD to the tag projeto-final-submission."
printf "\n"
cd projeto-ssc0541-grupo-06 && git fetch --all && git checkout main && git reset --hard projeto-final-submission
printf "\n"
echo "[DONE] Repository cloned and set up."
echo "The repository is available at '~/projeto-ssc0541-grupo-06'."
echo "The instructions to run the project are located at '~/projeto-ssc0541-grupo-06/README.md'."
echo "PLEASE READ the instructions to understand the architecture and how to run."