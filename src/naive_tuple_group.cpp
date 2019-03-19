//
// Created by tejun on 3/18/2019.
//

#include "includes/naive_tuple_group.h"

#include <tuple>
#include <assert.h>

//////// Constructor ////////

NaiveTupleGroup::NaiveTupleGroup() {

    // Reserve space for all the tuples in group to keep it contiguous
    this->tuples.reserve(NUMBER_TUPLES_PER_GROUP);

}

//////// DDL Operations ////////

void NaiveTupleGroup::addColumnsToEnd(int numColumns) {
    // TODO
}

void NaiveTupleGroup::removeColumnsFromEnd(int numColumns) {
    // TODO
}

//////// DML Operations ////////

void NaiveTupleGroup::addTuple(std::tuple<int> data) {

    // Preconditions
    assert(!this->isFull());

    // Add tuple directly to underlying vector (by copy)
    this->tuples.push_back(data);
}

void NaiveTupleGroup::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

std::tuple<int> &NaiveTupleGroup::getNextTuple() {

    // Check if there are no more tuples to scan
    if (this->scan_index >= this->tuples.size()) {
        throw std::length_error("No more tuples to scan in tuple group");
    }

    // Return and increment
    return this->tuples[scan_index++];

}

//////// Other helpers ////////

bool NaiveTupleGroup::isFull() const {
    return this->tuples.size() >= NUMBER_TUPLES_PER_GROUP;
}