FROM gcc:14.2.0-bookworm AS buildstage
LABEL stage="builder"
COPY entrypoint.sh /usr/app/entrypoint.sh
RUN mkdir -p /usr/app
RUN mkdir -p /usr/app/bin
RUN mkdir -p /usr/app/build
RUN mkdir -p /usr/app/src
COPY ./Makefile /usr/app
COPY ./src /usr/app/src
WORKDIR /usr/app

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
COPY --from=buildstage /usr/app /usr/app
ENTRYPOINT ["/bin/bash", "/usr/app/entrypoint.sh"]