FROM python:3.10-slim-bullseye
EXPOSE 8080
COPY . /app
WORKDIR /app
ENV DATABASE_URL postgres://postgres:password@localhost:5432/hrmanager
ENV AMQP_URL redis://localhost:6379
ENV FRONTEND_URL http://localhost:5173
# In case we're running with gunicorn:
#ENV GUNICORN_WORKERS 3
#ENV GUNICORN_THREADS 1
#WORKDIR /app
#ENTRYPOINT gunicorn --conf /app/gunicorn.conf.py app:app
RUN pip install --no-cache-dir -r /app/requirements.txt
ENTRYPOINT ["flask", "--debug", "-A", "app.py", "run", "-h", "0.0.0.0", "-p", "8080"]