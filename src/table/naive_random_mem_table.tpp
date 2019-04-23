//
// Created by tejun on 3/23/2019.
//

#include "includes/table/naive_random_mem_table.h"
#include "includes/tuple_group/naive_contiguous_mem_tuple_group.h"

#include <assert.h>
#include <stdexcept>

//////// Constructor ////////

template<int NumAttr>
NaiveRandomMemTable<NumAttr>::NaiveRandomMemTable()
        : last_tuple_group_index(0), scan_index(0) {

    this->tuple_groups = new std::array<std::unique_ptr<NaiveContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS>();

    // Allocate space for the first tuple group. Init tuple group
    auto new_tuple_group_ptr = std::make_unique<NaiveContiguousMemTupleGroup<NumAttr>>();

    // Store tuple group in table
    (*(this->tuple_groups))[0] = std::move(new_tuple_group_ptr);

}

//////// Destructor ////////

template<int NumAttr>
NaiveRandomMemTable<NumAttr>::~NaiveRandomMemTable() {
    delete this->tuple_groups;
}

//////// DDL Operations (Copy Constructor) ////////

template<int NumAttr>
template<int PrevNumAttr>
NaiveRandomMemTable<NumAttr>::NaiveRandomMemTable(NaiveRandomMemTable<PrevNumAttr> &toCopy)
        : last_tuple_group_index(toCopy.getLastTupleGroupIndex()), scan_index(0) {

    this->tuple_groups = new std::array<std::unique_ptr<NaiveContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS>();

    // Copy actual tuples groups directly. Note memory is NOT pre-allocated
    for (int i = 0; i <= toCopy.getLastTupleGroupIndex(); i++) {

        // Extract actual tuple
        NaiveContiguousMemTupleGroup<PrevNumAttr> *to_copy_tuple_group = toCopy.getTupleGroupAtIndex(i);

        // Copy into pointer
        auto new_tuple_group_ptr = std::make_unique<NaiveContiguousMemTupleGroup<NumAttr>>(*to_copy_tuple_group);

        // Copy directly into array
        (*(this->tuple_groups))[i] = std::move(new_tuple_group_ptr);
    }

}

//////// DML Operations ////////

template<int NumAttr>
void NaiveRandomMemTable<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Check if the last tuple group has space for this tuple
    NaiveContiguousMemTupleGroup<NumAttr> *last_tuple_group = this->getTupleGroupAtIndex(this->last_tuple_group_index);
    if (!last_tuple_group->isFull()) {

        // Has space, directly push to it
        last_tuple_group->addTuple(data);
        return;

    }

    // Otherwise, need to move onto the next tuple group
    // Preconditions
    assert(!this->isFull());

    // Move pointer
    this->last_tuple_group_index++;

    // Init tuple group
    auto new_tuple_group_ptr = std::make_unique<NaiveContiguousMemTupleGroup<NumAttr>>();

    // Push tuple to this new tuple group
    new_tuple_group_ptr->addTuple(data);

    // Store tuple group in table
    (*(this->tuple_groups))[this->last_tuple_group_index] = std::move(new_tuple_group_ptr);

}

template<int NumAttr>
void NaiveRandomMemTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

    // Reset scan index in all tuple groups
    for (int i = 0; i <= this->last_tuple_group_index; i++) {
        NaiveContiguousMemTupleGroup<NumAttr> *tuple_group = this->getTupleGroupAtIndex(i);
        tuple_group->startScan();
    }

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveRandomMemTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index > this->last_tuple_group_index) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Try to scan the current tuple group
            NaiveContiguousMemTupleGroup<NumAttr> *curr_tuple_group = this->getTupleGroupAtIndex(this->scan_index);
            return curr_tuple_group->getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

//////// Other ////////

template<int NumAttr>
bool NaiveRandomMemTable<NumAttr>::isFull() const {
    return this->last_tuple_group_index >= NUMBER_TUPLE_GROUPS;
}

//////// Getters ////////

template<int NumAttr>
int NaiveRandomMemTable<NumAttr>::getLastTupleGroupIndex() const {
    return last_tuple_group_index;
}

template<int NumAttr>
int NaiveRandomMemTable<NumAttr>::getScanIndex() const {
    return scan_index;
}

template<int NumAttr>
NaiveContiguousMemTupleGroup<NumAttr> *NaiveRandomMemTable<NumAttr>::getTupleGroupAtIndex(int i) {
    return (*(this->tuple_groups))[i].get();
}
