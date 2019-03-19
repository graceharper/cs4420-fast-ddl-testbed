//
// Created by tejun on 3/18/2019.
//

#include "naive_tuple_group.h"

#include <assert.h>

//////// Constructor ////////

template<int NumAttr>
NaiveTupleGroup<NumAttr>::NaiveTupleGroup() = default;

//////// DDL Operations ////////

// TODO

//////// DML Operations ////////

template<int NumAttr>
void NaiveTupleGroup<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Preconditions
    assert(!this->isFull());

    // Create tuple
    DbTuple<NumAttr> tuple(data);

    // Add tuple directly to underlying vector (by copy)
    this->tuples[num_tuples_filled++] = tuple;
}

template<int NumAttr>
void NaiveTupleGroup<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
DbTuple<NumAttr> &NaiveTupleGroup<NumAttr>::getNextTuple() {

    // Check if there are no more tuples to scan
    if (this->scan_index >= this->num_tuples_filled) {
        throw std::length_error("No more tuples to scan in tuple group");
    }

    // Return and increment
    return this->tuples[scan_index++];

}

//////// Other helpers ////////

template<int NumAttr>
bool NaiveTupleGroup<NumAttr>::isFull() const {
    return this->num_tuples_filled >= NUMBER_TUPLES_PER_GROUP;
}

// Example usage (don't delete, needed for linking!)
template
class NaiveTupleGroup<10>;