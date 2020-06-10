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
    auto validColumns = dConf.GetDataIndices();
    attrNames = std::vector<std::string>();
    
    // Make sure the file is open
    if(!inputDataFile.is_open()) throw std::runtime_error("Could not open file");
    std::string line, colname, dataField;
    unsigned int colIdx = 0;

    // Read the column headers
    if(inputDataFile.good())
    {
        std::getline(inputDataFile, line);
        std::stringstream ss(line);

        // Extract each (unignored) column name
        while (std::getline(ss, colname, ',')) {
            if (colIdx == validColumns[colIdx]) {
                // Remove CR if present
                if (colname.back() == '\r')
                    colname = colname.substr(0, colname.length()-1);
                
                attrNames.push_back(colname);
            }
            colIdx++;
        }
    }

    numAttributes = attrNames.size();

    // Close file
    inputDataFile.close();

}

unsigned int DataParser::LoadNextDataBatch(ClusterGraph* outputGraph) {
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
    auto validColumns = dConf.GetDataIndices();

    // Read data, line by line
    while(batchCount < bSize && std::getline(inputDataFile, line))
    {
        std::stringstream ss(line);
        colIdx = 0;
        bool validLine = true;
        while(std::getline(ss, dataField, ',')) {
            // Is this a column of interest?
            if (colIdx == validColumns[colIdx]) {
                // Remove CR if present
                if (dataField.back() == '\r')
                    dataField = dataField.substr(0, dataField.length()-1);
                
                // Null handling
                if (dataField.empty()) {
                    if (dConf.GetNullAction() == ACTION_OMIT) {
                        validLine = false;
                        break;
                    }
                    dataField = dConf.GetDefaultVal(colIdx);
                }
                csvRow[colIdx] = dataField;
            }
                
            colIdx++;
        }

        // Check for case where last column was null
        if (colIdx != numAttributes && dataField.empty()) {
            if (dConf.GetNullAction() == ACTION_DEFAULT) {
                csvRow[colIdx] = dConf.GetDefaultVal(colIdx);
            }
            else {
                validLine = false;
            }
        }
        
        // Only include row in batch if all values are present
        if (validLine)
            rawData[batchCount++] = csvRow;
        lastLine++;
    }

    if (batchCount != bSize) {
        // Reached the end of the file, close it
        inputDataFile.close();
        readComplete = true;
    }

    // Feed data batch to cluster graph if one is specified
    if (outputGraph != nullptr) {
        for (int i = 0; i < batchCount; i++) {
            DataPoint dp(numAttributes);
            dp.PopulateRawData(rawData[i].data());
            
            // Id is effectively line # in file
            outputGraph->ProcessDataPoint(lastLine-batchCount+i, &dp);
        }
    }
    
    return batchCount;
}

std::vector<std::string> DataParser::GetAttrNames() {
    return attrNames;
}

std::vector<std::vector<std::string>> DataParser::GetRawData() {
    return rawData;
}