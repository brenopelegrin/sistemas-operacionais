FROM gcc:14.2.0-bookworm AS buildstage
LABEL stage="builder"
COPY entrypoint.sh /usr/app/checkpoint1/entrypoint.sh
RUN mkdir -p /usr/app/checkpoint1
RUN mkdir -p /usr/app/checkpoint1/bin
RUN mkdir -p /usr/app/checkpoint1/build
RUN mkdir -p /usr/app/checkpoint1/src
COPY ./Makefile /usr/app/checkpoint1
COPY ./src /usr/app/checkpoint1/src
WORKDIR /usr/app/checkpoint1

# Compile each syscall module
RUN make io-and-files && cp ./build/io-and-files.o ./bin/io-and-files
RUN make memory && cp ./build/memory.o ./bin/memory
RUN make processes && cp ./build/processes.o ./bin/processes

# Compile each demonstration program
RUN make cpu-bound && cp ./build/cpu-bound.o ./bin/cpu-bound
RUN make io-bound && cp ./build/io-bound.o ./bin/io-bound
RUN make clean

FROM ubuntu:24.04
RUN apt-get update -yq && apt-get install -yq time strace
COPY --from=buildstage /usr/app/checkpoint1 /usr/app/checkpoint1
ENTRYPOINT ["/bin/bash", "/usr/app/checkpoint1/entrypoint.sh"]