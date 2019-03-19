//
// Created by tejun on 3/18/2019.
//

#include "includes/naive_table.h"
#include "includes/naive_tuple_group.h"

#include <tuple>
#include <assert.h>
#include <stdexcept>

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

    // Check if the last tuple group has space for this tuple
    TupleGroupType &last_tuple_group = this->tuple_groups.back();
    if (!last_tuple_group.isFull()) {

        // Has space, directly push to it
        last_tuple_group.addTuple(data);
        return;

    }

    // Otherwise, need to make a new tuple group
    // Preconditions
    assert(!this->isFull());
    TupleGroupType new_tuple_group = TupleGroupType();

    // Push tuple to this new tuple group
    new_tuple_group.addTuple(data);

    // Add tuple group to our vector
    this->tuple_groups.push_back(new_tuple_group);
}

template<typename TupleGroupType>
void NaiveTable<TupleGroupType>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<typename TupleGroupType>
std::tuple<int> &NaiveTable<TupleGroupType>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index >= this->tuple_groups.size()) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Try to scan the current tuple group
            TupleGroupType &curr_tuple_group = this->tuple_groups[this->scan_index];
            return curr_tuple_group.getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

//////// Other ////////

template<typename TupleGroupType>
bool NaiveTable<TupleGroupType>::isFull() const {
    return this->tuple_groups.size() >= NUMBER_TUPLE_GROUPS;
}


// Example usage (don't delete, needed for linking!)
template
class NaiveTable<NaiveTupleGroup>;