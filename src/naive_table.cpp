//
// Created by tejun on 3/18/2019.
//

#include "includes/naive_table.h"
#include "includes/naive_tuple_group.h"

#include <tuple>
#include <assert.h>

//////// Constructor ////////

template<typename TupleGroupType>
NaiveTable<TupleGroupType>::NaiveTable() {

    // Reserve space for all the tuple groups to keep it contiguous
    this->tuple_groups.reserve(NUMBER_TUPLE_GROUPS);

}

//////// DDL Operations ////////

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::addColumnsToEnd(int numColumns) {
    // TODO
}

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::removeColumnsFromEnd(int numColumns) {
    // TODO
}

//////// DML Operations ////////

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::addTuple(std::tuple<int> data) {

    // Preconditions
    assert(tuple_groups.size() < NUMBER_TUPLE_GROUPS);

    // TODO
}

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::startScan() {
    // TODO probs need some extra member variables for this
}

template<typename TupleGroupType>
std::tuple<int> &NaiveTable<TupleGroupType>::getNextTuple() {
    // TODO
    std::tuple<int> t = {};
    return t;   // FIXME dummy code
}

//////// Other ////////

// Example usage (don't delete, needed for linking!)
template
class NaiveTable<NaiveTupleGroup>;