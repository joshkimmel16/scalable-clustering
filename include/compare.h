// Define extensible library for complex data types with compare functions

#ifndef COMPARE_H
#define COMPARE_H

#include "datapoint.h"

//define possible data parameter types
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS ENUM
enum DataType {
    DATA_STRING, 
    DATA_INT,
    DATA_DOUBLE,
    DATA_DATE
};

//define valid date formats
enum DateFormat {
    MONTH_DAY_YEAR
};

struct Date {
    Date(std::string d, DateFormat format); //construct date from string
    bool compare(Date& d); //compare to another date object
    unsigned int month;
    unsigned int day;
    unsigned int year;
};

//generic compare method to help traverse the ClusterGraph
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION (IN .CC FILE)
unsigned int Compare (DataPoint* anchor, DataPoint* input, unsigned int index, DataType type);

//return a DataType value from a string representation
//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION (IN .CC FILE)
DataType FromString (std::string dt);

#endif