#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "datapoint.h"
#include "config.h"
#include "clustergraph.h"

class DataParser {
public:
    DataParser(Config dataConfig, unsigned int batchSize); //constructor 
    ~DataParser(); //destructor
    std::vector<std::string> GetAttrNames(); // Returns column header vector
    void LoadHeaders(); // Reads column headers into attrNames
    unsigned int LoadNextDataBatch(ClusterGraph* outputGraph=nullptr); // Loads next bSize lines into rawData
    std::vector<std::vector<std::string>> GetRawData();

private:
    Config dConf; // Config of input dataset to load
    std::ifstream inputDataFile;
    bool readComplete; // true after the entire csv file has been read, else false
    unsigned int numAttributes; // Number of columns per data point retrieved from Config
    unsigned int bSize; // Internal batch size
    unsigned int lastLine; // Memory of last read line for batch continuation
    std::vector<std::string> attrNames; // List of column headers
    std::vector<std::vector<std::string>> rawData;

};
#endif