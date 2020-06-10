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

TEST_F(ReporterTest, Test1) {
    std::ofstream myfile;
    Parse("test5.conf", &c);
    ClusterGraph * cluster_graph = new ClusterGraph(&c);
    cluster_graph->PopulateChildren();

    unsigned int readCount, trueOffset = 1, batchSize = 10;
    DataParser dp(c, batchSize);

    dp.LoadHeaders();
    
    while (readCount = dp.LoadNextDataBatch(cluster_graph))
    {
    }    
    printClusterGraph(cluster_graph->GetRoot());
    
}




