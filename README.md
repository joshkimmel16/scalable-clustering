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
* Run the production/example container to ensure it is working properly. Note that these containers currently have no entry point and thus will need one specified in the command line if they are meant to run persistently. The covid container is meant to be used only to generate a report on the latest COVID data set (see next step for how to retrieve that report).
```
docker run --name scalable-clustering -d scalable-clustering:[latest/covid]
```
* (Optional) Retrieve report from COVID docker file by copying it from the running COVID container.
```
docker cp scalable-clustering:/usr/cluster/covid_report.txt [LOCAL_PATH]/covid_report.txt
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
BATCH_SIZE=5000
NULL_ACTION=ACTION_OMIT
REPORT_PATH=covid_report.txt

```

### Config Parameters

* _DATA_START_ => The line of the input data set file that contains row 1 of the data (Int)
* _DATA_INDICES_ => The columns (starting at 0) of the input data set to use for the clustering algorithm (Comma-Separated List of Ints)
* _NUM_ATTRS_ => The number of columns (dimensions) that are used for the clustering algorithm (Int)
* _DATA_TYPES_ => The types of each column that is used for the clustering algorithm (Comma-Separated list of Strings). See below for a list of supported data types.
* _CUTOFF_VALS_ => The cutoff values for each column that define the clusters for the algorithm. Each array is interpreted as a binary tree in array form. (Comma-Separated list of Arrays of Strings)
* _DATA_PATH_ => Path to the input data set file (String)
* _THRESHOLD_ => Threshold value to use to identify "interesting" clusters (Double)
* _BATCH_SIZE_ => Number of rows to read into the cluster graph at any given time. Default=1000 (Int, Optional)
* _NULL_ACTION_ => Defines how to handle missing/null values for rows in the data set. See below for a list of supported actions (String)
* _DEFAULT_VALS_ => The values to use as defaults for each dimension when a null/missing value is encountered. Only honored if NULL_ACTION is ACTION_DEFAULT (Comma-Separated List of Strings)
* _REPORT_PATH_ => Path to the output report file (String)

#### Supported Data Types

* DATA_STRING (treated as std::string for comparisons)
* DATA_INT (treated as int for comparisons)
* DATA_DOUBLE (treated as double for comparisons)
* DATA_DATE (assumed to be m/d/yyyy)

#### Supported Null Actions

* ACTION_OMIT (skip any row where any dimension is null/missing)
* ACTION_DEFAULT (replace all null/missing values with a default specified in DEFAULT_VALS)

## Authors

* **Daniel Achee**
* **Can Aygun**
* **Bobby Judd**
* **Josh Kimmel**
* **Zixuan Zhong**