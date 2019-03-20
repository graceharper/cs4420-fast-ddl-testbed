//
// Created by tejun on 3/18/2019.
//

#include "naive_table.h"
#include "naive_tuple_group.h"

#include <assert.h>
#include <stdexcept>

//////// DDL Operations ////////

template<int NumAttr>
template<int PrevNumAttr>
NaiveTable<NumAttr>::NaiveTable(NaiveTable<PrevNumAttr> &toCopy) : num_tuple_groups_filled(0), scan_index(0) {
    // TODO @sai, copy all tuple groups into this instance
    // Make sure to use the copy constructor of the NaiveTupleGroups
}

//////// DML Operations ////////

template<int NumAttr>
void NaiveTable<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Check if the last tuple group has space for this tuple
    NaiveTupleGroup<NumAttr> &last_tuple_group = this->tuple_groups[this->num_tuple_groups_filled];
    if (!last_tuple_group.isFull()) {

        // Has space, directly push to it
        last_tuple_group.addTuple(data);
        return;

    }

    // Otherwise, need to move onto the next tuple group
    // Preconditions
    assert(!this->isFull());

    // Move pointer
    this->num_tuple_groups_filled++;

    // Get next tuple
    NaiveTupleGroup<NumAttr> &next_tuple_group = this->tuple_groups[this->num_tuple_groups_filled];

    // Push tuple to this new tuple group
    next_tuple_group.addTuple(data);
}

template<int NumAttr>
void NaiveTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

    // Reset scan index in all tuple groups
    for (NaiveTupleGroup<NumAttr> group : this->tuple_groups) {
        group.startScan();
    }

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index > this->num_tuple_groups_filled) {
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
    return this->num_tuple_groups_filled >= NUMBER_TUPLE_GROUPS;
}