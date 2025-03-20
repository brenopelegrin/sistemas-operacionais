# checkpoint1

## Building the docker image

Assuming you are currently in the ``checkpoint1`` directory and has **Docker** installed, run:

```bash
docker build -t so-icmc/checkpoint1:latest .
```

Then, the image will be built and saved with the tag ``so-icmc/checkpoint1:latest``.

## Running the docker image

To run the code, first build the Docker image, and then do:

```bash
docker run --name checkpoint1 so-icmc/checkpoint1:latest
```