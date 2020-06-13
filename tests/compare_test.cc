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
  std::string s4 = "DATA_DATE";
  std::string s5 = "DATA_UNKNOWN";
  
  EXPECT_EQ(FromString(s1), DATA_STRING);
  EXPECT_EQ(FromString(s2), DATA_INT);
  EXPECT_EQ(FromString(s3), DATA_DOUBLE);
  EXPECT_EQ(FromString(s4), DATA_DATE);
  EXPECT_EQ(FromString(s5), DATA_STRING);
}

//Date functions
TEST_F(CompareTest, Date) {
  std::string t1 = "7/14/1992";
  std::string t2 = "1/1/2020";
  std::string t3 = "12/31/1980";

  std::string t4 = "1992-06-14";
  std::string t5 = "1992-08-01";

  Date d1 = Date(t1, MONTH_DAY_YEAR);
  EXPECT_EQ(d1.month, 7);
  EXPECT_EQ(d1.year, 1992);
  EXPECT_EQ(d1.day, 14);

  Date d2 = Date(t2, MONTH_DAY_YEAR);
  Date d3 = Date(t3, MONTH_DAY_YEAR);
  EXPECT_EQ(d1.compare(d2), true);
  EXPECT_EQ(d1.compare(d3), false);
  EXPECT_EQ(d1.compare(d1), true);

  Date d4 = Date(t4, YEAR_MONTH_DAY);
  Date d5 = Date(t5, YEAR_MONTH_DAY);
  EXPECT_EQ(d1.compare(d4), false);
  EXPECT_EQ(d1.compare(d5), true);
}

//Compare test
TEST_F(CompareTest, Compare) {
    std::string anch_data[4] = { "j", "3", "1.555", "1992-07-14" };
    unsigned int anch_len = 4;
    DataPoint* anch = new DataPoint(anch_len);
    anch->PopulateRawData(anch_data);

    std::string raw_1[4] = { "a", "1", "1.54", "1991-06-20" };
    unsigned int raw_1_len = 4;
    std::string raw_2[4] = { "j", "3", "1.555", "1992-07-14" };
    unsigned int raw_2_len = 4;
    std::string raw_3[4] = { "p", "10", "1.7", "2000-04-20" };
    unsigned int raw_3_len = 4;
    DataPoint* d1 = new DataPoint(raw_1_len);
    d1->PopulateRawData(raw_1);
    DataPoint* d2 = new DataPoint(raw_2_len);
    d2->PopulateRawData(raw_2);
    DataPoint* d3 = new DataPoint(raw_3_len);
    d3->PopulateRawData(raw_3);

    EXPECT_EQ(Compare(anch, d1, 0, DATA_STRING), 1);
    EXPECT_EQ(Compare(anch, d1, 1, DATA_INT), 1);
    EXPECT_EQ(Compare(anch, d1, 2, DATA_DOUBLE), 1);
    EXPECT_EQ(Compare(anch, d1, 3, DATA_DATE), 1);

    EXPECT_EQ(Compare(anch, d2, 0, DATA_STRING), 1);
    EXPECT_EQ(Compare(anch, d2, 1, DATA_INT), 1);
    EXPECT_EQ(Compare(anch, d2, 2, DATA_DOUBLE), 1);
    EXPECT_EQ(Compare(anch, d2, 3, DATA_DATE), 1);

    EXPECT_EQ(Compare(anch, d3, 0, DATA_STRING), 2);
    EXPECT_EQ(Compare(anch, d3, 1, DATA_INT), 2);
    EXPECT_EQ(Compare(anch, d3, 2, DATA_DOUBLE), 2);
    EXPECT_EQ(Compare(anch, d3, 3, DATA_DATE), 2);

    delete d1;
    delete d2;
    delete d3;

}