#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include "datapoint.h"
#include "config.h"
#include "clustergraph.h"

class DataParser {
public:
    DataParser(Config dataConfig); //constructor 
    ~DataParser(); //destructor
    void LoadInputData();

private:
    Config dataConfig; // Config of input dataset to load
    unsigned int numAttributes; // Number of columns per data point retrieved from Config
    std::vector<std::string> attrNames; // List of column headers
};
#endif