FROM gcc:14.2.0-bookworm
RUN mkdir -p /usr/app
RUN mkdir -p /usr/app/bin
RUN mkdir -p /usr/app/build
RUN mkdir -p /usr/app/src
COPY ./Makefile /usr/app
COPY ./src /usr/app/src
WORKDIR /usr/app
RUN make
RUN cp ./build/main.o ./bin/main
CMD ["./bin/main"]