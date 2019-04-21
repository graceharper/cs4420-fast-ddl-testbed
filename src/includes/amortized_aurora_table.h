//
// Created by tejun on 4/20/2019.
//


#pragma once


#include "constants.h"
#include "versioned_contiguous_mem_tuple_group.h"

#include <array>
#include <memory>

/**
 * TODO
 */
template<int NumAttr>
class AmortizedAuroraTable {

public:

    AmortizedAuroraTable();

    ~AmortizedAuroraTable();

    template<int PrevNumAttr>
    explicit AmortizedAuroraTable(AmortizedAuroraTable<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data);

    void startScan();

    template<int PrevNumAttr>
    void startScan(AmortizedAuroraTable<PrevNumAttr> &toCopy);

    std::array<int, NumAttr> &getNextTuple();

    template<int PrevNumAttr>
    std::array<int, NumAttr> &getNextTuple(AmortizedAuroraTable<PrevNumAttr> &toCopy,
                                           std::array<int, PrevNumAttr> &unmaterialized_tuple,
                                           bool &materialized);

    bool isFull() const;

    // Getters

    VersionedContiguousMemTupleGroup<NumAttr> *getTupleGroupAtIndex(int i);

    int getLastTupleGroupIndex() const;

    int getScanIndex() const;

protected:

    std::array<std::unique_ptr<VersionedContiguousMemTupleGroup<NumAttr>>, NUMBER_TUPLE_GROUPS> *tuple_groups = nullptr;

    int last_tuple_group_index = 0;

    int scan_index = 0;

    int to_copy_index = NUMBER_TUPLE_GROUPS + 1;

    int num_tuple_groups_materialized = 0;

    // Default-initialization of array
    std::array<int, NumAttr> buffer_tuple;

};

// Link to template implementation
#include "amortized_aurora_table.tpp"