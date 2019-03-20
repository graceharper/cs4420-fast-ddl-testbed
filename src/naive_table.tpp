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
    NaiveTupleGroup<NumAttr> &last_tuple_group = this->tuple_groups[num_tuple_groups_filled];
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

    // Add tuple group to our vector and increment
    this->tuple_groups[num_tuple_groups_filled++] = new_tuple_group;
}

template<int NumAttr>
void NaiveTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

}

template<int NumAttr>
std::array<int, NumAttr> &NaiveTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index >= this->num_tuple_groups_filled) {
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