//
// Created by tejun on 4/20/2019.
//

#include "amortized_aurora_table.h"
#include "versioned_contiguous_mem_tuple_group.h"

#include <assert.h>
#include <stdexcept>

//////// Constructor ////////

template<int NumAttr>
AmortizedAuroraTable<NumAttr>::AmortizedAuroraTable()
        : last_tuple_group_index(0), scan_index(0), to_copy_index(NUMBER_TUPLE_GROUPS + 1) {

    // Allocate space for array
    this->tuple_groups = new std::array<std::unique_ptr<VersionedContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS>();

    // Allocate space for the first tuple group. Init tuple group
    auto new_tuple_group_ptr = std::make_unique<VersionedContiguousMemTupleGroup<NumAttr>>();

    // Store tuple group in table
    (*(this->tuple_groups))[0] = std::move(new_tuple_group_ptr);

}

//////// Destructor ////////

template<int NumAttr>
AmortizedAuroraTable<NumAttr>::~AmortizedAuroraTable() {
    delete this->tuple_groups;
}

//////// DDL Operations (Copy Constructor) ////////

template<int NumAttr>
template<int PrevNumAttr>
AmortizedAuroraTable<NumAttr>::AmortizedAuroraTable(AmortizedAuroraTable<PrevNumAttr> &toCopy)
        : last_tuple_group_index(toCopy.getLastTupleGroupIndex()), scan_index(0), to_copy_index(0) {
    this->tuple_groups = new std::array<std::unique_ptr<VersionedContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS>();

    // Mark all tuple groups in old table as a new version
    for (int i = 0; i <= toCopy.getLastTupleGroupIndex(); i++) {
        VersionedContiguousMemTupleGroup<PrevNumAttr> *tuple_group = toCopy.getTupleGroupAtIndex(i);
        tuple_group->setVersion(Version::IN_DDL);
    }
}

//////// DML Operations ////////

template<int NumAttr>
void AmortizedAuroraTable<NumAttr>::addTuple(std::array<int, NumAttr> data) {

    // Check if the last tuple group has space for this tuple
    VersionedContiguousMemTupleGroup<NumAttr> *last_tuple_group = this->getTupleGroupAtIndex(
            this->last_tuple_group_index);
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
    auto new_tuple_group_ptr = std::make_unique<VersionedContiguousMemTupleGroup<NumAttr>>();

    // Push tuple to this new tuple group
    new_tuple_group_ptr->addTuple(data);

    // Store tuple group in table
    (*(this->tuple_groups))[this->last_tuple_group_index] = std::move(new_tuple_group_ptr);

}

template<int NumAttr>
void AmortizedAuroraTable<NumAttr>::startScan() {

    // Reset scan index to point to first tuple group
    this->scan_index = 0;

    // Reset scan index in all tuple groups (only if copy done)
    for (int i = 0; i <= this->last_tuple_group_index && i < this->to_copy_index; i++) {
        VersionedContiguousMemTupleGroup<NumAttr> *tuple_group = this->getTupleGroupAtIndex(i);
        tuple_group->startScan();
    }

    // Reset materialization threshold
    this->num_tuple_groups_materialized = 0;

}

template<int NumAttr>
template<int PrevNumAttr>
void AmortizedAuroraTable<NumAttr>::startScan(AmortizedAuroraTable<PrevNumAttr> &toCopy) {

    // Start scan on this table
    startScan();

    // Start scan on old table
    toCopy.startScan();

}

template<int NumAttr>
std::array<int, NumAttr> &AmortizedAuroraTable<NumAttr>::getNextTuple() {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index > this->last_tuple_group_index) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Make a lazy copy of the tuple group if needed
            if (this->to_copy_index <= this->scan_index) {
                assert(false);
            }

            // Try to scan the current tuple group
            VersionedContiguousMemTupleGroup<NumAttr> *curr_tuple_group = this->getTupleGroupAtIndex(this->scan_index);
            return curr_tuple_group->getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

template<int NumAttr>
template<int PrevNumAttr>
std::array<int, NumAttr> &AmortizedAuroraTable<NumAttr>::getNextTuple(AmortizedAuroraTable<PrevNumAttr> &toCopy) {

    while (true) {

        // Check if there are no more tuple groups to scan
        if (this->scan_index > this->last_tuple_group_index) {
            throw std::length_error("No more tuple groups to scan in table");
        }

        try {

            // Extract tuple group to copy
            VersionedContiguousMemTupleGroup<PrevNumAttr> *to_copy_tuple_group =
                    toCopy.getTupleGroupAtIndex(this->scan_index);

            // Check if tuple group needs to be materialized
            if (to_copy_tuple_group->getVersion() == Version::IN_DDL) {

                // Check if we allowed to materialize in this scan operation
                if (this->num_tuple_groups_materialized >= MAX_MATERIALIZATIONS_PER_QUERY) {

                    // Materialization not allowed
                    std::array<int, PrevNumAttr> &unmaterialized_tuple = to_copy_tuple_group->getNextTuple();
                    for (int i = 0; i < PrevNumAttr; i++) {
                        buffer_tuple[i] = unmaterialized_tuple[i];
                    }
                    for (int i = PrevNumAttr; i < NumAttr; i++) {
                        buffer_tuple[i] = unmaterialized_tuple[PrevNumAttr - 1];
                    }
                    return buffer_tuple;

                }

                // Copy into pointer
                auto new_tuple_group_ptr =
                        std::make_unique<VersionedContiguousMemTupleGroup<NumAttr>>(*to_copy_tuple_group);

                // Reset scan index and version
                new_tuple_group_ptr->startScan();

                // Copy directly into array
                (*(this->tuple_groups))[this->scan_index] = std::move(new_tuple_group_ptr);

                // Increment index
                this->to_copy_index++;

                // Increment materialization threshold
                this->num_tuple_groups_materialized++;
            }

            // Try to scan the current tuple group, should be materialized
            VersionedContiguousMemTupleGroup<NumAttr> *curr_tuple_group = this->getTupleGroupAtIndex(this->scan_index);
            return curr_tuple_group->getNextTuple();

        } catch (const std::length_error &e) {

            // Tuple group has been fully scanned, move onto next tuple group
            this->scan_index++;

        }
    }
}

//////// Other ////////

template<int NumAttr>
bool AmortizedAuroraTable<NumAttr>::isFull() const {
    return this->last_tuple_group_index >= NUMBER_TUPLE_GROUPS;
}

//////// Getters ////////

template<int NumAttr>
int AmortizedAuroraTable<NumAttr>::getLastTupleGroupIndex() const {
    return last_tuple_group_index;
}

template<int NumAttr>
int AmortizedAuroraTable<NumAttr>::getScanIndex() const {
    return scan_index;
}

template<int NumAttr>
VersionedContiguousMemTupleGroup<NumAttr> *AmortizedAuroraTable<NumAttr>::getTupleGroupAtIndex(int i) {
    return ((*(this->tuple_groups))[i]).get();
}
