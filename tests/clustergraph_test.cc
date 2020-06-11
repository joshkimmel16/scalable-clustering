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
    root->IncrementCount();
    root->IncrementCount();
    root->SetFlag(0, 2);
    root->SetFlag(2, 3);
    std::vector<unsigned int> ranges{0, 1, 0, 2, 0, 3, 0, 5};

    EXPECT_EQ(root->GetDimension(), 4);
    EXPECT_EQ(root->GetChildren().size(), 4);
    EXPECT_EQ(root->GetCount(), 2);
    EXPECT_EQ(root->GetFlag(0), 2);
    EXPECT_EQ(root->GetFlag(1), 0);
    EXPECT_EQ(root->GetFlag(2), 3);
    EXPECT_EQ(root->GetFlag(3), 0);
    root->ResetFlags();
    EXPECT_EQ(root->GetFlag(0), 0);
    EXPECT_EQ(root->GetFlag(1), 0);
    EXPECT_EQ(root->GetFlag(2), 0);
    EXPECT_EQ(root->GetFlag(3), 0);
    root->SetFlags(5);
    EXPECT_EQ(root->GetFlag(0), 5);
    EXPECT_EQ(root->GetFlag(1), 5);
    EXPECT_EQ(root->GetFlag(2), 5);
    EXPECT_EQ(root->GetFlag(3), 5);

    for (int i = 0; i < root->GetDimension(); ++i) {
        EXPECT_EQ(root->GetParent(i), nullptr);
        EXPECT_EQ(root->GetChildren(i).GetLeft(), nullptr);
        EXPECT_EQ(root->GetChildren(i).GetRight(), nullptr);
    }

    EXPECT_EQ(root->GetRanges().size(), ranges.size());
    for (int i = 0; i < root->GetRanges().size(); ++i) {
        EXPECT_EQ(root->GetRanges()[i], ranges[i]);
    }

    DataPoint *anchor = new DataPoint(4);
    std::string raw_data[4] = {"g", "6", "1.81", "5"};
    anchor->PopulateRawData(raw_data);
    EXPECT_EQ(root->GetAnchor()->GetStringValue(0), anchor->GetStringValue(0));
    EXPECT_EQ(root->GetAnchor()->GetIntValue(1), anchor->GetIntValue(1));
    EXPECT_EQ(root->GetAnchor()->GetDoubleValue(2), anchor->GetDoubleValue(2));
    EXPECT_EQ(root->GetAnchor()->GetIntValue(3), anchor->GetIntValue(3));
    delete anchor;
}

//ClusterGraph testing
TEST_F(ClusterGraphTest, ClusterGraph) {

    EXPECT_EQ(cluster_graph->GetDimension(), 4);
    cluster_graph->PopulateChildren();

    Cluster *root = cluster_graph->GetRoot();

    // check whether different paths can reach the same cluster
    Cluster *ptr1 = root->GetChild(1, LEFT)->GetChild(0, LEFT);
    Cluster *ptr2 = root->GetChild(0, LEFT)->GetChild(1, LEFT);
    ptr1->SetFlag(0, 3);
    ptr1->SetFlag(2, 4);
    EXPECT_EQ(ptr1, ptr2);
    EXPECT_EQ(ptr2->GetFlag(0), 3);
    EXPECT_EQ(ptr2->GetFlag(2), 4);

    Cluster *ptr3 = root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(1, RIGHT);
    Cluster *ptr4 = root->GetChild(1, LEFT)->GetChild(0, LEFT)->GetChild(1, RIGHT);
    Cluster *ptr5 = root->GetChild(1, LEFT)->GetChild(1, RIGHT)->GetChild(0, LEFT);
    ptr3->SetFlag(0, 3);
    ptr3->SetFlag(2, 4);
    EXPECT_EQ(ptr3, ptr4);
    EXPECT_EQ(ptr4, ptr5);
    EXPECT_EQ(ptr3, ptr5);
    EXPECT_EQ(ptr4->GetFlag(0), 3);
    EXPECT_EQ(ptr4->GetFlag(1), 0);
    EXPECT_EQ(ptr5->GetFlag(2), 4);
    EXPECT_EQ(ptr5->GetFlag(3), 0);
    ptr3->ResetFlags();
    EXPECT_EQ(ptr4->GetFlag(0), 0);
    EXPECT_EQ(ptr4->GetFlag(1), 0);
    EXPECT_EQ(ptr5->GetFlag(2), 0);
    EXPECT_EQ(ptr5->GetFlag(3), 0);
    ptr4->SetFlags(5);
    EXPECT_EQ(ptr3->GetFlag(0), 5);
    EXPECT_EQ(ptr3->GetFlag(1), 5);
    EXPECT_EQ(ptr5->GetFlag(2), 5);
    EXPECT_EQ(ptr5->GetFlag(3), 5);

    // check the leave node is correct
    Cluster *ptr6 = root->GetChild(0, LEFT);
    EXPECT_EQ(ptr6->GetChild(0, LEFT), nullptr);

    // check anchor value
    EXPECT_EQ(ptr1->GetAnchor()->GetStringValue(0), "");
    EXPECT_EQ(ptr1->GetAnchor()->GetIntValue(1), 3);
    EXPECT_EQ(ptr3->GetAnchor()->GetStringValue(0), "");
    EXPECT_EQ(ptr3->GetAnchor()->GetStringValue(1), "");
    EXPECT_EQ(ptr6->GetAnchor()->GetStringValue(0), "");

    // test
    Cluster *ptr7 = root->GetChild(0, LEFT)->GetChild(1, LEFT);
    Cluster *ptr8 = root->GetChild(1, LEFT)->GetChild(0, LEFT);
    EXPECT_EQ(ptr7, ptr8);
    EXPECT_EQ(ptr7->GetCount(), 0);

    // check Processing DataPoint
    DataPoint *dp1 = new DataPoint(4);
    std::string raw1[4] = {"b", "4", "1.6", "5"};
    dp1->PopulateRawData(raw1);
    DataPoint *dp2 = new DataPoint(4);
    std::string raw2[4] = {"c", "2", "3.2", "6"};
    dp2->PopulateRawData(raw2);
    DataPoint *dp3 = new DataPoint(4);
    std::string raw3[4] = {"h", "7", "1.49", "-1"};
    dp3->PopulateRawData(raw3);
    cluster_graph->ProcessDataPoint(1, dp1);
    cluster_graph->ProcessDataPoint(2, dp2);
    cluster_graph->ProcessDataPoint(3, dp3);
    EXPECT_EQ(root->GetCount(), 3);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetCount(), 2);
    EXPECT_EQ(root->GetChild(0, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(1, LEFT)->GetCount(), 2);
    EXPECT_EQ(root->GetChild(1, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(2, LEFT)->GetCount(), 2);
    EXPECT_EQ(root->GetChild(2, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(3, LEFT)->GetCount(), 2);
    EXPECT_EQ(root->GetChild(3, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT), root->GetChild(1, LEFT)->GetChild(0, LEFT));
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetCount(), 2);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, RIGHT)->GetCount(), 0);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, LEFT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, LEFT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, LEFT)->GetChild(3, LEFT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, LEFT)->GetChild(3, RIGHT)->GetCount(), 0);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, RIGHT)->GetChild(3, LEFT)->GetCount(), 0);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, RIGHT)->GetChild(3, RIGHT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, LEFT)->GetChild(2, LEFT)->GetCount(), 1);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, LEFT)->GetChild(2, RIGHT)->GetCount(), 0);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, RIGHT)->GetChild(2, LEFT)->GetCount(), 0);
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(3, RIGHT)->GetChild(2, RIGHT)->GetCount(), 1);

    // test GetName and GetFullName
    EXPECT_EQ(root->GetFullName(), "(-INF,INF),(-INF,INF),(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(0, LEFT)->GetFullName(), "(-INF,g],(-INF,INF),(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(0, RIGHT)->GetFullName(), "(g,INF),(-INF,INF),(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(1, LEFT)->GetFullName(), "(-INF,INF),(-INF,6],(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(1, RIGHT)->GetFullName(), "(-INF,INF),(6,INF),(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(1, LEFT)->GetChild(1, LEFT)->GetFullName(), "(-INF,INF),(-INF,3],(-INF,INF),(-INF,INF)");
    EXPECT_EQ(root->GetChild(1, LEFT)->GetChild(1, RIGHT)->GetFullName(), "(-INF,INF),(3,6],(-INF,INF),(-INF,INF)");

    // test pruning
    Cluster *ptr = root->GetChild(1, LEFT)->GetChild(2, LEFT);
    delete ptr;
    EXPECT_EQ(root->GetChild(0, LEFT)->GetChild(1, LEFT)->GetChild(2, LEFT), nullptr);

    // delete the whole graph
    delete cluster_graph;
}