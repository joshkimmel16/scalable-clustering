#include "gtest/gtest.h"
#include "dataparser.h"
#include "reporter.h"
#include <queue>

class ReporterTest : public ::testing::Test {
protected:
    Config c;
    virtual void SetUp() {}
        std::vector<std::vector<std::string>> testData = {
        {"col1", "col2"},
        {   "-1",    "-1"},
        {   "-1",    "1"}};

    void printClusterGraph(Cluster * cluster) {
        int depth = 0;
        int maxDepth = 0;
        std::queue<Cluster*> cluster_q;
        std::queue<int> depth_q;
        if (cluster == nullptr)
            return;
        std::cout << "-------Level " <<depth << "-------" << std::endl;
        std::cout << "Root Count: " << cluster->GetCount() << std::endl;

        cluster_q.push(cluster);
        depth_q.push(depth+1);
        while (!cluster_q.empty()) {
            cluster = cluster_q.front();
            depth = depth_q.front();
            cluster_q.pop();
            depth_q.pop();
            for (int i = 0; i < cluster->GetDimension(); i++) {
                Cluster * leftChild = cluster->GetChild(i, LEFT);
                Cluster * rightChild = cluster->GetChild(i, RIGHT);
                
                if (leftChild != nullptr) {
                    
                    if (depth > maxDepth) {
                        std::cout << "-------Level " <<depth << "-------" << std::endl;
                        maxDepth = depth;
                    }
                    std::cout << "Left Child Dim " << i << " Count: " << leftChild->GetCount()<< std::endl;
                    for (int j=0; j < leftChild->GetRanges().size(); j++)
                        std::cout << leftChild->GetRanges()[j] << " ";
                    std::cout << std::endl;
                    cluster_q.push(cluster->GetChild(i, LEFT));
                    depth_q.push(depth+1);
                }
                if (rightChild != nullptr) {
                    if (depth > maxDepth) {
                        std::cout << "-------Level " <<depth << "-------" << std::endl;
                        maxDepth = depth;
                    }
                    
                    std::cout << "Right Child Dim " << i << " Count: "<< rightChild->GetCount() << std::endl;
                    for (int j=0; j < rightChild->GetRanges().size(); j++)
                        std::cout << rightChild->GetRanges()[j] << " ";
                    std::cout << std::endl;
                    cluster_q.push(cluster->GetChild(i, RIGHT));
                    depth_q.push(depth+1);
                }
            }
        }
    }

};

TEST_F(ReporterTest, SimpleGraphTest) {
    std::vector<std::vector<unsigned int>> expectedResults {
				{ 0,0,0,0 },
                { 0,0,1,1 }
			};
    Parse("test5.conf", &c);
    ClusterGraph * cluster_graph = new ClusterGraph(&c);
    cluster_graph->PopulateChildren();

    unsigned int readCount, trueOffset = 1, batchSize = 10, threshold = 1;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }
    std::cout << "Original Cluster Graph" << std::endl; 
    printClusterGraph(cluster_graph->GetRoot());

    Reporter * reporter = new Reporter(cluster_graph, threshold);
    reporter->CompressClusterGraph();


    std::cout << std::endl << std::endl << "Compressed Cluster Graph" << std::endl;
    printClusterGraph(cluster_graph->GetRoot());

    std::cout << "Report: " <<std::endl;
    std::vector<Cluster *> * report = reporter->GenerateReport();
    for (int i=0; i < report->size(); i++) {
        std::cout << "Cluster: " << std::endl;
        std::cout << "Count: " << (*report)[i]->GetCount() << std::endl;
        std::cout << "Range: ";
        for (int j=0; j < (*report)[i]->GetRanges().size(); j++)
                        std::cout << (*report)[i]->GetRanges()[j] << " ";
        std::cout << std::endl;
    }

    for (int i=0; i < report->size(); i++) {
        bool foundMatch = false;
        for(int j=0; j < expectedResults.size(); j++) {
            EXPECT_EQ((*report)[i]->GetRanges().size(), expectedResults[j].size());
            bool flag = true;
            for(int k = 0; k < expectedResults[j].size(); k++) {
                if ((*report)[i]->GetRanges()[k] != expectedResults[j][k]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                expectedResults.erase(expectedResults.begin() + j);
                foundMatch = true;
                break;
            }
            EXPECT_EQ(true, foundMatch);
        }
    }

    delete reporter;
    delete cluster_graph;
}


TEST_F(ReporterTest, OneDimensionTest) {
    std::vector<std::vector<unsigned int>> expectedResults {
				{ 0,7 },
                { 8,8 },
				{ 9,9 },
                { 8,15 }
			};
    Parse("test6.conf", &c);
    ClusterGraph * cluster_graph = new ClusterGraph(&c);
    cluster_graph->PopulateChildren();

    unsigned int readCount, trueOffset = 1, batchSize = 10, threshold = 10;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }
    std::cout << "Original Cluster Graph" << std::endl; 
    printClusterGraph(cluster_graph->GetRoot());

    Reporter * reporter = new Reporter(cluster_graph, threshold);
    reporter->CompressClusterGraph();


    std::cout << std::endl << std::endl << "Compressed Cluster Graph" << std::endl;
    printClusterGraph(cluster_graph->GetRoot());

    std::cout << "Report: " <<std::endl;
    std::vector<Cluster *> * report = reporter->GenerateReport();
    for (int i=0; i < report->size(); i++) {
        std::cout << "Cluster: " << std::endl;
        std::cout << "Count: " << (*report)[i]->GetCount() << std::endl;
        std::cout << "Range: ";
        for (int j=0; j < (*report)[i]->GetRanges().size(); j++)
                        std::cout << (*report)[i]->GetRanges()[j] << " ";
        std::cout << std::endl;
    }

    for (int i=0; i < report->size(); i++) {
        bool foundMatch = false;
        for(int j=0; j < expectedResults.size(); j++) {
            EXPECT_EQ((*report)[i]->GetRanges().size(), expectedResults[j].size());
            bool flag = true;
            for(int k = 0; k < expectedResults[j].size(); k++) {
                if ((*report)[i]->GetRanges()[k] != expectedResults[j][k]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                expectedResults.erase(expectedResults.begin() + j);
                foundMatch = true;
                break;
            }
            EXPECT_EQ(true, foundMatch);
        }
    }

    delete reporter;
    delete cluster_graph;
}

TEST_F(ReporterTest, NoLeavesPrunedTest) {
    std::vector<std::vector<unsigned int>> expectedResults {
				{ 0,0,0,0 },
                { 0,0,1,1 },
				{ 1,1,0,0 },
                { 1,1,1,1 },
				{ 2,2,0,0 },
                { 2,2,1,1 },
                { 3,3,0,0 },
                { 3,3,1,1 }
			};
    Parse("test7.conf", &c);
    ClusterGraph * cluster_graph = new ClusterGraph(&c);
    cluster_graph->PopulateChildren();

    unsigned int readCount, trueOffset = 1, batchSize = 10, threshold = 3;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }
    std::cout << "Original Cluster Graph" << std::endl; 
    printClusterGraph(cluster_graph->GetRoot());

    Reporter * reporter = new Reporter(cluster_graph, threshold);
    reporter->CompressClusterGraph();


    std::cout << std::endl << std::endl << "Compressed Cluster Graph" << std::endl;
    printClusterGraph(cluster_graph->GetRoot());

    std::cout << "Report: " <<std::endl;
    std::vector<Cluster *> * report = reporter->GenerateReport();
    for (int i=0; i < report->size(); i++) {
        std::cout << "Cluster: " << std::endl;
        std::cout << "Count: " << (*report)[i]->GetCount() << std::endl;
        std::cout << "Range: ";
        for (int j=0; j < (*report)[i]->GetRanges().size(); j++)
                        std::cout << (*report)[i]->GetRanges()[j] << " ";
        std::cout << std::endl;
    }

    EXPECT_EQ(report->size(), expectedResults.size());
    
    for (int i=0; i < report->size(); i++) {
        bool foundMatch = false;
        for(int j=0; j < expectedResults.size(); j++) {
            EXPECT_EQ((*report)[i]->GetRanges().size(), expectedResults[j].size());
            bool flag = true;
            for(int k = 0; k < expectedResults[j].size(); k++) {
                if ((*report)[i]->GetRanges()[k] != expectedResults[j][k]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                expectedResults.erase(expectedResults.begin() + j);
                foundMatch = true;
                break;
            }
            EXPECT_EQ(true, foundMatch);
        }
    }

    delete reporter;
    delete cluster_graph;
}

TEST_F(ReporterTest, LevelSkippedTest) {
    std::vector<std::vector<unsigned int>> expectedResults {
				{ 0,0,1,1 },
				{ 1,1,0,0 },
				{ 0,1,0,1 },
				{ 2,2,0,0 },
                { 3,3,0,0 },
                { 2,3,1,1 }
			};
    Parse("test8.conf", &c);
    ClusterGraph * cluster_graph = new ClusterGraph(&c);
    cluster_graph->PopulateChildren();

    unsigned int readCount, trueOffset = 1, batchSize = 10, threshold = 6;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }
    std::cout << "Original Cluster Graph" << std::endl; 
    printClusterGraph(cluster_graph->GetRoot());

    Reporter * reporter = new Reporter(cluster_graph, threshold);
    reporter->CompressClusterGraph();


    std::cout << std::endl << std::endl << "Compressed Cluster Graph" << std::endl;
    printClusterGraph(cluster_graph->GetRoot());

    std::cout << "Report: " <<std::endl;
    std::vector<Cluster *> * report = reporter->GenerateReport();
    for (int i=0; i < report->size(); i++) {
        std::cout << "Cluster: " << std::endl;
        std::cout << "Count: " << (*report)[i]->GetCount() << std::endl;
        std::cout << "Range: ";
        for (int j=0; j < (*report)[i]->GetRanges().size(); j++)
                        std::cout << (*report)[i]->GetRanges()[j] << " ";
        std::cout << std::endl;
    }

    EXPECT_EQ(report->size(), expectedResults.size());
    
    for (int i=0; i < report->size(); i++) {
        bool foundMatch = false;
        for(int j=0; j < expectedResults.size(); j++) {
            EXPECT_EQ((*report)[i]->GetRanges().size(), expectedResults[j].size());
            bool flag = true;
            for(int k = 0; k < expectedResults[j].size(); k++) {
                if ((*report)[i]->GetRanges()[k] != expectedResults[j][k]) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                expectedResults.erase(expectedResults.begin() + j);
                foundMatch = true;
                break;
            }
            EXPECT_EQ(true, foundMatch);
        }
    }
    delete reporter;
    delete cluster_graph;
}




