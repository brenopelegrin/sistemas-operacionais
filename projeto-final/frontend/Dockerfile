FROM node:18-alpine
EXPOSE 3000
COPY . /app
WORKDIR /app

# Instala dependencias
RUN npm i -g --force yarn
RUN yarn install --frozen-lockfile
RUN yarn build

# Serve o site
CMD [ "yarn", "preview", "--port", "3000", "--host"]