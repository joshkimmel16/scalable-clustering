#include "gtest/gtest.h"
#include "dataparser.h"

class DataParserTest : public ::testing::Test {
    protected:
    Config c;
    virtual void SetUp() {}
    std::vector<std::vector<std::string>> trueData = {
        {"col1", "col2",  "col3"  },
        {   "a",    "1",  "1.54"  },
        {   "j",    "3" , "1.555" },
        {   "p",    "10", "1.7"   }};
    
    std::vector<std::vector<std::string>> trueSkipData = {
        {"col1",    "col3"  },
        {   "a",    "1.54"  },
        {   "j",    "1.555" },
        {   "p",    "1.7"   }};
};

// Make sure the data path in the config is mutable
TEST_F(DataParserTest, DataPathTest) {
    Parse("test3.conf", &c);
    DataParser dp(c, 10);
    EXPECT_EQ("test.csv", c.GetDataPath()) << "Config data path was not set";
    auto colIndices = c.GetDataIndices();

    for (int i = 0; i < colIndices.size(); i++)
        EXPECT_EQ(i, colIndices[i]);
}

// Test reading column headers from file
TEST_F(DataParserTest, ColumnHeaderTest) {
    Parse("test3.conf", &c);
    DataParser dp(c, 10);
    dp.LoadHeaders();
    std::vector<std::string> testColHeaders = dp.GetAttrNames();
    std::cout << "From test: " << std::endl;
    for (auto col : testColHeaders) {
        std::cout << col << std::endl;
    }
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
    Parse("test3.conf", &c);
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
    Parse("test3.conf", &c);
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
    Parse("test3.conf", &c);
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

// Test selective columns specified in config DATA_INDICES attribute
TEST_F(DataParserTest, SkipColumnTest) {
    unsigned int readCount, trueOffset = 1, batchSize = 1;
    Parse("test4.conf", &c);
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

// Test selective columns specified in config DATA_INDICES attribute
TEST_F(DataParserTest, NullHandlingTest) {
    unsigned int readCount, trueOffset = 1, batchSize = 1;
    Parse("nulls-test.conf", &c);
    DataParser dp(c, batchSize);

    std::vector<std::vector<std::string>> trueNullFills = {
        {   "col1", "col2",  "col3"  },
        {   "blah",    "1",  "1.54"  },
        {   "j",       "0" , "1.555" },
        {   "p",       "10", "1.5"   }};

    dp.LoadHeaders();
    while (readCount = dp.LoadNextDataBatch())
    {
        std::vector<std::vector<std::string>> testData = dp.GetRawData();
        // Data match?
        for (int row = 0; row < readCount; row++) {
            for (int col = 0; col < testData[0].size(); col++) {
                EXPECT_EQ(trueNullFills[trueOffset+row][col], testData[row][col])
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