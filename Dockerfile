FROM node:alpine

RUN apk add --no-cache git openssh

RUN mkdir /app

WORKDIR /app

COPY yarn*.lock ./

COPY package*.json ./

RUN yarn

# COPY .env ./

COPY tsconfig.json ./

COPY . ./

RUN yarn build

CMD ["yarn", "start"]