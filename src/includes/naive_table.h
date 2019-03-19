//
// Created by tejun on 3/18/2019.
//


#pragma once


#include "ddl_operable.h"
#include "dml_operable.h"

#include <vector>

constexpr size_t NUMBER_TUPLE_GROUPS = 8;

/**
 * A naive table implementation. On addition and deletion of a column:
 * - Locks the entire table
 * - Copies each tuple group into new tuple groups
 * - Does an atomic swap (across all tuple groups) after copying is done
 * - Unlocks the entire table
 */
template<typename TupleGroupType>
class NaiveTable :
        public virtual DdlOperable,
        public virtual DmlOperable {

public:

    NaiveTable();

    ~NaiveTable() override = default;

    void addColumnsToEnd(int numColumns) override;

    void removeColumnsFromEnd(int numColumns) override;

    void addTuple(std::tuple<int> data) override;

    void startScan() override;

    std::tuple<int> &getNextTuple() override;

protected:

    std::vector<TupleGroupType> tuple_groups;

};