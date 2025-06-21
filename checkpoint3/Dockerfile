FROM gcc:14.2.0-bookworm AS buildstage
LABEL stage="builder"
COPY entrypoint.sh /usr/app/checkpoint3/entrypoint.sh
RUN mkdir -p /usr/app/checkpoint3
RUN mkdir -p /usr/app/checkpoint3/bin
RUN mkdir -p /usr/app/checkpoint3/build
RUN mkdir -p /usr/app/checkpoint3/src
RUN mkdir -p /usr/app/checkpoint3/data
COPY ./Makefile /usr/app/checkpoint3
COPY ./src /usr/app/checkpoint3/src
COPY ./data /usr/app/checkpoint3/data
WORKDIR /usr/app/checkpoint3

# Compile the program
RUN make && chmod +x ./build/main.o && cp ./build/main.o ./bin/main
RUN make clean

FROM ubuntu:24.04
COPY --from=buildstage /usr/app/checkpoint3 /usr/app/checkpoint3
ENTRYPOINT ["/bin/bash", "/usr/app/checkpoint3/entrypoint.sh"]