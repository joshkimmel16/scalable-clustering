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
  
  EXPECT_EQ(StateFromString(s1), DATA_START);
  EXPECT_EQ(StateFromString(s2), DATA_INDICES);
  EXPECT_EQ(StateFromString(s3), NUM_ATTRS);
  EXPECT_EQ(StateFromString(s4), DATA_TYPES);
  EXPECT_EQ(StateFromString(s5), CUTOFF_VALS);
  EXPECT_EQ(StateFromString(s6), DATA_PATH);
  EXPECT_EQ(StateFromString(s7), REPORT_PATH);

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
    
    EXPECT_EQ(c.GetDataStart(), 1);
    EXPECT_EQ(c.GetNumAttrs(), 3);
    EXPECT_EQ(c.GetDataType(0), DATA_STRING);
    EXPECT_EQ(c.GetDataPath(), "path_to_data.csv");
    EXPECT_EQ(c.GetReportPath(), "path_to_report.txt");
    
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
TEST_F(ConfigTest, Parse) {
    Config* c = new Config();
    std::string path = "test.conf";
    bool test = Parse(path.c_str(), c);

    EXPECT_EQ(test, true);
    EXPECT_EQ(c->GetDataStart(), 1);
    EXPECT_EQ(c->GetNumAttrs(), 3);
    EXPECT_EQ(c->GetDataType(0), DATA_STRING);
    //EXPECT_EQ(c->GetDataPath(), "path_to_data.csv");
    //EXPECT_EQ(c->GetReportPath(), "path_to_report.txt");
    
    /*
    std::vector<unsigned int> c1 = c->GetDataIndices();
    EXPECT_EQ(c1[0], 0);
    EXPECT_EQ(c1[1], 2);
    EXPECT_EQ(c1[2], 4);

    std::vector<std::string> c2 = c->GetCutoffs(1);
    EXPECT_EQ(c2[0], "5");
    EXPECT_EQ(c2[1], "3");
    EXPECT_EQ(c2[2], "7");
    */    
}