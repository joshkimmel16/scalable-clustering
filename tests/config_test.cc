#include "gtest/gtest.h"
#include "config.h"

class ConfigTest : public ::testing::Test {
  protected:
    virtual void SetUp() {}
    virtual bool CompareArraysInt (unsigned int* a1, unsigned int* a2, unsigned int length) {
        for (unsigned int i=0; i<length; i++) {
            if (a1[i] != a2[i]) {
                return false;
            }
        }
        return true;
    }
    virtual bool CompareArraysString (std::string* a1, std::string* a2, unsigned int length) {
        for (unsigned int i=0; i<length; i++) {
            if (a1[i] != a2[i]) {
                return false;
            }
        }
        return true;
    }
};

//StateFromString
TEST_F(ConfigTest, StateFromString) {
  std::string s1 = "DATA_START";
  std::string s2 = "DATA_INDICES";
  std::string s3 = "NUM_ATTRS";
  std::string s4 = "DATA_TYPES";
  std::string s5 = "CUTOFF_VALS";
  std::string s6 = "DATA_PATH";
  std::string s7 = "REPORT_PATH";
  std::string s8 = "THRESHOLD";
  std::string s9 = "NULL_ACTION";
  std::string s10 = "DEFAULT_VALS";
  std::string s11 = "BATCH_SIZE";
  
  EXPECT_EQ(StateFromString(s1), DATA_START);
  EXPECT_EQ(StateFromString(s2), DATA_INDICES);
  EXPECT_EQ(StateFromString(s3), NUM_ATTRS);
  EXPECT_EQ(StateFromString(s4), DATA_TYPES);
  EXPECT_EQ(StateFromString(s5), CUTOFF_VALS);
  EXPECT_EQ(StateFromString(s6), DATA_PATH);
  EXPECT_EQ(StateFromString(s7), REPORT_PATH);
  EXPECT_EQ(StateFromString(s8), THRESHOLD);
  EXPECT_EQ(StateFromString(s9), NULL_ACTION);
  EXPECT_EQ(StateFromString(s10), DEFAULT_VALS);
  EXPECT_EQ(StateFromString(s11), BATCH_SIZE);
}

//ActionFromString test
TEST_F(ConfigTest, ActionFromString) {
    std::string t1 = "ACTION_OMIT";
    std::string t2 = "ACTION_DEFAULT";

    EXPECT_EQ(ActionFromString(t1), ACTION_OMIT);
    EXPECT_EQ(ActionFromString(t2), ACTION_DEFAULT);
}

//Config internal functions
TEST_F(ConfigTest, Config) {
    Config c = Config();
    
    c.SetDataStart("1");
    c.SetNumAttrs("3");
    c.AddDataIndex("0");
    c.AddDataType("DATA_STRING");
    c.AddCutoffVals("j");

    c.AddDataIndex("2");
    c.AddDataType("DATA_INT");
    c.AddCutoffVals("5,3,7");

    c.AddDataIndex("4");
    c.AddDataType("DATA_DOUBLE");
    c.AddCutoffVals("1.555");

    c.SetDataPath("path_to_data.csv");
    c.SetReportPath("path_to_report.txt");

    c.SetThreshold("0.67");
    c.SetBatchSize("5000");

    c.SetNullAction("ACTION_OMIT");
    c.AddDefaultVal("blah");
    
    EXPECT_EQ(c.GetDataStart(), 1);
    EXPECT_EQ(c.GetNumAttrs(), 3);
    EXPECT_EQ(c.GetDataType(0), DATA_STRING);
    EXPECT_EQ(c.GetDataPath(), "path_to_data.csv");
    EXPECT_EQ(c.GetReportPath(), "path_to_report.txt");
    EXPECT_EQ(c.GetThreshold(), 0.67);
    EXPECT_EQ(c.GetNullAction(), ACTION_OMIT);
    EXPECT_EQ(c.GetDefaultVal(0), "blah");
    EXPECT_EQ(c.GetBatchSize(), 5000);
    
    std::vector<unsigned int> c1 = c.GetDataIndices();
    EXPECT_EQ(c1[0], 0);
    EXPECT_EQ(c1[1], 2);
    EXPECT_EQ(c1[2], 4);
    
    std::vector<std::string> c2 = c.GetCutoffs(1);
    EXPECT_EQ(c2[0], "5");
    EXPECT_EQ(c2[1], "3");
    EXPECT_EQ(c2[2], "7");
}

//ReadLine function
TEST_F(ConfigTest, ReadLine) {
    std::string t1 = "DATA_START=1\n";
    std::istringstream* i1 = new std::istringstream(t1);
    std::vector<std::string> o1 = ReadLine(i1);
    EXPECT_EQ(o1[0], "DATA_START");
    EXPECT_EQ(o1[1], "1");

    std::string t2 = "DATA_INDICES=0,2,4\n";
    std::istringstream* i2 = new std::istringstream(t2);
    std::vector<std::string> o2 = ReadLine(i2);
    EXPECT_EQ(o2[0], "DATA_INDICES");
    EXPECT_EQ(o2[1], "0,2,4");

    std::string t3 = "NUM_ATTRS=3\n";
    std::istringstream* i3 = new std::istringstream(t3);
    std::vector<std::string> o3 = ReadLine(i3);
    EXPECT_EQ(o3[0], "NUM_ATTRS");
    EXPECT_EQ(o3[1], "3");

    std::string t4 = "DATA_TYPES=DATA_STRING,DATA_INT,DATA_DOUBLE\n";
    std::istringstream* i4 = new std::istringstream(t4);
    std::vector<std::string> o4 = ReadLine(i4);
    EXPECT_EQ(o4[0], "DATA_TYPES");
    EXPECT_EQ(o4[1], "DATA_STRING,DATA_INT,DATA_DOUBLE");

    std::string t5 = "CUTOFF_VALS=[j],[5,3,7],[1.7]\n";
    std::istringstream* i5 = new std::istringstream(t5);
    std::vector<std::string> o5 = ReadLine(i5);
    EXPECT_EQ(o5[0], "CUTOFF_VALS");
    EXPECT_EQ(o5[1], "[j],[5,3,7],[1.7]");

    std::string t6 = "DATA_PATH=path_to_data.csv\n";
    std::istringstream* i6 = new std::istringstream(t6);
    std::vector<std::string> o6 = ReadLine(i6);
    EXPECT_EQ(o6[0], "DATA_PATH");
    EXPECT_EQ(o6[1], "path_to_data.csv");

    std::string t7 = "REPORT_PATH=path_to_report.txt\n";
    std::istringstream* i7 = new std::istringstream(t7);
    std::vector<std::string> o7 = ReadLine(i7);
    EXPECT_EQ(o7[0], "REPORT_PATH");
    EXPECT_EQ(o7[1], "path_to_report.txt");

    std::string t8 = "THRESHOLD=1.67\n";
    std::istringstream* i8 = new std::istringstream(t8);
    std::vector<std::string> o8 = ReadLine(i8);
    EXPECT_EQ(o8[0], "THRESHOLD");
    EXPECT_EQ(o8[1], "1.67");

    std::string t9 = "NULL_ACTION=ACTION_OMIT\n";
    std::istringstream* i9 = new std::istringstream(t9);
    std::vector<std::string> o9 = ReadLine(i9);
    EXPECT_EQ(o9[0], "NULL_ACTION");
    EXPECT_EQ(o9[1], "ACTION_OMIT");

    std::string t10 = "DEFAULT_VALS=blah,0,1.5\n";
    std::istringstream* i10 = new std::istringstream(t10);
    std::vector<std::string> o10 = ReadLine(i10);
    EXPECT_EQ(o10[0], "DEFAULT_VALS");
    EXPECT_EQ(o10[1], "blah,0,1.5");

    std::string t11 = "BATCH_SIZE=300\n";
    std::istringstream* i11 = new std::istringstream(t11);
    std::vector<std::string> o11 = ReadLine(i11);
    EXPECT_EQ(o11[0], "BATCH_SIZE");
    EXPECT_EQ(o11[1], "300");
}

//SetConfigVal tests
TEST_F(ConfigTest, SetConfigVal) {
    Config* c = new Config();
    
    std::vector<std::string> t1 { "DATA_START", "1" };
    SetConfigVal(t1, c);
    EXPECT_EQ(c->GetDataStart(), 1);

    std::vector<std::string> t2 { "NUM_ATTRS", "3" };
    SetConfigVal(t2, c);
    EXPECT_EQ(c->GetNumAttrs(), 3);

    std::vector<std::string> t3 { "DATA_INDICES", "0,2,4" };
    SetConfigVal(t3, c);
    std::vector<unsigned int> dis = c->GetDataIndices();
    EXPECT_EQ(dis.size(), 3);
    EXPECT_EQ(dis[0], 0);
    EXPECT_EQ(dis[1], 2);
    EXPECT_EQ(dis[2], 4);

    std::vector<std::string> t4 { "DATA_TYPES", "DATA_STRING,DATA_INT,DATA_DOUBLE" };
    SetConfigVal(t4, c);
    EXPECT_EQ(c->GetDataType(0), DATA_STRING);
    EXPECT_EQ(c->GetDataType(1), DATA_INT);
    EXPECT_EQ(c->GetDataType(2), DATA_DOUBLE);

    std::vector<std::string> t5 { "CUTOFF_VALS", "[j],[5,3,7],[1.7]" };
    SetConfigVal(t5, c);
    EXPECT_EQ(c->GetCutoffs(0)[0], "j");
    EXPECT_EQ(c->GetCutoffs(1)[0], "5");
    EXPECT_EQ(c->GetCutoffs(1)[1], "3");
    EXPECT_EQ(c->GetCutoffs(1)[2], "7");
    EXPECT_EQ(c->GetCutoffs(2)[0], "1.7");

    std::vector<std::string> t6 { "DATA_PATH", "path_to_data.csv" };
    SetConfigVal(t6, c);
    EXPECT_EQ(c->GetDataPath(), "path_to_data.csv");

    std::vector<std::string> t7 { "REPORT_PATH", "path_to_report.txt" };
    SetConfigVal(t7, c);
    EXPECT_EQ(c->GetReportPath(), "path_to_report.txt");

    std::vector<std::string> t8 { "THRESHOLD", "1.67" };
    SetConfigVal(t8, c);
    EXPECT_EQ(c->GetThreshold(), 1.67);

    std::vector<std::string> t9 { "NULL_ACTION", "ACTION_OMIT" };
    SetConfigVal(t9, c);
    EXPECT_EQ(c->GetNullAction(), ACTION_OMIT);

    std::vector<std::string> t10 { "DEFAULT_VALS", "blah,0,1.5" };
    SetConfigVal(t10, c);
    EXPECT_EQ(c->GetDefaultVal(0), "blah");
    EXPECT_EQ(c->GetDefaultVal(1), "0");
    EXPECT_EQ(c->GetDefaultVal(2), "1.5");

    std::vector<std::string> t11 { "BATCH_SIZE", "10000" };
    SetConfigVal(t11, c);
    EXPECT_EQ(c->GetBatchSize(), 10000);
}

//ReadLine function
TEST_F(ConfigTest, Parse) {
    Config* c = new Config();
    std::string path = "test.conf";
    bool test = Parse(path.c_str(), c);

    EXPECT_EQ(test, true);
    EXPECT_EQ(c->GetDataStart(), 1);
    EXPECT_EQ(c->GetNumAttrs(), 3);
    
    EXPECT_EQ(c->GetDataType(0), DATA_STRING);
    EXPECT_EQ(c->GetDataType(1), DATA_INT);
    EXPECT_EQ(c->GetDataType(2), DATA_DOUBLE);

    EXPECT_EQ(c->GetDataPath(), "path_to_data.csv");
    EXPECT_EQ(c->GetReportPath(), "path_to_report.txt");

    EXPECT_EQ(c->GetThreshold(), 1.67);
    EXPECT_EQ(c->GetBatchSize(), 4000);

    EXPECT_EQ(c->GetNullAction(), ACTION_DEFAULT);
    EXPECT_EQ(c->GetDefaultVal(0), "blah");
    EXPECT_EQ(c->GetDefaultVal(1), "0");
    EXPECT_EQ(c->GetDefaultVal(2), "1.5");
    
    std::vector<unsigned int> c1 = c->GetDataIndices();
    EXPECT_EQ(c1[0], 0);
    EXPECT_EQ(c1[1], 2);
    EXPECT_EQ(c1[2], 4);

    std::vector<std::string> c2 = c->GetCutoffs(0);
    std::vector<std::string> c3 = c->GetCutoffs(1);
    std::vector<std::string> c4 = c->GetCutoffs(2);
    EXPECT_EQ(c2[0], "j");
    EXPECT_EQ(c3[0], "5");
    EXPECT_EQ(c3[1], "3");
    EXPECT_EQ(c3[2], "7");
    EXPECT_EQ(c4[0], "1.7");    
}