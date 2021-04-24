cargo build --release &&
rm -rf ./build &&
mkdir ./build &&
cp ./target/release/tup-support-server ./build/ &&
cp ./config.json ./build/ &&
cp ./Rocket.toml ./build/ &&
cp ./Dockerfile ./build/ &&
cp ./build_run_docker.sh ./build


