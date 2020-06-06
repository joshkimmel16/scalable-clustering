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

## Testing

* Ensure all dependencies are installed on the target machine (see Dockerfile for a list, namely googletest).
* Browse to _build_ directory from the root of the repository.
``` cd build ```
* Run the following command to execute all unit tests:
``` make test ```

## Docker Instructions

The following section details how to prepare the production docker container.

* Install Docker on the machine building the container (see https://docs.docker.com/v17.09/engine/installation/).
* Browse to the root directory of the application.
* Build the base container. This container is used solely to install dependencies.
```
docker build -t scalable-clustering:base -f docker/base.Dockerfile .
```
* Build the main container. This is the container to be deployed for production use.
```
docker build -t scalable-clustering:latest -f docker/Dockerfile .
```
* Build the example container. This containers runs clustering on a COVID data set (https://www.google.com/covid19/mobility/?fbclid=IwAR3vfMCCY4iFQCB3I-m8BlMGVvfJArJjwupRUVEvaCE7ER6-tQzbZZpP2rY)
```
docker build -t scalable-clustering:covid -f docker/COVID.Dockerfile .
```
* Run the production/example container to ensure it is working properly.
```
docker run --name scalable-clustering -d scalable-clustering:[latest/example]
```

## Configuration

The application takes as input a config file. This config file is passed as a command line argument to the main executable as follows:
```
main [path_to_config]
```

Below is a sample config file, which is explained in detail:
```
DATA_START=1
NUM_ATTRS=3
DATA_INDICES=4,9,10
DATA_TYPES=DATA_DATE,DATA_INT,DATA_INT
CUTOFF_VALS=[],[],[]
DATA_PATH=Global_Mobility_Report.csv
THRESHOLD=80
REPORT_PATH=covid_report.txt

```

### Config Parameters

* _DATA_START_ => The line of the input data set file that contains row 1 of the data (Int)
* _DATA_INDICES_ => The columns (starting at 0) of the input data set to use for the clustering algorithm (Comma-Separated List of Ints)
* _NUM_ATTRS_ => The number of columns (dimensions) that are used for the clustering algorithm (Int)
* _DATA_TYPES_ => The types of each column that is used for the clustering algorithm (Comma-Separated list of Strings). See [TODO] for a list of supported data types.
* _CUTOFF_VALS_ => The cutoff values for each column that define the clusters for the algorithm. Each array is interpreted as a binary tree in array form. (Comma-Separated list of Arrays of Strings)
* _DATA_PATH_ => Path to the input data set file (String)
* _THRESHOLD_ => Threshold value to use to identify "interesting" clusters (Double)
* _REPORT_PATH_ => Path to the output report file (String)

## Authors

* **Daniel Achee**
* **Can Aygun**
* **Bobby Judd**
* **Josh Kimmel**
* **Zixuan Zhong**