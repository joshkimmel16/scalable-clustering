### Build/test container ###
FROM scalable-clustering:base as builder

RUN mkdir -p /usr/cluster
COPY . /usr/cluster
RUN mkdir -p /usr/cluster/build
WORKDIR /usr/cluster/build

RUN cmake ..
RUN make
RUN ctest --output-on_failure

### Deploy container ###
FROM scalable-clustering:base as deploy

RUN mkdir /usr/cluster
WORKDIR /usr/cluster

# copy build artifacts from builder
COPY --from=builder /usr/cluster/build/bin/main .
