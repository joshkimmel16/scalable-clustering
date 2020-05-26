// Define extensible library for complex data types with compare functions

#ifndef COMPARE_H
#define COMPARE_H

//define possible data parameter types
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS ENUM
enum DataType {
    DATA_STRING, 
    DATA_INT,
    DATA_DOUBLE 
};

//generic compare method to help traverse the ClusterGraph
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION (IN .CC FILE)
unsigned int Compare (DataPoint* anchor, DataPoint* input, unsigned int index, DataType type);

//return a DataType value from a string representation
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION (IN .CC FILE)
DataType FromString (std::string dt);

#endif