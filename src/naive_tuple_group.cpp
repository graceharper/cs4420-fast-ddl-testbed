//
// Created by tejun on 3/18/2019.
//

#include "includes/naive_tuple_group.h"

#include <tuple>
#include <assert.h>

//////// Constructor ////////

template<int PreNumAttr, int PostNumAttr>
NaiveTupleGroup<PreNumAttr, PostNumAttr>::NaiveTupleGroup() = default;

//////// DDL Operations ////////

template<int PreNumAttr, int PostNumAttr>
void NaiveTupleGroup<PreNumAttr, PostNumAttr>::runDdlOperation() {
    // TODO
}

//////// DML Operations ////////

template<int PreNumAttr, int PostNumAttr>
void NaiveTupleGroup<PreNumAttr, PostNumAttr>::addTuple(std::array<int, PreNumAttr> data) {

    // Preconditions
    assert(!this->isFull());

    // Create tuple
    DbTuple<PreNumAttr> tuple(data);

    // Add tuple directly to underlying vector (by copy)
    this->tuples[num_tuples_filled++] = tuple;
}

template<int PreNumAttr, int PostNumAttr>
void NaiveTupleGroup<PreNumAttr, PostNumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int PreNumAttr, int PostNumAttr>
DbTuple<PreNumAttr> &NaiveTupleGroup<PreNumAttr, PostNumAttr>::getNextTuple() {

    // Check if there are no more tuples to scan
    if (this->scan_index >= this->num_tuples_filled) {
        throw std::length_error("No more tuples to scan in tuple group");
    }

    // Return and increment
    return this->tuples[scan_index++];

}

//////// Other helpers ////////

template<int PreNumAttr, int PostNumAttr>
bool NaiveTupleGroup<PreNumAttr, PostNumAttr>::isFull() const {
    return this->num_tuples_filled >= NUMBER_TUPLES_PER_GROUP;
}

// Example usage (don't delete, needed for linking!)
template
class NaiveTupleGroup<10, 20>;