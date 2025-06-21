FROM gcc:14.2.0-bookworm AS buildstage
LABEL stage="builder"
COPY entrypoint.sh /usr/app/checkpoint2/entrypoint.sh
RUN mkdir -p /usr/app/checkpoint2
RUN mkdir -p /usr/app/checkpoint2/bin
RUN mkdir -p /usr/app/checkpoint2/build
RUN mkdir -p /usr/app/checkpoint2/src
COPY ./Makefile /usr/app/checkpoint2
COPY ./src /usr/app/checkpoint2/src
WORKDIR /usr/app/checkpoint2

# Compile the program
RUN make && cp ./build/main.o ./bin/main
RUN make clean

FROM ubuntu:24.04
COPY --from=buildstage /usr/app/checkpoint2 /usr/app/checkpoint2
ENTRYPOINT ["/bin/bash", "/usr/app/checkpoint2/entrypoint.sh"]