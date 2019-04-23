//
// Created by tejun on 3/18/2019.
//

#include "includes/tuple_group/versioned_contiguous_mem_tuple_group.h"

#include <assert.h>

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
VersionedContiguousMemTupleGroup<NumAttr>::VersionedContiguousMemTupleGroup(
        VersionedContiguousMemTupleGroup<PrevNumAttr> &toCopy): last_tuple_index(toCopy.getLastTupleIndex()),
                                                                scan_index(0),
                                                                version(Version::PRE_DDL) {

    // Assertions
    assert(toCopy.getVersion() == Version::IN_DDL);

    // Copy actual tuples directly. Note memory is already pre-allocated, just overwrite
    for (int i = 0; i < toCopy.getLastTupleIndex(); i++) {

        // Extract actual tuple
        DbTuple<PrevNumAttr> &to_copy_tuple = toCopy.getTupleAtIndex(i);

        // Create new tuple
        DbTuple<NumAttr> new_tuple(to_copy_tuple);

        // Copy new tuple into array
        this->tuples[i] = new_tuple;

    }

    // Mark copied group as outdated
    toCopy.setVersion(Version::POST_DDL);

}

//////// DML Operations ////////

template<int NumAttr>
void VersionedContiguousMemTupleGroup<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Preconditions
    assert(!this->isFull());

    // Get tuple
    DbTuple<NumAttr> &tuple = this->tuples[this->last_tuple_index];

    // Store data    ~DDLTest() = default;
    tuple.setData(data);

    // Increment index
    this->last_tuple_index++;
}

template<int NumAttr>
void VersionedContiguousMemTupleGroup<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
std::array<int, NumAttr> &VersionedContiguousMemTupleGroup<NumAttr>::getNextTuple() {

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
bool VersionedContiguousMemTupleGroup<NumAttr>::isFull() const {
    return this->last_tuple_index >= NUMBER_TUPLES_PER_GROUP;
}

//////// Getters ////////

template<int NumAttr>
int VersionedContiguousMemTupleGroup<NumAttr>::getLastTupleIndex() const {
    return last_tuple_index;
}

template<int NumAttr>
int VersionedContiguousMemTupleGroup<NumAttr>::getScanIndex() const {
    return scan_index;
}

template<int NumAttr>
DbTuple<NumAttr> &VersionedContiguousMemTupleGroup<NumAttr>::getTupleAtIndex(int i) {
    return this->tuples[i];
}

template<int NumAttr>
Version VersionedContiguousMemTupleGroup<NumAttr>::getVersion() const {
    return version;
}

template<int NumAttr>
void VersionedContiguousMemTupleGroup<NumAttr>::setVersion(Version v) {
    this->version = v;
}
