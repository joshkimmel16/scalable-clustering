# Scalable Clustering

## Compiling

* Ensure all dependencies are installed on the target machine (see Dockerfile for a list).
* Create a _build_ directory in the root of the repository. 
```mkdir build && cd build```
* Run _cmake_ from this directory pointing to _CMakeLists.txt_.
    * Note: If any new header files/dependencies/tests are added, _CMakeLists.txt_ must be updated to account for them.
```cmake ..```
* The above command generates a Makefile. The actual building can then be accomplished by executing the default _make_ task of this file.
``` make ```

## Docker Instructions

The following section details how to prepare the production docker container.

* Install Docker on the machine building the container (see https://docs.docker.com/v17.09/engine/installation/).
* Browse to the root directory of the application.
* Build the base container. This container is used solely to install dependencies.
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