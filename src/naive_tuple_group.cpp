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
    assert(tuples.size() < NUMBER_TUPLES_PER_GROUP);

    // TODO
}

void NaiveTupleGroup::startScan() {
    // TODO probs need some extra member variables for this
}

std::tuple<int> &NaiveTupleGroup::getNextTuple() {
    // TODO
    std::tuple<int> t = {};
    return t;   // FIXME dummy code
}
