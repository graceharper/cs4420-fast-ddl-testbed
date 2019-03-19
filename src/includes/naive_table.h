//
// Created by tejun on 3/18/2019.
//


#pragma once


#include "ddl_operable.h"
#include "dml_operable.h"
#include "constants.h"
#include "naive_tuple_group.h"

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

    NaiveTable();

    ~NaiveTable() override = default;

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    DbTuple<NumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    std::vector<NaiveTupleGroup<NumAttr>> tuple_groups;

    int scan_index = 0;

};