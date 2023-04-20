FROM ubuntu:22.04

# Dependencies
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -yq \
	python3-pip \
	make \
    gcc
RUN pip install pwntools

# Challenge
WORKDIR "/chal"
COPY ./src .
RUN make

# Solution
COPY ./solution.py .
