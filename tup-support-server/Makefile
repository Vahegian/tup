run:
	docker build -t tup . && \
	docker rm -f tup_container && \
	docker run --rm -it -p 5666:5666 --name tup_container tup

publish:
		docker build -t registry.anspar.io/tup:latest .
		docker push registry.anspar.io/tup:latest