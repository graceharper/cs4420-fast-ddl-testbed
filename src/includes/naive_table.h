//
// Created by tejun on 3/18/2019.
//


#pragma once


#include "ddl_operable.h"
#include "dml_operable.h"

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

    ~NaiveTable() = default;

    void addColumnsToEnd(int numColumns) override;

    void removeColumnsFromEnd(int numColumns) override;

    void startScan() override;

    std::tuple<int> &getNextTuple() override;
};


