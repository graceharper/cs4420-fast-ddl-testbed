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
    std::cout << "Created small table" << std::endl;

    // Add a few tuples to it
    smallTable.addTuple({1, 1, 1, 1});
    smallTable.addTuple({2, 2, 2, 2});
    smallTable.addTuple({3, 3, 3, 3});
    std::cout << "Added tuples to small table" << std::endl;

    // FIXME Verify tuples
//    smallTable.startScan();
//    for (int i = 0; i < 3; i++) {
//        std::array<int, 4> tuple_out = smallTable.getNextTuple();
//        std::cout << "Tuple " << i << " is " << tuple_out.data() << std::endl;
//    }

    // Make a new table of 7 columns that copies everything from the smaller table
    // Since this is a "naive" table, its expected that EVERYTHING is copied right now
    NaiveTable<7> bigTable(smallTable);
    std::cout << "Copied small table to big table" << std::endl;

    // Add another tuple (this time with 7 columns)
    bigTable.addTuple({4, 4, 4, 4, 4, 4, 4});
    std::cout << "Added tuples to big table" << std::endl;

    return 0;
}