//
// Created by tejun on 3/18/2019.
//

#include "ddl_operable.h"
#include "dml_operable.h"
#include "db_tuple.h"
#include "constants.h"

#include <array>

#pragma once

/**
 * A naive tuple group implementation. On DDL operation (copy constructor):
 * - Copies each tuple into new tuples
 * - Does an atomic swap (across all tuples) after copying is done
 */
template<int NumAttr>
class NaiveTupleGroup :
        public virtual DmlOperable<NumAttr> {

public:

    NaiveTupleGroup();

    ~NaiveTupleGroup() override = default;

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    DbTuple<NumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    std::array<DbTuple<NumAttr>, NUMBER_TUPLES_PER_GROUP> tuples;

    int num_tuples_filled = 0;

    int scan_index = 0;

};


