//
// Created by tejun on 3/18/2019.
//

#include "naive_table.h"
#include "db_tuple.h"

#include <iostream>

int main() {
    std::cout << "Hello world :)" << std::endl;

    // Make a Table of 4 columns
    NaiveTable<4> smallTable;

    // Add a few tuples to it
    std::array<int, 4> tuple = {1, 1, 1, 1};
    smallTable.addTuple(tuple);

    tuple = {2, 2, 2, 2};
    smallTable.addTuple(tuple);

    tuple = {3, 3, 3, 3};
    smallTable.addTuple(tuple);

    // Make a new table of 7 columns that copies everything from the smaller table
    // Since this is a "naive" table, its expected that EVERYTHING is copied right now
    NaiveTable<7> bigTable(smallTable);

}