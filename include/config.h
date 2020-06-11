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
    REPORT_PATH,
    THRESHOLD,
    NULL_ACTION,
    DEFAULT_VALS,
    BATCH_SIZE
};
ParserState StateFromString (std::string state); //get state from string

enum NullAction {
    ACTION_OMIT,
    ACTION_DEFAULT
};
NullAction ActionFromString (std::string action); //get action from string

class Config {
public:
    Config(); //constructor
    ~Config(); //destructor
    void SetDataStart (std::string ds); //setter for data_start
    bool AddDataIndex (std::string index); //setter for new data index
    void SetNumAttrs (std::string na); //setter for num_attrs
    bool AddDataType (std::string dt); //setter for new data type
    bool AddCutoffVals (std::string cvs); //setter for new cutoff AddCutoffVals
    void SetDataPath (std::string dp); //setter for data_path
    void SetReportPath (std::string rp); //setter for report_path
    void SetThreshold (std::string t); //setter for threshold
    void SetNullAction (std::string na); //setter for null_action
    bool AddDefaultVal (std::string dv); //setter for default_vals
    void SetBatchSize (std::string bs); //setter for batch size
    unsigned int GetDataStart (); //return data_start
    std::vector<unsigned int> GetDataIndices(); //return data_indices
    unsigned int GetNumAttrs(); //return num_attrs
    DataType GetDataType(unsigned int index); //retrieve type at given index
    std::vector<std::string> GetCutoffs(unsigned int index); //retrieve cutoff_vals at given index
    std::string GetDataPath(); //retrieve data_path
    std::string GetReportPath(); //retrieve report_path
    double GetThreshold(); //retrieve threshold
    NullAction GetNullAction(); //retrieve null_action
    std::string GetDefaultVal(unsigned int index); //retrieve default_val at given index
    unsigned int GetBatchSize(); //retrieve batch size
private:
    unsigned int data_start;
    std::vector<unsigned int> data_indices;
    unsigned int num_attrs;
    std::vector<DataType> types;
    std::vector<std::vector<std::string>> cutoff_vals;
    std::string data_path;
    std::string report_path;
    double threshold;
    NullAction null_action;
    std::vector<std::string> default_vals;
    unsigned int batch_size;
};

std::vector<std::string> ReadLine (std::istream* input); //read a line from the config file
void SetConfigVal (std::vector<std::string> line, Config* config); //method to set a config value appropriately based on parsed line
bool Parse(std::istream* config_file, Config* config); //actual parse function to extract config values from config file
bool Parse(const char* file_name, Config* config); //high-level parse function that takes config path as a string

#endif