FROM ubuntu:18.04
SHELL ["/bin/bash", "-eucx"]

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -yq \
	python-pip \
	make \
    gcc
RUN pip install pwntools

ARG SRC="/src"
COPY ./src "${SRC}"
COPY ./solution.py /

RUN cd "${SRC}" && make
