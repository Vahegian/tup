docker build -t tup . && \
docker rm -f tup_container && \
docker run -d -p 5666:5000 --name tup_container tup