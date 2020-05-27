#include "compare.h"

//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION
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

//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION
DataType FromString (std::string dt) {
    if (dt == "DATA_STRING") {
        return DATA_STRING;
    }
    else if (dt == "DATA_INT") {
        return DATA_INT;
    }
    else if (dt == "DATA_DOUBLE") {
        return DATA_DOUBLE;
    }
    else {
        return DATA_STRING;
    }
}