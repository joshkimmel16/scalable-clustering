### Build/test container ###
FROM scalable-clustering:base as builder

COPY . /usr/src/project
WORKDIR /usr/src/project/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure


### Deploy container ###
FROM ubuntu:latest as deploy

# TODO: copy build output from builder 
# TODO: install main executable globally?
