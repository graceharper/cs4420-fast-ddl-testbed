//
// Created by tejun on 3/18/2019.
//


#pragma once


#include "dml_operable.h"
#include "constants.h"
#include "naive_tuple_group.h"

#include <array>

/**
 * A naive table implementation. On DDL operation (copy constructor):
 * - Locks the entire table
 * - Copies each tuple group into new tuple groups
 * - Does an atomic swap (across all tuple groups) after copying is done
 * - Unlocks the entire table
 */
template<int NumAttr>
class NaiveTable :
        public virtual DmlOperable<NumAttr> {

public:

    NaiveTable() = default;

    ~NaiveTable() override = default;

    template<int PrevNumAttr>
    NaiveTable(NaiveTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    DbTuple<NumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    // Default-initialization of array
    std::array<NaiveTupleGroup<NumAttr>, NUMBER_TUPLE_GROUPS> tuple_groups;

    int num_tuple_groups_filled;

    int scan_index;

};