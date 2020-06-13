#include "compare.h"

//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION
unsigned int Compare (DataPoint* anchor, DataPoint* input, unsigned int index, DataType type) {
    switch (type) {
        case DATA_STRING:
            return (input->GetStringValue(index) <= anchor->GetStringValue(index)) ? 1 : 2;
        case DATA_INT:
            return (input->GetIntValue(index) <= anchor->GetIntValue(index)) ? 1 : 2;
        case DATA_DOUBLE:
            return (input->GetDoubleValue(index) <= anchor->GetDoubleValue(index)) ? 1 : 2;
        case DATA_DATE:
        {
            Date d1 = Date(input->GetStringValue(index), YEAR_MONTH_DAY);
            Date d2 = Date(anchor->GetStringValue(index), YEAR_MONTH_DAY);
            return (d1.compare(d2)) ? 1 : 2;
        }
        default:
            return 0;
    }
}

//TO SUPPORT A NEW DATA TYPE, EXTEND THIS FUNCTION
DataType FromString (std::string dt) {
    if (dt == "DATA_STRING") {
        return DATA_STRING;
    }
    else if (dt == "DATA_INT") {
        return DATA_INT;
    }
    else if (dt == "DATA_DOUBLE") {
        return DATA_DOUBLE;
    }
    else if (dt == "DATA_DATE") {
        return DATA_DATE;
    }
    else {
        return DATA_STRING;
    }
}

//this method assumes that inputs are valid
//the data set should be sanitized appropriately
Date::Date(std::string d, DateFormat format) {
    switch (format) {
        //assumes date is of form M/dd/yyyy
        case MONTH_DAY_YEAR:
        {
            unsigned int state = 0;
            std::string tmp = "";
            for (unsigned int i=0; i<d.length(); i++) {
                if (d[i] == '/') {
                    if (state == 0) { month = std::stoi(tmp); tmp = ""; state++; }
                    else if (state == 1) { day = std::stoi(tmp); tmp = ""; state++; }
                }
                else { tmp += d[i]; }
            }
            year = std::stoi(tmp);
            break;
        }
        case YEAR_MONTH_DAY:
        {
            unsigned int state = 0;
            std::string tmp = "";
            for (unsigned int i=0; i<d.length(); i++) {
                if (d[i] == '-') {
                    if (state == 0) { year = std::stoi(tmp); tmp = ""; state++; }
                    else if (state == 1) { month = std::stoi(tmp); tmp = ""; state++; }
                }
                else { tmp += d[i]; }
            }
            day = std::stoi(tmp);
            break;
        }
        //assumes MONTH_DAY_YEAR
        default:
        {
            unsigned int state = 0;
            std::string tmp = "";
            for (unsigned int i=0; i<d.length(); i++) {
                if (d[i] == '/') {
                    if (state == 0) { month = std::stoi(tmp); tmp = ""; state++; }
                    else if (state == 1) { day = std::stoi(tmp); tmp = ""; state++; }
                }
                else { tmp += d[i]; }
            }
            year = std::stoi(tmp);
            break;
        }
    }
}

//returns true if given date is the same or comes before the date passed as an argument
bool Date::compare(Date& d) {
    if (year < d.year) { return true; }
    else if (year == d.year && month < d.month) { return true; }
    else if (year == d.year && month == d.month && day <= d.day) { return true; }
    else { return false; }
}