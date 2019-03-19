//
// Created by tejun on 3/18/2019.
//

#include "naive_table.h"

#include <includes/naive_tuple_group.h>
#include <tuple>

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::addColumnsToEnd(int numColumns) {
    // TODO
}

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::removeColumnsFromEnd(int numColumns) {
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

// Example usage (don't delete, needed for linking!)
template
class NaiveTable<NaiveTupleGroup>;