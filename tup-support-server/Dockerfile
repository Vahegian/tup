FROM ubuntu:20.04

RUN apt-get update && apt-get upgrade -y && apt-get install -y curl && curl https://sh.rustup.rs -sSf 

COPY ./* ./tup/

WORKDIR /tup

CMD ./tup-support-server ./config.json