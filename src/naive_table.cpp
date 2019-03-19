//
// Created by tejun on 3/18/2019.
//

#include "naive_table.h"
#include "naive_tuple_group.h"

#include <tuple>
#include <assert.h>
#include <stdexcept>

//////// Constructor ////////

template<int NumAttr>
NaiveTable<NumAttr>::NaiveTable() {

    // Reserve space for all the tuple groups to keep it contiguous
    this->tuple_groups.reserve(NUMBER_TUPLE_GROUPS);

}

//////// DDL Operations ////////

// TODO

//////// DML Operations ////////

template<int NumAttr>
void NaiveTable<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Check if the last tuple group has space for this tuple
    NaiveTupleGroup<NumAttr> &last_tuple_group = this->tuple_groups.back();
    if (!last_tuple_group.isFull()) {

        // Has space, directly push to it
        last_tuple_group.addTuple(data);
        return;

    }

    // Otherwise, need to make a new tuple group
    // Preconditions
    assert(!this->isFull());
    NaiveTupleGroup<NumAttr> new_tuple_group{};

    // Push tuple to this new tuple group
    new_tuple_group.addTuple(data);

    // Add tuple group to our vector
    this->tuple_groups.push_back(new_tuple_group);
}

template<int NumAttr>
void NaiveTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
DbTuple<NumAttr> &NaiveTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index >= this->tuple_groups.size()) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Try to scan the current tuple group
            NaiveTupleGroup<NumAttr> &curr_tuple_group = this->tuple_groups[this->scan_index];
            return curr_tuple_group.getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

//////// Other ////////

template<int NumAttr>
bool NaiveTable<NumAttr>::isFull() const {
    return this->tuple_groups.size() >= NUMBER_TUPLE_GROUPS;
}


// Example usage (don't delete, needed for linking!)
template
class NaiveTable<10>;