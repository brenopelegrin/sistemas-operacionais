#!/bin/bash
# Automatically run the checkpoint3 when logged in the VM
cd ~/ && \
docker build -t so-icmc/checkpoint3:latest ~/projeto-ssc0541-grupo-06/checkpoint3/ && \
docker rm -f checkpoint3 && docker run --name checkpoint3 so-icmc/checkpoint3:latest