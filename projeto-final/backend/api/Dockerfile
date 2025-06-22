FROM python:3.10-slim-bullseye
COPY . /app
COPY ./jwtRS256.key /tmp/jwtRS256.key
COPY ./jwtRS256.key.pub /tmp/jwtRS256.key.pub
ARG pytasks_taskpacks_manager_INSTALL_PACKAGES
ENV DATABASE_URL postgres://postgres:password@localhost:5432/pytasks
ENV AMQP_URL redis://localhost:6379
ENV FRONTEND_URL *
ENV GUNICORN_WORKERS 3
ENV GUNICORN_THREADS 2
EXPOSE 8080
RUN pip install --no-cache-dir -r /app/requirements.txt
# For this project, we are shipping packages with the container using COPY instead of installing it on build time
#RUN python /app/tasks/manager/manager.py -i physicsjs test soicmc --api /app
WORKDIR /app
ENTRYPOINT ["/bin/sh", "-c", "export JWT_PRIVATE_PEM=\"$(cat /tmp/jwtRS256.key)\" && export JWT_PUBLIC_PEM=\"$(cat /tmp/jwtRS256.key.pub)\" && exec gunicorn --config /app/gunicorn.conf.py app:app"]

