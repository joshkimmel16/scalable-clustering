### Base environment container ###
# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest as base

# Update the base image and install build environment
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    netcat \
    libboost-regex-dev \
    libboost-log-dev \
#     libboost-signals-dev \ it is not in ubuntu:latest (ubuntu:20.04)
    libboost-system-dev \
    libgtest-dev
