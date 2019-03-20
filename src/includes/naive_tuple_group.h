//
// Created by tejun on 3/18/2019.
//

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

    NaiveTupleGroup() = default;

    ~NaiveTupleGroup() override = default;

    template<int PrevNumAttr>
    NaiveTupleGroup(NaiveTupleGroup<PrevNumAttr> &toCopy);

    void addTuple(std::array<int, NumAttr> data) override;

    void startScan() override;

    DbTuple<NumAttr> &getNextTuple() override;

    bool isFull() const;

protected:

    // Default-initialization of array
    std::array<DbTuple<NumAttr>, NUMBER_TUPLES_PER_GROUP> tuples;

    int num_tuples_filled = 0;

    int scan_index = 0;

};


