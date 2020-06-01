#include "config.h"

Config::Config () {
    data_start = 0;
    num_attrs = 0;
    data_path = "data.csv";
    report_path = "report.txt";
}

Config::~Config () {}

void Config::SetDataStart (std::string ds) {
    data_start = std::stoi(ds);
}

bool Config::AddDataIndex (std::string index) {
    if (data_indices.size() < num_attrs) {
        data_indices.push_back(std::stoi(index));
        return true;
    }
    else {
        return false;
    }
}

void Config::SetNumAttrs (std::string na) {
    num_attrs = std::stoi(na);
    data_indices.clear();
    types.clear();
    cutoff_vals.clear();
}

bool Config::AddDataType (std::string dt) {
    if (types.size() < num_attrs) {
        types.push_back(FromString(dt));
        return true;
    }
    else {
        return false;
    }
}

bool Config::AddCutoffVals (std::string cvs) {
    if (cutoff_vals.size() < num_attrs) {
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
        cutoff_vals.push_back(tmp);
    }
    else {
        return false;
    }
}

void Config::SetDataPath (std::string dp) {
    data_path = dp;
}

void Config::SetReportPath (std::string rp) {
    report_path = rp;
}

void Config::SetThreshold (std::string t) {
    threshold = std::stod(t);
}

unsigned int Config::GetDataStart () {
    return data_start;
}

std::vector<unsigned int> Config::GetDataIndices () {
    return data_indices;
}

unsigned int Config::GetNumAttrs () {
    return num_attrs;
}

DataType Config::GetDataType (unsigned int index) {
    return types[index];
}

std::vector<std::string> Config::GetCutoffs (unsigned int index) {
    return cutoff_vals[index];
}

std::string Config::GetDataPath () {
    return data_path;
}

std::string Config::GetReportPath () {
    return report_path;
}

double Config::GetThreshold () {
    return threshold;
}

ParserState StateFromString (std::string state) {
    if (state == "DATA_START") {
        return DATA_START;
    }
    else if (state == "DATA_INDICES") {
        return DATA_INDICES;
    }
    else if (state == "NUM_ATTRS") {
        return NUM_ATTRS;
    }
    else if (state == "DATA_TYPES") {
        return DATA_TYPES;
    }
    else if (state == "CUTOFF_VALS") {
        return CUTOFF_VALS;
    }
    else if (state == "DATA_PATH") {
        return DATA_PATH;
    }
    else if (state == "REPORT_PATH") {
        return REPORT_PATH;
    }
    else if (state == "THRESHOLD") {
        return THRESHOLD;
    }
    else {
        return DATA_START;
    }
}

std::vector<std::string> ReadLine (std::istream* input) {
    std::vector<std::string> output;
    std::string tmp = "";
    while (input->good()) {
        const char c = input->get();
        if (c == '\n') {
            output.push_back(tmp);
            break;
        }
        else if (c == '=') {
            output.push_back(tmp);
            tmp = "";
        }
        else if (c == '\r') {
            continue;
        }
        else {
            tmp += c;
        }
    }
    return output;
}

void SetConfigVal (std::vector<std::string> line, Config* config) {
    ParserState state = StateFromString(line[0]);
    switch (state) {
        case DATA_START:
        {
            config->SetDataStart(line[1]);
            break;
        }
        case DATA_INDICES:
        {
            std::string tmp1 = "";
            for (unsigned int i=0; i<line[1].length(); i++) {
                if (line[1][i] == ',') {
                    config->AddDataIndex(tmp1);
                    tmp1 = "";
                }
                else {
                    tmp1 += line[1][i];
                }
            }
            if (tmp1 != "") {
                config->AddDataIndex(tmp1);
            }
            break;
        }
        case NUM_ATTRS:
        {
            config->SetNumAttrs(line[1]);
            break;
        }
        case DATA_TYPES:
        {
            std::string tmp2 = "";
            for (unsigned int i=0; i<line[1].length(); i++) {
                if (line[1][i] == ',') {
                    config->AddDataType(tmp2);
                    tmp2 = "";
                }
                else {
                    tmp2 += line[1][i];
                }
            }
            if (tmp2 != "") {
                config->AddDataType(tmp2);
            }
            break;
        }
        case CUTOFF_VALS:
        {
            std::string tmp3 = "";
            bool endVal = false;
            for (unsigned int i=0; i<line[1].length(); i++) {
                if (line[1][i] == '[') {
                    continue;
                }
                else if (line[1][i] == ',' && endVal) {
                    endVal = false;
                    continue;
                }
                else if (line[1][i] == ']') {
                    config->AddCutoffVals(tmp3);
                    tmp3 = "";
                    endVal = true;
                }

                else {
                    tmp3 += line[1][i];
                }
            }
            break;
        }
        case DATA_PATH:
        {
            config->SetDataPath(line[1]);
            break;
        }
        case REPORT_PATH:
        {
            config->SetReportPath(line[1]);
            break;
        }
        case THRESHOLD:
        {
            config->SetThreshold(line[1]);
            break;
        }
        default:
            break;
    }
}

bool Parse(std::istream* config_file, Config* config) {
    while (true) {
        std::vector<std::string> line = ReadLine(config_file);
        if (line.size() < 2) { break; }
        else {
            SetConfigVal(line, config);
        }
    }
    return true;
}

bool Parse(const char* file_name, Config* config) {
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good()) {
    printf ("Failed to open config file: %s\n", file_name);
    return false;
  }

  const bool return_value =
      Parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}