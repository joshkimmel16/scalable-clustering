#include "config.h"

Config::Config () {
    current_index = 0;
}

Config::~Config () {
    delete [] data_indices;
    delete [] types;
    for (unsigned int i=0; i<num_attrs; i++) {
        delete [] cutoff_vals[i];
    }
    delete [] cutoff_vals;
    delete [] cutoff_val_lens;
}

void Config::SetDataStart (std::string ds) {
    data_start = std::stoi(ds);
}

bool Config::AddDataIndex (std::string index) {
    if (current_index < num_attrs) {
        data_indices[current_index] = std::stoi(index);
        return true;
    }
    else {
        return false;
    }
}

void Config::SetNumAttrs (std::string na) {
    num_attrs = std::stoi(na);
}

bool Config::AddDataType (std::string dt) {
    if (current_index < num_attrs) {
        data_indices[current_index] = FromString(dt);
        return true;
    }
    else {
        return false;
    }
}

bool Config::AddCutoffVals (std::string cvs) {
    if (current_index < num_attrs) {
        std::vector<std::string> tmp;
        std::string tmpAdd = "";
        for (unsigned int i=0; i<cvs.length(); i++) {
            if (cvs[i] == ',') {
                tmp.push_back(tmpAdd);
                tmpAdd = "";
            }
            else {
                tmpAdd += cvs[i];
            }
        }
        if (tmpAdd != "") {
            tmp.push_back(tmpAdd);
        }
        cutoff_vals[current_index] = new std::string[tmp.size()];
        for (unsigned int j=0; j<tmp.size(); j++) {
            cutoff_vals[current_index][j] = tmp[j];
        }
        cutoff_val_lens[current_index] = tmp.size();
        return true;
    }
    else {
        return false;
    }
}

void Config::IncrementCurrIndex () {
    current_index++;
}

void Config::SetDataPath (std::string dp) {
    data_path = dp;
}

void Config::SetReportPath (std::string rp) {
    report_path = rp;
}

unsigned int Config::GetDataStart () {
    return data_start;
}

unsigned int* Config::GetDataIndices () {
    return data_indices;
}

unsigned int Config::GetNumAttrs () {
    return num_attrs;
}

DataType Config::GetDataType (unsigned int index) {
    return types[index];
}

std::string* Config::GetCutoffs (unsigned int index) {
    return cutoff_vals[index];
}

unsigned int Config::GetCutoffsLength (unsigned int index) {
    return cutoff_val_lens[index];
}

std::string Config::GetDataPath () {
    return data_path;
}

std::string Config::GetReportPath () {
    return report_path;
}