// Define object to represent generic data point to be parsed

#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>

//generic DataPoint class
class DataPoint {
public:
    DataPoint(unsigned int len); //constructor (requires knowledge of # of parameters)
    DataPoint(const DataPoint& dp); // copy constructor
    ~DataPoint(); //destructor
    void PopulateRawData(std::string raw []); //take an array of strings and set attributes appropriately
    void SetStringValue(unsigned int index, std::string str); // set string data parameter
    std::string GetStringValue(unsigned int index); //retrieve string data parameter
    int GetIntValue(unsigned int index); //retrieve int data parameter
    double GetDoubleValue (unsigned int index); //retrieve double data parameter
private:
    std::string* raw_data;
    unsigned int raw_data_len;
};

#endif