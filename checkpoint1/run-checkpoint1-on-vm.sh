#!/bin/bash
# Automatically run the checkpoint1 when logged in the VM
cd ~/ && \
bash ~/deploy.sh && \
docker build -t so-icmc/checkpoint1:latest ~/projeto-ssc0541-grupo-06/checkpoint1/ && \
docker rm -f checkpoint1 && docker run --name checkpoint1 so-icmc/checkpoint1:latest