FROM gcc:14.2.0-bookworm AS buildstage
LABEL stage="builder"
COPY entrypoint.sh /usr/app/entrypoint.sh
RUN mkdir -p /usr/app
RUN mkdir -p /usr/app/bin
RUN mkdir -p /usr/app/build
RUN mkdir -p /usr/app/src
RUN mkdir -p /usr/app/data
COPY ./Makefile /usr/app
COPY ./src /usr/app/src
COPY ./data /usr/app/data
WORKDIR /usr/app

# Compile the program
RUN make && chmod +x ./build/main.o && cp ./build/main.o ./bin/main
RUN make clean

FROM ubuntu:24.04
COPY --from=buildstage /usr/app /usr/app
ENTRYPOINT ["/bin/bash", "/usr/app/entrypoint.sh"]