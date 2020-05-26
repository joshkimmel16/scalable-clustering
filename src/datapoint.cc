#include "datapoint.h"

DataPoint::DataPoint(unsigned int len) {
    raw_data = new std::string [len];
    raw_data_len = len;
}

DataPoint::~DataPoint() {
    delete [] raw_data;
}

void DataPoint::PopulateRawData(std::string raw []) {
    for (unsigned int i=0; i<raw_data_len; i++) {
        raw_data[i] = raw[i];
    }
}

std::string DataPoint::GetStringValue(unsigned int index) {
    return raw_data[index];
}

int DataPoint::GetIntValue(unsigned int index) {
    return std::stoi(raw_data[index]);
}

double DataPoint::GetDoubleValue(unsigned int index) {
    return std::stod(raw_data[index]);
}