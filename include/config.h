#ifndef CONFIG_H
#define CONFIG_H

#include "compare.h"

class Config {
public:
    Config(); //constructor
    ~Config(); //destructor
    void SetDataStart (std::string ds); //setter for data_start
    bool AddDataIndex (std::string index); //setter for new data index
    void SetNumAttrs (std::string na); //setter for num_attrs
    bool AddDataType (std::string dt); //setter for new data type
    bool AddCutoffVals (std::string cvs); //setter for new cutoff AddCutoffVals
    void IncrementCurrIndex (); //update current_index
    void SetDataPath (std::string dp); //setter for data_path
    void SetReportPath (std::string rp); //setter for report_path
    unsigned int GetDataStart (); //return data_start
    unsigned int* GetDataIndices(); //return data_indices
    unsigned int GetNumAttrs(); //return num_attrs
    DataType GetDataType(unsigned int index); //retrieve type at given index
    std::string* GetCutoffs(unsigned int index); //retrieve cutoff_vals at given index
    unsigned int GetCutoffsLength(unsigned int index); //retrieve cutoff_val_lens at given index
    std::string GetDataPath(); //retrieve data_path
    std::string GetReportPath(); //retrieve report_path
private:
    unsigned int data_start;
    unsigned int* data_indices;
    unsigned int num_attrs;
    DataType* types;
    std::string** cutoff_vals;
    unsigned int* cutoff_val_lens;
    unsigned int current_index;
    std::string data_path;
    std::string report_path;
};

#endif