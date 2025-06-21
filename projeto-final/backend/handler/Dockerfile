FROM build_checkpoint1 AS build_checkpoint1
FROM build_checkpoint2 AS build_checkpoint2
FROM build_checkpoint3 AS build_checkpoint3

FROM ubuntu:22.04 AS build_worker

RUN mkdir -p /usr/app/checkpoint1 && mkdir -p /usr/app/checkpoint2 && mkdir -p /usr/app/checkpoint3

# Setup dependencies from other build stages
COPY --from=build_checkpoint1 /usr/app/checkpoint1 /usr/app/checkpoint1
COPY --from=build_checkpoint2 /usr/app/checkpoint2 /usr/app/checkpoint2
COPY --from=build_checkpoint3 /usr/app/checkpoint3 /usr/app/checkpoint3

RUN chmod +x /usr/app/checkpoint1/entrypoint.sh && chmod +x /usr/app/checkpoint2/entrypoint.sh && chmod +x /usr/app/checkpoint3/entrypoint.sh 

# Setup handler files
COPY . /app

# Setup other dependencies for running checkpoint1, checkpoint2, checkpoint3
RUN apt-get update -yq
RUN apt-get install -yq strace time build-essential python3 python3-pip software-properties-common
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test
RUN apt-get update -yq
RUN apt-get install -yq --only-upgrade libstdc++6

# Setup Python-related things
ARG pytasks_taskpacks_manager_INSTALL_PACKAGES
ENV DATABASE_URL postgres://postgres:password@localhost:5432/pytasks
ENV AMQP_URL redis://localhost:6379
ENV WORKER_1_NAME worker1
ENV WORKER_2_NAME worker2
ENV ENVIRONMENT handler
ENV DEFAULT_TASK_QUEUE tasks
RUN python3 -m pip install --no-cache-dir -r /app/requirements.txt
# For this project, we are shipping packages with the container using COPY instead of installing it on build time
#RUN python /app/tasks/manager/manager.py -i physicsjs test soicmc --handler /app

# Setup entrypoint
WORKDIR /app
RUN cd /app
ENTRYPOINT celery --app=celeryapp worker --loglevel=INFO -c 2 --hostname=$WORKER_1_NAME@%h
