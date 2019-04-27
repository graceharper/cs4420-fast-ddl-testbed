//
// Created by tejun on 3/23/2019.
//


#pragma once


#include "constants.h"
#include "includes/tuple_group/naive_contiguous_mem_tuple_group.h"

#include <array>
#include <memory>

/**
 * Tuple groups are not stored contiguously.
 * Instead, they are created when needed (when a previous tuple group doesn't have space for a new tuple).
 *
 * Only materializes tuple groups when they are accessed (lazily).
 */
template<int NumAttr>
class AuroraTable {

public:

    AuroraTable();

    ~AuroraTable();

    template<int PrevNumAttr>
    explicit AuroraTable(AuroraTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data);

    void startScan();

    std::array<int, NumAttr> &getNextTuple();

    /**
     * For Aurora table, lazy copies tuple groups after copy.
     * Therefore, this function must be used! (instead of the one that takes no args)
     */
    template<int PrevNumAttr>
    std::array<int, NumAttr> &getNextTuple(AuroraTable<PrevNumAttr> &toCopy);

    bool isFull() const;

    // Getters

    NaiveContiguousMemTupleGroup<NumAttr> *getTupleGroupAtIndex(int i);

    int getLastTupleGroupIndex() const;

    int getScanIndex() const;

protected:

    std::array<std::unique_ptr<NaiveContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS> *tuple_groups = nullptr;

    int last_tuple_group_index = 0;

    int scan_index = 0;

    int to_copy_index = NUMBER_TUPLE_GROUPS + 1;

};

// Link to template implementation
#include "table/aurora_table.tpp"