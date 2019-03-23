//
// Created by tejun on 3/18/2019.
//

#include "naive_contiguous_mem_tuple_group.h"

#include <assert.h>

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
NaiveContiguousMemTupleGroup<NumAttr>::NaiveContiguousMemTupleGroup(NaiveContiguousMemTupleGroup<PrevNumAttr> &toCopy)
        : num_tuples_filled(0), scan_index(0) {
    // TODO @sai, copy all tuples into this instance
}

//////// DML Operations ////////

template<int NumAttr>
void NaiveContiguousMemTupleGroup<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Preconditions
    assert(!this->isFull());

    // Get tuple
    DbTuple<NumAttr> &tuple = this->tuples[this->num_tuples_filled];

    // Store data
    tuple.setData(data);

    // Increment index
    this->num_tuples_filled++;
}

template<int NumAttr>
void NaiveContiguousMemTupleGroup<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveContiguousMemTupleGroup<NumAttr>::getNextTuple() {

    // Check if there are no more tuples to scan
    if (this->scan_index >= this->num_tuples_filled) {
        throw std::length_error("No more tuples to scan in tuple group");
    }

    // Return and increment
    DbTuple<NumAttr> &tuple = this->tuples[scan_index++];
    return tuple.getData();

}

//////// Other helpers ////////

template<int NumAttr>
bool NaiveContiguousMemTupleGroup<NumAttr>::isFull() const {
    return this->num_tuples_filled >= NUMBER_TUPLES_PER_GROUP;
}