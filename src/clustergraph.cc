#include "clustergraph.h"

// #include <utility>

Cluster::Cluster(unsigned int d, std::vector<unsigned int> r, ClusterGraph *g, Config *conf,
                 DataPoint *anch) :
        dimension(d), ranges(r), graph(g), config(conf) {
    count = 0;
    anchor = anch;
    for (int i = 0; i < dimension; ++i) {

        parents.push_back(nullptr);
//        std::cout << "d: " << GetParents().size();
        Branch branch;
        children.push_back(branch);
    }
}

Cluster::~Cluster() {
    delete anchor;
}

void Cluster::SetParent(unsigned int d, Cluster *p) {
    parents[d] = p;
}

void Cluster::SetChild(unsigned int d, BranchDirection direction, Cluster *p) {
    switch (direction) {
        case LEFT:
            children[d].SetLeft(p);
            break;
        case RIGHT:
            children[d].SetRight(p);
            break;
        default:
            break;
    }
}

void Cluster::SetAnchor(DataPoint *anchor) {
    Cluster::anchor = anchor;
}

const unsigned int Cluster::GetDimension() const {
    return dimension;
}

Cluster *Cluster::GetParent(unsigned int d) const {
    return parents[d];
}

std::vector<Cluster *> Cluster::GetParents() const {
    return parents;
}

Branch Cluster::GetBranch(unsigned int d) const {
    return children[d];
}

std::vector<Branch> Cluster::GetChildren() const {
    return children;
}

DataPoint *Cluster::GetAnchor() const {
    return anchor;
}

const std::vector<unsigned int> &Cluster::GetRanges() const {
    return ranges;
}

void Cluster::PopulateChildren() {
    for (int i = 0; i < dimension; i++) {
        PopulateChildren(i);
    }
}

void Cluster::PopulateChildren(unsigned int d) {
    const unsigned int start_index = d * 2;
    const unsigned int end_index = start_index + 1;
    const unsigned int start = ranges[start_index];
    const unsigned int end = ranges[end_index];
    // if the length of cutoff[d] is 1, it is a leaf cluster in this dimension, no need to populate children
    if (start >= end) {
        return;
    }
    // left
    std::vector<unsigned int> left_ranges = ranges;
    left_ranges[end_index] = (start + end) / 2;
    // check if the left child has been added to the graph
    std::vector<Cluster *> search_result = graph->Search(left_ranges);
    // it has been added before
    if (search_result.size() == 2 && search_result[0] != this) {
        search_result[1]->SetParent(d, this);
        SetChild(d, LEFT, search_result[1]);
    } else { // it has not been added before
        DataPoint *left_anchor = new DataPoint(*anchor);
        left_anchor->SetStringValue(d, config->GetCutoffs(d)[(left_ranges[start_index] + left_ranges[end_index]) / 2]);
        Cluster *left = new Cluster(dimension, left_ranges, graph, config, left_anchor);
        left->SetParent(d, this);
        SetChild(d, LEFT, left);
        left->PopulateChildren();
    }
    //right
    std::vector<unsigned int> right_ranges = ranges;
    right_ranges[start_index] = (start + end) / 2 + 1;
    // check if the right child has been added to the graph
    search_result = graph->Search(right_ranges);
    // it has been added before
    if (search_result.size() == 2 && search_result[0] != this) {
        search_result[1]->SetParent(d, this);
        SetChild(d, RIGHT, search_result[1]);
    } else { // it has not been added before
        DataPoint *right_anchor = new DataPoint(*anchor);
        if (right_ranges[start_index] < right_ranges[end_index]) {
            right_anchor->SetStringValue(d,
                                         config->GetCutoffs(d)[(right_ranges[start_index] + right_ranges[end_index]) /
                                                               2]);
        } else {
            right_anchor->SetStringValue(d, NULL);
        }
        Cluster *right = new Cluster(dimension, right_ranges, graph, config, right_anchor);
        right->SetParent(d, this);
        SetChild(d, RIGHT, right);
        right->PopulateChildren();
    }
}


void Branch::SetLeft(Cluster *p) {
    left = p;
}

void Branch::SetRight(Cluster *p) {
    right = p;
}

Cluster *Branch::GetLeft() {
    return left;
}

Cluster *Branch::GetRight() {
    return right;
}


ClusterGraph::ClusterGraph(Config *conf) : dimension(conf->GetNumAttrs()) {
    std::vector<unsigned int> ranges(dimension * 2, 0);
    DataPoint *anchor = new DataPoint(dimension);
    for (unsigned int i = 0; i < dimension; i++) {
        ranges[i * 2 + 1] = conf->GetCutoffs(i).size();
        anchor->SetStringValue(i, conf->GetCutoffs(i)[conf->GetCutoffs(i).size() / 2]);
    }
    root = new Cluster(dimension, ranges, this, conf, anchor);
//    std::cout << dimension << "ZZX";
}

ClusterGraph::~ClusterGraph() {
    //TODO: recurse through graph deleting all clusters (use stack)
    //be careful about only adding non-empty pointers onto the stack (since a cluster can have multiple parents)
}

std::vector<Cluster *> ClusterGraph::Search(std::vector<unsigned int> ranges) {
    Cluster *parent = nullptr;
    Cluster *target = root;
    for (int i = 0; i < dimension; i++) {
        while (true) {
            if (target == nullptr) {
                // search fails
                return std::vector<Cluster *>();
            }
            unsigned int target_left = target->GetRanges()[i * 2];
            unsigned int target_right = target->GetRanges()[i * 2 + 1];
            // go the left child
            if (target_right > target_left && ranges[i * 2 + 1] <= ((target_left + target_right) / 2)) {
                parent = target;
                target = target->GetBranch(i).GetLeft();
            } else if (target_right > target_left && ranges[i * 2] >= ((target_left + target_right) / 2 + 1)) {
                // go the right child
                parent = target;
                target = target->GetBranch(i).GetRight();
            } else if (ranges[i * 2] == target_left && ranges[i * 2 + 1] == target_right) {
                //match this, i++
                break;
            } else {
                // search fails
                return std::vector<Cluster *>();
            }
        }
        std::vector<Cluster *> result(2);
        result[0] = parent;
        result[1] = target;
        return result;
    }


    return std::vector<Cluster *>();
}

void ClusterGraph::PopulateChildren() {
    root->PopulateChildren();
}

Cluster *ClusterGraph::GetRoot() {
    return root;
}




