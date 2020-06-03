#include "gtest/gtest.h"
#include "dataparser.h"

class DataParserTest : public ::testing::Test {
    protected:
    Config c;
    virtual void SetUp() {
        c.SetDataPath("test.csv");
    }
    std::vector<std::vector<std::string>> trueData = {
        {"col1", "col2",  "col3"  },
        {   "a",    "1",  "1.54"  },
        {   "j",    "3" , "1.555" },
        {   "p",    "10", "1.7"   }};
};

// Make sure the data path in the config is mutable
TEST_F(DataParserTest, DataPathTest) {
    EXPECT_EQ("test.csv", c.GetDataPath()) << "Config data path was not set";
}

// Test reading column headers from file
TEST_F(DataParserTest, ColumnHeaderTest) {
    DataParser dp(c, 10);
    dp.LoadHeaders();
    std::vector<std::string> testColHeaders = dp.GetAttrNames();

    // Are there the same number of columns?
    EXPECT_EQ(trueData[0].size(), testColHeaders.size()) << "Number of columns not equal.";
    
    // Do the column headers match?
    for (int i = 0; i < trueData[0].size(); i++) {
        EXPECT_EQ(trueData[0][i], testColHeaders[i])
        << "Columns headers "
        << trueData[0][i]
        << " and "
        << testColHeaders[i]
        << " are not equal";
    }
}

// Test reading batch data from file
// Batch size less than dataset size
TEST_F(DataParserTest, Batch1DataTest) {

    unsigned int readCount, trueOffset = 1, batchSize = 1;

    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    while (readCount = dp.LoadNextDataBatch())
    {
        std::vector<std::vector<std::string>> testData = dp.GetRawData();
        // Data match?
        for (int row = 0; row < readCount; row++) {
            for (int col = 0; col < testData[0].size(); col++) {
                EXPECT_EQ(trueData[trueOffset+row][col], testData[row][col])
                << "Data at ["
                << row
                << "]["
                << col
                << "] is not equal.";
            }
        }
        trueOffset += readCount;
    }
}

// Batch size equal to dataset size
TEST_F(DataParserTest, Batch3DataTest) {

    unsigned int readCount, trueOffset = 1, batchSize = 3;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    while (readCount = dp.LoadNextDataBatch())
    {
        std::vector<std::vector<std::string>> testData = dp.GetRawData();
        // Data match?
        for (int row = 0; row < readCount; row++) {
            for (int col = 0; col < testData[0].size(); col++) {
                EXPECT_EQ(trueData[trueOffset+row][col], testData[row][col])
                << "Data at ["
                << row
                << "]["
                << col
                << "] is not equal.";
            }
        }
        trueOffset += readCount;
    }
}

// Batch size greater than dataset size
TEST_F(DataParserTest, Batch10DataTest) {

    unsigned int readCount, trueOffset = 1, batchSize = 10;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    while (readCount = dp.LoadNextDataBatch())
    {
        std::vector<std::vector<std::string>> testData = dp.GetRawData();
        // Data match?
        for (int row = 0; row < readCount; row++) {
            for (int col = 0; col < testData[0].size(); col++) {
                EXPECT_EQ(trueData[trueOffset+row][col], testData[row][col])
                << "Data at ["
                << row
                << "]["
                << col
                << "] is not equal.";
            }
        }
        trueOffset += readCount;
    }
}

