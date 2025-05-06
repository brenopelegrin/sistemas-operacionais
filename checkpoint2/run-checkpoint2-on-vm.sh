#!/bin/bash
# Automatically run the checkpoint2 when logged in the VM
cd ~/ && \
docker build -t so-icmc/checkpoint2:latest ~/projeto-ssc0541-grupo-06/checkpoint2/ && \
docker rm -f checkpoint2 && docker run --name checkpoint2 so-icmc/checkpoint2:latest