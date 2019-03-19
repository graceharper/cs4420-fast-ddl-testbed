//
// Created by tejun on 3/18/2019.
//

#include "ddl_operable.h"
#include "dml_operable.h"

#include <vector>
#include <tuple>

#pragma once

constexpr size_t NUMBER_TUPLES_PER_GROUP = 16;

/**
 * A naive tuple group implementation. On addition and deletion of a column:
 * - Copies each tuple into new tuples
 * - Does an atomic swap (across all tuples) after copying is done
 */
class NaiveTupleGroup :
        public virtual DdlOperable,
        public virtual DmlOperable {

public:

    NaiveTupleGroup();

    ~NaiveTupleGroup() override = default;

    void addColumnsToEnd(int numColumns) override;

    void removeColumnsFromEnd(int numColumns) override;

    void addTuple(std::tuple<int> data) override;

    void startScan() override;

    std::tuple<int> &getNextTuple() override;

protected:

    std::vector<std::tuple<int>> tuples;

};


