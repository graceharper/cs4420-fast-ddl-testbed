//
// Created by tejun on 3/18/2019.
//

#include "dml_operable.h"
#include "db_tuple.h"
#include "constants.h"

#include <array>

#pragma once

/**
 * Stores all tuples in contiguous memory. Memory is pre-allocated at compile time.
 *
 * On DDL operation (copy constructor):
 * - Copies each tuple into new tuples
 * - Does an atomic swap (across all tuples) after copying is done
 */
template<int NumAttr>
class NaiveContiguousMemTupleGroup :
        public virtual DmlOperable<NumAttr> {

public:

    NaiveContiguousMemTupleGroup() = default;

    ~NaiveContiguousMemTupleGroup() override = default;

    template<int PrevNumAttr>
    NaiveContiguousMemTupleGroup(NaiveContiguousMemTupleGroup<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    std::array<int, NumAttr> &getNextTuple() override;

    bool isFull() const;

    // Getters

    DbTuple<NumAttr> &getTupleAtIndex(int i);

    int getLastTupleIndex() const;

    int getScanIndex() const;

protected:

    // Default-initialization of array
    std::array<DbTuple<NumAttr>, NUMBER_TUPLES_PER_GROUP> tuples;

    int last_tuple_index = 0;

    int scan_index = 0;

};

// Link to template implementation
#include "naive_contiguous_mem_tuple_group.tpp"

