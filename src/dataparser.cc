#include "dataparser.h"

DataParser::DataParser(Config dataConfig) {
    dataConfig = dataConfig;
}

DataParser::~DataParser() {

}

void DataParser::LoadInputData() {
    
    // Open CSV file at filePath
    std::ifstream inputDataFile(dataConfig.GetDataPath());

    unsigned int numAttrs = dataConfig.GetNumAttrs();
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

    // Read data, line by line
    DataPoint* currentDp;
    std::string csvRow[numAttrs];
    int colIdx;
    while(std::getline(inputDataFile, line))
    {
        std::stringstream ss(line);
        colIdx = 0;
        currentDp = new DataPoint(numAttrs);
        while(std::getline(ss, dataField, ',')) {
            csvRow[colIdx++] = dataField;
        }

        // Poplate the DataPoint's raw data
        currentDp->PopulateRawData(csvRow);

        //TODO: Feed to Cluster to be processed
    }

    // Close file
    inputDataFile.close();

}