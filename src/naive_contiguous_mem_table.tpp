//
// Created by tejun on 3/18/2019.
//

#include "naive_contiguous_mem_table.h"
#include "naive_contiguous_mem_tuple_group.h"

#include <assert.h>
#include <stdexcept>

//////// Constructor ////////

template<int NumAttr>
NaiveContiguousMemTable<NumAttr>::NaiveContiguousMemTable()
        : last_tuple_group_index(0), scan_index(0), tuple_groups(nullptr) {

    // Allocate space for array
    this->tuple_groups = new std::array<NaiveContiguousMemTupleGroup<NumAttr>, NUMBER_TUPLE_GROUPS>();
}

//////// Destructor ////////

template<int NumAttr>
NaiveContiguousMemTable<NumAttr>::~NaiveContiguousMemTable() {
    delete this->tuple_groups;
}

//////// DDL Operations (Copy Constructor) ////////

template<int NumAttr>
template<int PrevNumAttr>
NaiveContiguousMemTable<NumAttr>::NaiveContiguousMemTable(NaiveContiguousMemTable<PrevNumAttr> &toCopy)
        : last_tuple_group_index(toCopy.getLastTupleGroupIndex()), scan_index(0), tuple_groups(nullptr) {

    this->tuple_groups = new std::array<NaiveContiguousMemTupleGroup<NumAttr>, NUMBER_TUPLE_GROUPS>();

    // Copy actual tuples groups directly. Note memory is already pre-allocated, just overwrite
    for (int i = 0; i <= toCopy.getLastTupleGroupIndex(); i++) {

        // Extract actual tuple group
        NaiveContiguousMemTupleGroup<PrevNumAttr> &to_copy_tuple_group = toCopy.getTupleGroupAtIndex(i);

        // Copy tuple group
        NaiveContiguousMemTupleGroup<NumAttr> new_tuple_group(to_copy_tuple_group);

        // Copy new tuple group into array
        (*(this->tuple_groups))[i] = new_tuple_group;
    }

}

//////// DML Operations ////////

template<int NumAttr>
void NaiveContiguousMemTable<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Check if the last tuple group has space for this tuple
    NaiveContiguousMemTupleGroup<NumAttr> &last_tuple_group = (*(this->tuple_groups))[this->last_tuple_group_index];
    if (!last_tuple_group.isFull()) {

        // Has space, directly push to it
        last_tuple_group.addTuple(data);
        return;

    }

    // Otherwise, need to move onto the next tuple group
    // Preconditions
    assert(!this->isFull());

    // Move pointer
    this->last_tuple_group_index++;

    // Get next tuple
    NaiveContiguousMemTupleGroup<NumAttr> &next_tuple_group = (*(this->tuple_groups))[this->last_tuple_group_index];

    // Push tuple to this new tuple group
    next_tuple_group.addTuple(data);
}

template<int NumAttr>
void NaiveContiguousMemTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

    // Reset scan index in all tuple groups
    for (NaiveContiguousMemTupleGroup<NumAttr> &group : (*(this->tuple_groups))) {
        group.startScan();
    }

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveContiguousMemTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index > this->last_tuple_group_index) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Try to scan the current tuple group
            NaiveContiguousMemTupleGroup<NumAttr> &curr_tuple_group = (*(this->tuple_groups))[this->scan_index];
            return curr_tuple_group.getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

//////// Other ////////

template<int NumAttr>
bool NaiveContiguousMemTable<NumAttr>::isFull() const {
    return this->last_tuple_group_index >= NUMBER_TUPLE_GROUPS;
}




//////// Getters ////////

template<int NumAttr>
int NaiveContiguousMemTable<NumAttr>::getLastTupleGroupIndex() const {
    return last_tuple_group_index;
}

template<int NumAttr>
int NaiveContiguousMemTable<NumAttr>::getScanIndex() const {
    return scan_index;
}

template<int NumAttr>
NaiveContiguousMemTupleGroup<NumAttr> &NaiveContiguousMemTable<NumAttr>::getTupleGroupAtIndex(int i) {
    return (*(this->tuple_groups))[i];
}
