#include "gtest/gtest.h"
#include "datapoint.h"

class DataPointTest : public ::testing::Test {
  protected:
    virtual void SetUp() {}    
};

//DataPoint testing
TEST_F(DataPointTest, Test1) {
  
  std::string raw_data[3] = { "hello", "3", "1.555" };
  unsigned int raw_len = 3;
  DataPoint d = DataPoint(raw_len);
  d.PopulateRawData(raw_data);

  EXPECT_EQ(d.GetStringValue(0), "hello");
  EXPECT_EQ(d.GetIntValue(1), 3);
  EXPECT_EQ(d.GetDoubleValue(2), 1.555);
}