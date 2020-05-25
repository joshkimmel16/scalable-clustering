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
unsigned int Compare (DataPoint* anchor, DataPoint* input, unsigned int index, DataType type) {
    switch (type) {
        case DATA_STRING:
            return (input->GetStringValue(index) <= anchor->GetStringValue(index)) ? 1 : 2;
        case DATA_INT:
            return (input->GetIntValue(index) <= anchor->GetIntValue(index)) ? 1 : 2;
        case DATA_DOUBLE:
            return (input->GetDoubleValue(index) <= anchor->GetDoubleValue(index)) ? 1 : 2;
        default:
            return 0;
    }
}

#endif