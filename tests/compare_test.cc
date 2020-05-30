#include "gtest/gtest.h"
#include "compare.h"

class CompareTest : public ::testing::Test {
  protected:
    virtual void SetUp() {}
};

//FromString test
TEST_F(CompareTest, FromString) {
  std::string s1 = "DATA_STRING";
  std::string s2 = "DATA_INT";
  std::string s3 = "DATA_DOUBLE";
  std::string s4 = "DATA_UNKNOWN";
  
  EXPECT_EQ(FromString(s1), DATA_STRING);
  EXPECT_EQ(FromString(s2), DATA_INT);
  EXPECT_EQ(FromString(s3), DATA_DOUBLE);
  EXPECT_EQ(FromString(s4), DATA_STRING);
}

//Compare test
TEST_F(CompareTest, Compare) {
    std::string anch_data[3] = { "j", "3", "1.555" };
    unsigned int anch_len = 3;
    DataPoint* anch = new DataPoint(anch_len);
    anch->PopulateRawData(anch_data);

    std::string raw_1[3] = { "a", "1", "1.54" };
    unsigned int raw_1_len = 3;
    std::string raw_2[3] = { "j", "3", "1.555" };
    unsigned int raw_2_len = 3;
    std::string raw_3[3] = { "p", "10", "1.7" };
    unsigned int raw_3_len = 3;
    DataPoint* d1 = new DataPoint(raw_1_len);
    d1->PopulateRawData(raw_1);
    DataPoint* d2 = new DataPoint(raw_2_len);
    d2->PopulateRawData(raw_2);
    DataPoint* d3 = new DataPoint(raw_3_len);
    d3->PopulateRawData(raw_3);

    EXPECT_EQ(Compare(anch, d1, 0, DATA_STRING), 1);
    EXPECT_EQ(Compare(anch, d1, 1, DATA_INT), 1);
    EXPECT_EQ(Compare(anch, d1, 2, DATA_DOUBLE), 1);
    EXPECT_EQ(Compare(anch, d2, 0, DATA_STRING), 1);
    EXPECT_EQ(Compare(anch, d2, 1, DATA_INT), 1);
    EXPECT_EQ(Compare(anch, d2, 2, DATA_DOUBLE), 1);
    EXPECT_EQ(Compare(anch, d3, 0, DATA_STRING), 2);
    EXPECT_EQ(Compare(anch, d3, 1, DATA_INT), 2);
    EXPECT_EQ(Compare(anch, d3, 2, DATA_DOUBLE), 2);

    delete d1;
    delete d2;
    delete d3;

}