# Scalable Clustering

## Docker Instructions

The following section details how to prepare the production docker container.

* Install Docker on the machine building the container (see https://docs.docker.com/v17.09/engine/installation/).
* Browse to the root directory of the application.
* Build the base container. This container is used solely to execute the necessary npm build commands.
```
docker build -t scalable-clustering:base -f docker/dockerfile_build .
```
* Build the main container. This is the container to be deployed for production use.
```
docker build -t scalable-clustering:latest -f docker/Dockerfile .
```
* Run the production container to ensure it is working properly.
```
docker run --name scalable-clustering -d scalable-clustering:latest
```

## Authors

* **Daniel Achee**
* **Can Aygun**
* **Bobby Judd**
* **Josh Kimmel**
* **Zixuan Zhong**