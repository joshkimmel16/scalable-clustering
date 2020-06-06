#include "gtest/gtest.h"
#include "clustergraph.h"

class ClusterGraphTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        conf = new Config();
        std::string path = "test2.conf";
        Parse(path.c_str(), conf);
        cluster_graph = new ClusterGraph(conf);
    }

    ClusterGraph *cluster_graph;
    Config *conf;
};



//Cluster testing
TEST_F(ClusterGraphTest, Cluster) {

    Cluster *root = cluster_graph->GetRoot();

    EXPECT_EQ(root->GetDimension(), 4);
    EXPECT_EQ(root->GetChildren().size(), 4);

    for (int i = 0; i < root->GetDimension(); ++i) {
        EXPECT_EQ(root->GetParent(i), nullptr);
        EXPECT_EQ(root->GetChildren(i).GetLeft(), nullptr) << "left child[" << i << "] is not correct";
        EXPECT_EQ(root->GetChildren(i).GetRight(), nullptr) << "right child[" << i << "] is not correct";
    }

    std::vector<unsigned int> ranges{0, 1, 0, 2, 0, 3, 0, 5};
    EXPECT_EQ(root->GetRanges().size(), ranges.size()) << "The length of ranges is not correct";
    for (int i = 0; i < root->GetRanges().size(); ++i) {
        EXPECT_EQ(root->GetRanges()[i], ranges[i]) << "ranges[" << i << "] is not correct";
    }

    DataPoint anchor(4);
    std::string raw_data[4] = {"a", "6", "1.81", "5"};
    anchor.PopulateRawData(raw_data);
    EXPECT_EQ(root->GetAnchor()->GetStringValue(0), "a");
    EXPECT_EQ(root->GetAnchor()->GetStringValue(1), "6");
    EXPECT_EQ(root->GetAnchor()->GetStringValue(2), "1.81");
    EXPECT_EQ(root->GetAnchor()->GetStringValue(3), "5");
}

//ClusterGraph testing
TEST_F(ClusterGraphTest, ClusterGraph) {

    EXPECT_EQ(cluster_graph->GetDimension(), 4);
    cluster_graph->PopulateChildren();

    Cluster *root = cluster_graph->GetRoot();

    // check whether different paths can reach the same cluster
    Cluster *ptr1 = root->GetChildren(1).GetLeft()->GetChildren(0).GetLeft();
    Cluster *ptr2 = root->GetChildren(0).GetLeft()->GetChildren(1).GetLeft();
    EXPECT_EQ(ptr1, ptr2);

    Cluster *ptr3 = root->GetChildren(0).GetLeft()->GetChildren(1).GetLeft()->GetChildren(1).GetRight();
    Cluster *ptr4 = root->GetChildren(1).GetLeft()->GetChildren(0).GetLeft()->GetChildren(1).GetRight();
    Cluster *ptr5 = root->GetChildren(1).GetLeft()->GetChildren(1).GetRight()->GetChildren(0).GetLeft();
    EXPECT_EQ(ptr3, ptr4);
    EXPECT_EQ(ptr4, ptr5);
    EXPECT_EQ(ptr3, ptr5);

    // check the leave node is correct
    Cluster *ptr6 = root->GetChildren(0).GetLeft();
    EXPECT_EQ(ptr6->GetChildren(0).GetLeft(), nullptr);

    // check anchor value
    EXPECT_EQ(ptr1->GetAnchor()->GetStringValue(0), "");
    EXPECT_EQ(ptr1->GetAnchor()->GetIntValue(1), 3);
    EXPECT_EQ(ptr3->GetAnchor()->GetStringValue(0), "");
    EXPECT_EQ(ptr3->GetAnchor()->GetStringValue(1), "");
    EXPECT_EQ(ptr6->GetAnchor()->GetStringValue(0), "");
}