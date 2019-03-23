//
// Created by tejun on 3/18/2019.
//

#include "naive_contiguous_mem_tuple_group.h"

#include <assert.h>

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
NaiveContiguousMemTupleGroup<NumAttr>::NaiveContiguousMemTupleGroup(NaiveContiguousMemTupleGroup<PrevNumAttr> &toCopy)
        : last_tuple_index(toCopy.getLastTupleIndex()), scan_index(0) {

    // Copy actual tuples directly. Note memory is already pre-allocated, just overwrite
    for (int i = 0; i <= toCopy.getLastTupleIndex(); i++) {

        // Extract actual tuple
        DbTuple<PrevNumAttr> &to_copy_tuple = toCopy.getTupleAtIndex(i);

        // Copy directly into array
        this->tuples[i] = to_copy_tuple;

    }

}

//////// DML Operations ////////

template<int NumAttr>
void NaiveContiguousMemTupleGroup<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Preconditions
    assert(!this->isFull());

    // Get tuple
    DbTuple<NumAttr> &tuple = this->tuples[this->last_tuple_index];

    // Store data
    tuple.setData(data);

    // Increment index
    this->last_tuple_index++;
}

template<int NumAttr>
void NaiveContiguousMemTupleGroup<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveContiguousMemTupleGroup<NumAttr>::getNextTuple() {

    // Check if there are no more tuples to scan
    if (this->scan_index >= this->last_tuple_index) {
        throw std::length_error("No more tuples to scan in tuple group");
    }

    // Return and increment
    DbTuple<NumAttr> &tuple = this->tuples[scan_index++];
    return tuple.getData();

}

//////// Other helpers ////////

template<int NumAttr>
bool NaiveContiguousMemTupleGroup<NumAttr>::isFull() const {
    return this->last_tuple_index >= NUMBER_TUPLES_PER_GROUP;
}

//////// Getters ////////

template<int NumAttr>
int NaiveContiguousMemTupleGroup<NumAttr>::getLastTupleIndex() const {
    return last_tuple_index;
}

template<int NumAttr>
int NaiveContiguousMemTupleGroup<NumAttr>::getScanIndex() const {
    return scan_index;
}

template<int NumAttr>
DbTuple<NumAttr> &NaiveContiguousMemTupleGroup<NumAttr>::getTupleAtIndex(int i) {
    return this->tuples[i];
}