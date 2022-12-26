FROM rust:1.66.0-buster

COPY ./src /build/src
COPY ./Cargo.toml /build

RUN cd /build && apt-get update && apt-get upgrade -y && ls && cargo build --release


FROM debian:buster
RUN apt-get update && apt-get upgrade -y && apt install -y ca-certificates libssl-dev && rm -rf /var/lib/apt/lists/*
 
COPY --from=0 /build/target/release/tup-support-server /tup/
COPY Rocket.toml /tup/
COPY config.json /tup/

# WORKDIR /tup

CMD cd /tup && ./tup-support-server ./config.json