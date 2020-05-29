#ifndef CONFIG_H
#define CONFIG_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "compare.h"

enum ParserState {
    DATA_START,
    DATA_INDICES,
    NUM_ATTRS,
    DATA_TYPES,
    CUTOFF_VALS,
    DATA_PATH,
    REPORT_PATH
};
ParserState StateFromString (std::string state); //get state from string

class Config {
public:
    Config(); //constructor
    ~Config(); //destructor
    void SetDataStart (std::string ds); //setter for data_start
    bool AddDataIndex (std::string index); //setter for new data index
    void SetNumAttrs (std::string na); //setter for num_attrs
    bool AddDataType (std::string dt); //setter for new data type
    bool AddCutoffVals (std::string cvs); //setter for new cutoff AddCutoffVals
    unsigned int IncrementCurrIndex (); //update current_index
    void SetDataPath (std::string dp); //setter for data_path
    void SetReportPath (std::string rp); //setter for report_path
    unsigned int GetDataStart (); //return data_start
    int* GetDataIndices(); //return data_indices
    unsigned int GetNumAttrs(); //return num_attrs
    DataType GetDataType(unsigned int index); //retrieve type at given index
    std::string* GetCutoffs(unsigned int index); //retrieve cutoff_vals at given index
    unsigned int GetCutoffsLength(unsigned int index); //retrieve cutoff_val_lens at given index
    std::string GetDataPath(); //retrieve data_path
    std::string GetReportPath(); //retrieve report_path
private:
    unsigned int data_start;
    int* data_indices;
    unsigned int num_attrs;
    DataType* types;
    std::string** cutoff_vals;
    unsigned int* cutoff_val_lens;
    unsigned int current_index;
    std::string data_path;
    std::string report_path;
};

std::vector<std::string> ReadLine (std::istream* input); //read a line from the config file
void SetConfigVal (std::vector<std::string> line, Config* config); //method to set a config value appropriately based on parsed line
bool Parse(std::istream* config_file, Config* config); //actual parse function to extract config values from config file
bool Parse(const char* file_name, Config* config); //high-level parse function that takes config path as a string

#endif