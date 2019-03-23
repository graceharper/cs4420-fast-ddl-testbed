//
// Created by tejun on 3/23/2019.
//


#pragma once


#include "dml_operable.h"
#include "constants.h"
#include "naive_contiguous_mem_tuple_group.h"

#include <array>
#include <memory>

/**
 * Tuple groups are not stored contiguously. Instead, they are created when needed (running out of space).
 *
 * On DDL operation (copy constructor):
 * - Locks the entire table
 * - Copies each tuple group into new tuple groups
 * - Does an atomic swap (across all tuple groups) after copying is done
 * - Unlocks the entire table
 */
template<int NumAttr>
class NaiveRandomMemTable :
        public virtual DmlOperable<NumAttr> {

public:

    NaiveRandomMemTable() = default;

    ~NaiveRandomMemTable() override = default;

    template<int PrevNumAttr>
    NaiveRandomMemTable(NaiveRandomMemTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    std::array<int, NumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    // Default-initialization of array
    std::array<std::unique_ptr<NaiveContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS> tuple_groups;

    int num_tuple_groups_filled;

    int scan_index;

};

// Link to template implementation
#include "naive_random_mem_table.tpp"