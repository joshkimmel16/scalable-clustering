#include "datapoint.h"

DataPoint::DataPoint(unsigned int len) {
    raw_data = new std::string[len];
    raw_data_len = len;
}

DataPoint::DataPoint(const DataPoint &dp) {
    raw_data_len = dp.raw_data_len;
    raw_data = new std::string[raw_data_len];
    for (int i = 0; i < raw_data_len; i++) {
        raw_data[i] = dp.raw_data[i];
    }

}

DataPoint::~DataPoint() {
    delete[] raw_data;
}

void DataPoint::PopulateRawData(std::string raw[]) {
    for (unsigned int i = 0; i < raw_data_len; i++) {
        raw_data[i] = raw[i];
    }
}

void DataPoint::SetStringValue(unsigned int index, std::string str) {
    raw_data[index] = str;
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



