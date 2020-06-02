#include "gtest/gtest.h"
#include "clustergraph.h"

class ClusterGraphTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        conf = new Config();
        std::string path = "test2.conf";
        Parse(path.c_str(), conf);

//        std::cout << "DIMENSION " << conf->GetNumAttrs();
        cluster_graph = new ClusterGraph(conf);
        root = cluster_graph->GetRoot();
    }

    ClusterGraph *cluster_graph;
    Cluster *root;
    Config *conf;
};

//Cluster testing
TEST_F(ClusterGraphTest, Cluster) {
    Cluster *c = root;

    EXPECT_EQ(c->GetDimension(), 4);
//    EXPECT_EQ(c->GetParent(2), nullptr);
    EXPECT_EQ(c->GetChildren().size(), 4);
//
    for (int i = 0; i < c->GetDimension(); ++i) {
//        std::cout << "ZZX";
        EXPECT_EQ(c->GetParent(i), nullptr);
        // << "parent[" << i << "] is not correct"
        EXPECT_EQ(c->GetBranch(i).GetLeft(), nullptr) << "left child[" << i << "] is not correct";
        EXPECT_EQ(c->GetBranch(i).GetRight(), nullptr) << "right child[" << i << "] is not correct";
    }

    std::vector<unsigned int> ranges{0, 1, 0, 2, 0, 3, 0, 5};
    EXPECT_EQ(c->GetRanges().size(), ranges.size()) << "The length of ranges is not correct";
    for (int i = 0; i < c->GetRanges().size(); ++i) {
        EXPECT_EQ(c->GetRanges()[i], ranges[i]) << "ranges[" << i << "] is not correct";
    }

    DataPoint anchor(4);
    std::string raw_data[4] = {"a", "6", "1.81", "5"};
    anchor.PopulateRawData(raw_data);
    EXPECT_EQ(c->GetAnchor()->GetStringValue(0), "a");
    EXPECT_EQ(c->GetAnchor()->GetStringValue(1), "6");
    EXPECT_EQ(c->GetAnchor()->GetStringValue(2), "1.81");
    EXPECT_EQ(c->GetAnchor()->GetStringValue(3), "5");
}

//ClusterGraph testing
TEST_F(ClusterGraphTest, ClusterGraph) {
    cluster_graph->PopulateChildren();
//    Cluster *c = root;
//
//    std::vector<unsigned int> ranges{0, 1, 0, 2, 0, 3, 0, 5};
//    EXPECT_EQ(c->GetRanges().size(), ranges.size()) << "The length of ranges is not correct";
//    for (int i = 0; i < c->GetRanges().size(); ++i) {
//        EXPECT_EQ(c->GetRanges()[i], ranges[i]) << "ranges[" << i << "] is not correct";
//    }
//
//    DataPoint anchor(4);
//    std::string raw_data[4] = {"a", "6", "1.81", "5"};
//    anchor.PopulateRawData(raw_data);
//    EXPECT_EQ(c->GetAnchor()->GetStringValue(0), "a");
//    EXPECT_EQ(c->GetAnchor()->GetStringValue(1), "6");
//    EXPECT_EQ(c->GetAnchor()->GetStringValue(2), "1.81");
//    EXPECT_EQ(c->GetAnchor()->GetStringValue(3), "5");
}