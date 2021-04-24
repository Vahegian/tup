FROM ubuntu:20.04

COPY ./* ./tup/

WORKDIR /tup

RUN apt-get update && apt-get upgrade -y && apt-get install -y curl && curl https://sh.rustup.rs -sSf 

CMD ./tup-support-server ./config.json