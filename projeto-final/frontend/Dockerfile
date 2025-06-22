FROM node:18-alpine
EXPOSE 3000
COPY . /app
WORKDIR /app

# Instala dependencias
RUN npm i -g --force yarn serve
RUN yarn install --frozen-lockfile
RUN yarn build

# Serve o site
CMD [ "serve", "-s", "dist" ]