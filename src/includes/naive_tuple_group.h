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
 * A naive tuple group implementation. On addition and deletion of a column:
 * - Copies each tuple into new tuples
 * - Does an atomic swap (across all tuples) after copying is done
 *
 * Templated with the number of columns before and after the DDL operation.
 * Call runDdlOperation to actually change the number of columns.
 */
template<int PreNumAttr, int PostNumAttr>
class NaiveTupleGroup :
        public virtual DdlOperable,
        public virtual DmlOperable<PreNumAttr> {

public:

    NaiveTupleGroup();

    ~NaiveTupleGroup() override = default;

    void runDdlOperation() override;

    void addTuple(std::array<int, PreNumAttr> data) override;

    void startScan() override;

    DbTuple<PreNumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    std::array<DbTuple<PreNumAttr>, NUMBER_TUPLES_PER_GROUP> tuples;

    int num_tuples_filled = 0;

    int scan_index = 0;

};


