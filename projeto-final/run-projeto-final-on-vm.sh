#!/bin/bash
# Automatically run the projeto-final when logged in the VM
cd ~/ && \
cd  ~/projeto-ssc0541-grupo-06/projeto-final && \
docker compose down && \
docker compose build --no-cache && \
docker compose up -d