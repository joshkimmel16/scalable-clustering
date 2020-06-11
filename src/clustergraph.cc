#include <utility>

#include "clustergraph.h"

Cluster::Cluster(unsigned int d, std::vector<unsigned int> r, ClusterGraph *g, Config *conf,
                 DataPoint *anch) :
        dimension(d), ranges(std::move(r)), graph(g), config(conf), anchor(anch) {
    count = 0;
    for (int i = 0; i < dimension; ++i) {
        flags.push_back(0);
        parents.push_back(nullptr);
        Branch branch;
        children.push_back(branch);
    }
}

Cluster::~Cluster() {
    delete anchor;
    // recursively delete all children
    for (int d = 0; d < dimension; d++) {
        if (children[d].GetLeft() != nullptr) {
            Cluster *left = children[d].GetLeft();
            delete left;
        }
        if (children[d].GetRight() != nullptr) {
            Cluster *right = children[d].GetRight();
            delete right;
        }
    }
    // nullify the pointers to itself in parent
    for (int d = 0; d < dimension; d++) {
        Cluster *parent = parents[d];
        if (parent == nullptr) {
            continue;
        }
        const unsigned int start_index = d * 2;
        const unsigned int end_index = start_index + 1;
        if (ranges[start_index] == parent->GetRanges()[start_index] &&
            ranges[end_index] < parent->GetRanges()[end_index]) {
            parent->SetChild(d, LEFT, nullptr);
        } else if (ranges[start_index] > parent->GetRanges()[start_index] &&
                   ranges[end_index] == parent->GetRanges()[end_index]) {
            parent->SetChild(d, RIGHT, nullptr);
        }
    }
}


void Cluster::PopulateChildren() {
    for (int d = 0; d < dimension; d++) {
        PopulateChildren(d);
    }
}

void Cluster::PopulateChildren(const unsigned int &d) {
    const unsigned int start_index = d * 2; // if there are 2 dimensions, the ranges look like: [0, 2, 0, 5]
    const unsigned int end_index = start_index + 1;
    const unsigned int start = ranges[start_index];
    const unsigned int end = ranges[end_index];
    // if the length of cutoff[d] is 1, it is a leaf cluster in this dimension, this cluster has no children in this dimension
    if (start >= end) {
        return;
    }
    // add the left child
    std::vector<unsigned int> left_ranges = ranges;
    left_ranges[end_index] = (start + end) / 2;
    // check if the left child has been added to the graph
    std::vector<Cluster *> search_result = graph->Search(left_ranges);
    if (search_result.size() == 2 && search_result[0] != this) { // it has been added before
        search_result[1]->SetParent(d, this);
        SetChild(d, LEFT, search_result[1]);
    } else { // it has not been added before
        // build the anchor for the left child
        auto *left_anchor = new DataPoint(*anchor);
        if (left_ranges[start_index] < left_ranges[end_index]) {
            left_anchor->SetStringValue(d,
                                        config->GetCutoffs(d)[(left_ranges[start_index] + left_ranges[end_index]) / 2]);
        } else {
            left_anchor->SetStringValue(d, std::string()); // the left child is an leaf node and has no anchor value
        }
        auto *left = new Cluster(dimension, left_ranges, graph, config, left_anchor);
        left->SetParent(d, this);
        SetChild(d, LEFT, left);
        left->PopulateChildren(); // DFS
    }
    // add the right child
    std::vector<unsigned int> right_ranges = ranges;
    right_ranges[start_index] = (start + end) / 2 + 1;
    // check if the right child has been added to the graph
    search_result = graph->Search(right_ranges);
    if (search_result.size() == 2 && search_result[0] != this) { // it has been added before
        search_result[1]->SetParent(d, this);
        SetChild(d, RIGHT, search_result[1]);
    } else { // it has not been added before
        // build the anchor for the right child
        auto *right_anchor = new DataPoint(*anchor);
        if (right_ranges[start_index] < right_ranges[end_index]) {
            right_anchor->SetStringValue(d,
                                         config->GetCutoffs(d)[(right_ranges[start_index] + right_ranges[end_index]) /
                                                               2]);
        } else {
            right_anchor->SetStringValue(d, std::string()); // the right child is an leaf node and has no anchor value
        }
        auto *right = new Cluster(dimension, right_ranges, graph, config, right_anchor);
        right->SetParent(d, this);
        SetChild(d, RIGHT, right);
        right->PopulateChildren();
    }
}

void Cluster::ProcessDataPoint(const int &id, DataPoint *p) {
    // flags[0] is set to id once a datapoint has been visited.
    if (flags[0] == id) {
        return;
    }
    // count++
    IncrementCount();
    SetFlag(0, id);
    for (int d = 0; d < dimension; d++) {
        // leaf node in dimension d
        if (anchor->GetStringValue(d).empty()) {
            continue;
        }
        // NOT leaf node in dimension d
        unsigned int result = Compare(anchor, p, d, config->GetDataType(d));
        if (result == 1) { // left
            GetChild(d, LEFT)->ProcessDataPoint(id, p);
        } else if (result == 2) { // right
            GetChild(d, RIGHT)->ProcessDataPoint(id, p);
        }
    }
}

void Cluster::SetParent(const unsigned int &d, Cluster *p) {
    parents[d] = p;
}

void Cluster::SetChild(const unsigned int &d, BranchDirection direction, Cluster *p) {
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

void Cluster::IncrementCount() {
    count++;
}

void Cluster::SetCount(unsigned int c) {
    count = c;
}

void Cluster::SetFlag(const unsigned int &d, const int &flag) {
    flags[d] = flag;
}

void Cluster::SetFlags(const int &flag) {
    std::fill(flags.begin(), flags.end(), flag);
}

void Cluster::ResetFlags() {
    std::fill(flags.begin(), flags.end(), 0);
}


unsigned int Cluster::GetDimension() const {
    return dimension;
}

std::vector<Cluster *> Cluster::GetParents() const {
    return parents;
}

Cluster *Cluster::GetParent(const unsigned int &d) const {
    return parents[d];
}


std::vector<Branch> Cluster::GetChildren() const {
    return children;
}

Branch Cluster::GetChildren(const unsigned int &d) const {
    return children[d];
}

Cluster *Cluster::GetChild(const unsigned int &d, BranchDirection direction) const {
    return (direction == LEFT ? children[d].GetLeft() : children[d].GetRight());
}

DataPoint *Cluster::GetAnchor() const {
    return anchor;
}

const std::vector<unsigned int> &Cluster::GetRanges() const {
    return ranges;
}

unsigned int Cluster::GetCount() const {
    return count;
}

const int &Cluster::GetFlag(const unsigned int &d) const {
    return flags[d];
}

std::string Cluster::GetName(const unsigned int &d) const {
    if (d < 0 || d >= dimension) {
        return std::string();
    }
    const unsigned int start_index = d * 2; // if there are 2 dimensions, the ranges look like: [0, 2, 0, 5]
    const unsigned int end_index = start_index + 1;
    const unsigned int start = ranges[start_index];
    const unsigned int end = ranges[end_index];
    std::vector<std::string> cutoff = config->GetCutoffs(d);
    std::string left = start == 0 ? "-INF" : cutoff[start - 1];
    std::string right = end == cutoff.size() ? "INF" : cutoff[end];
    return "(" + left + "," + right + (end == cutoff.size() ? ")" : "]");
}

std::string Cluster::GetFullName() const {
    std::string full_name = GetName(0);
    for (int d = 1; d < dimension; d++) {
        full_name.append("," + GetName(d));
    }
    return full_name;
}

void Branch::SetLeft(Cluster *p) {
    left = p;
}

void Branch::SetRight(Cluster *p) {
    right = p;
}

Cluster *Branch::GetLeft() const {
    return left;
}

Cluster *Branch::GetRight() const {
    return right;
}


ClusterGraph::ClusterGraph(Config *conf) : dimension(conf->GetNumAttrs()) {
    std::vector<unsigned int> ranges(dimension * 2, 0);
    auto *anchor = new DataPoint(dimension);
    for (unsigned int d = 0; d < dimension; d++) {
        ranges[d * 2 + 1] = conf->GetCutoffs(d).size();
        anchor->SetStringValue(d, conf->GetCutoffs(d)[conf->GetCutoffs(d).size() / 2]);
    }
    root = new Cluster(dimension, ranges, this, conf, anchor);
}

ClusterGraph::~ClusterGraph() {
    delete root;
}

std::vector<Cluster *> ClusterGraph::Search(std::vector<unsigned int> ranges) {
    std::vector<Cluster *> result; // if found, return [parent, target]; if not found, return []
    Cluster *parent = nullptr;
    Cluster *target = root;
    for (int d = 0; d < dimension; d++) { // find matching in order: dimension 0 -> 1 -> 2 ...
        unsigned int target_left;
        unsigned int target_right;
        while (true) {
            if (target == nullptr) {// search fails
                return result;
            }
            target_left = target->GetRanges()[d * 2];
            target_right = target->GetRanges()[d * 2 + 1];
            if (ranges[d * 2] == target_left && ranges[d * 2 + 1] == target_right) {
                // matching is found in dimension, i++
                break;
            } else if (target_right > target_left && ranges[d * 2 + 1] <= ((target_left + target_right) / 2)) {
                // go the left child
                parent = target;
                target = target->GetChildren(d).GetLeft();
            } else if (target_right > target_left && ranges[d * 2] >= ((target_left + target_right) / 2 + 1)) {
                // go the right child
                parent = target;
                target = target->GetChildren(d).GetRight();
            } else { // search fails
                return result;
            }
        }
    }
    // return [parent, target]
    result.push_back(parent);
    result.push_back(target);
    return result;
}

void ClusterGraph::PopulateChildren() {
    root->PopulateChildren();
}

void ClusterGraph::ProcessDataPoint(const int &id, DataPoint *p) {
    root->ProcessDataPoint(id, p);
}

Cluster *ClusterGraph::GetRoot() {
    return root;
}

unsigned int ClusterGraph::GetDimension() const {
    return dimension;
}


