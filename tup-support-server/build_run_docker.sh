docker build -t tup . &&
docker run -d --net=host --name tup_container tup