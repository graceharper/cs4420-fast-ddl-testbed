//
// Created by tejun on 3/18/2019.
//


#pragma once


#include "constants.h"
#include "includes/tuple_group/naive_contiguous_mem_tuple_group.h"

#include <array>

/**
 * Stores entire table in contiguous memory. All tuples are contiguous, even between tuple groups.
 *
 * On DDL operation (copy constructor):
 * - Locks the entire table
 * - Copies each tuple group into new tuple groups
 * - Does an atomic swap (across all tuple groups) after copying is done
 * - Unlocks the entire table
 */
template<int NumAttr>
class NaiveContiguousMemTable {

public:

    NaiveContiguousMemTable();

    ~NaiveContiguousMemTable();

    template<int PrevNumAttr>
    explicit NaiveContiguousMemTable(NaiveContiguousMemTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data);

    void startScan();

    std::array<int, NumAttr> &getNextTuple();

    //Others
    bool isFull() const;

    // Getters
    int getLastTupleGroupIndex() const;
    NaiveContiguousMemTupleGroup<NumAttr>& getTupleGroupAtIndex(int i);
    int getScanIndex() const;

protected:

    std::array<NaiveContiguousMemTupleGroup<NumAttr>, NUMBER_TUPLE_GROUPS> *tuple_groups = nullptr;

    int last_tuple_group_index = 0;

    int scan_index = 0;

};

// Link to template implementation
#include "table/naive_contiguous_mem_table.tpp"