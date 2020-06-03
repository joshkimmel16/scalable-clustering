#include "dataparser.h"

DataParser::DataParser(Config dataConfig, unsigned int batchSize) {
    dConf = dataConfig;
    bSize = batchSize;
    readComplete = false;

    // Set the first line to read from if specified in the config
    lastLine = std::max(dConf.GetDataStart(), (unsigned int) 1); 
}

DataParser::~DataParser() {

}

void DataParser::LoadHeaders() {
    if(!inputDataFile.is_open())
        inputDataFile.open(dConf.GetDataPath());    // Open CSV file at filePath
    
    numAttributes = dConf.GetNumAttrs();
    attrNames = std::vector<std::string>();
    
    // Make sure the file is open
    if(!inputDataFile.is_open()) throw std::runtime_error("Could not open file");
    std::string line, colname, dataField;
    int val;

    // Read the column headers
    if(inputDataFile.good())
    {
        std::getline(inputDataFile, line);
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ','))
            attrNames.push_back(colname);
    }

    numAttributes = attrNames.size();

    // Close file
    inputDataFile.close();

}

unsigned int DataParser::LoadNextDataBatch() {
    std::string line, dataField;

    if(!readComplete && !inputDataFile.is_open()) {
        // Open CSV file at filePath
        inputDataFile.open(dConf.GetDataPath());
        
        unsigned int lineCount = 0;
        // Scan to the next line of interest
        while(lineCount++ < lastLine && std::getline(inputDataFile, line));
    }
        
    std::vector<std::string> csvRow(numAttributes);
    rawData = std::vector<std::vector<std::string>>(bSize, std::vector<std::string> (numAttributes));
    unsigned int colIdx, batchCount = 0;

    // Read data, line by line
    while(batchCount < bSize && std::getline(inputDataFile, line))
    {
        std::stringstream ss(line);
        colIdx = 0;
        while(std::getline(ss, dataField, ',')) {
            csvRow[colIdx++] = dataField;
        }
        rawData[batchCount++] = csvRow;
        lastLine++;
    }

    if (batchCount != bSize) {
        // Reached the end of the file, close it
        inputDataFile.close();
        readComplete = true;
    }
    
    return batchCount;
}

std::vector<std::string> DataParser::GetAttrNames() {
    return attrNames;
}

std::vector<std::vector<std::string>> DataParser::GetRawData() {
    return rawData;
}