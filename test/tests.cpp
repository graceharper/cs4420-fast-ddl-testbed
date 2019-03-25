//
// Created by tejun on 3/18/2019.
//

#include <iostream>
#include <chrono>

#include <gtest/gtest.h>
#include "naive_contiguous_mem_table.h"
#include "naive_random_mem_table.h"
#include "testing_constants.h"
#include "db_tuple.h"
//Because the table constructors use templates
//the size of the table must be a compile time constant
//hence the testing_constants.h file

#define LOG(X) std::cout << X << std::endl


std::array<int, SMALL_NUM_COLS> defaultTuple;

//initiates the default tuple that gets added during queries
void setup() {
    for (int i = 0; i < SMALL_NUM_COLS; i++) {
        defaultTuple[i] = i;
    }
}


/**
 * Adds tuples to a table.
 * Benchmarks average tuple addition time.
 */
template<int NumCols>
void addTuples(NaiveContiguousMemTable<NumCols> &table) {

    //query before ddl
    auto startq1 = std::chrono::high_resolution_clock::now();
    table.addTuple(defaultTuple);
    auto added = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> q1time = added - startq1;

    //run ddl
    auto startDDL = std::chrono::high_resolution_clock::now();
    NaiveContiguousMemTable<SMALL_NUM_COLS> newTable(table);
    auto endDDl = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> rDDLtime = endDDl - startDDL;

    //query after ddl
    auto startq2 = std::chrono::high_resolution_clock::now();
    table.addTuple(defaultTuple);
    auto endq2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> q2time = endq2 - startq2;

    LOG("Query 1 Time:" << q1time.count());
    LOG("DDL Time: " << rDDLtime.count());
    LOG("Query 2 Time: " << q2time.count());

}

/**
 * Scans all tuples in the table.
 * Benchmarks time to scan a tuple.
 */
template<int NumCols>
void scanTuples(NaiveContiguousMemTable<NumCols> &table) {


    //query before ddl
    auto startq1 = std::chrono::high_resolution_clock::now();
    table.startScan();
    //messy
    while (true) {
        try {
            table.getNextTuple();
        } catch (std::length_error &e) {
            LOG(e.what());
            break;
        }
    }
    auto added = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> q1time = added - startq1;

    //run ddl
    auto startDDL = std::chrono::high_resolution_clock::now();
    NaiveContiguousMemTable<SMALL_NUM_COLS> newTable(table);
    auto endDDl = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> rDDLtime = endDDl - startDDL;
    LOG("Query 1 Time:" << q1time.count());
    LOG("DDL Time: " << rDDLtime.count());
}

void NaiveContMemTest(const int numRows) {
    //TODO multiple tables
    if (numRows > (SMALL_NUM_COLS - 2)) {
        std::cout << "too many rows requested" << std::endl;
        return;
    }

    //set up table
    NaiveContiguousMemTable<SMALL_NUM_COLS> smallTable;

    // Benchmark operations
    addTuples(smallTable);
    scanTuples(smallTable);

    // Perform ddl with benchmarks
    NaiveContiguousMemTable<BIG_NUM_COLS> bigTable(smallTable);

    // Benchmark operations again
    scanTuples(bigTable);
    scanTuples(bigTable);


}

TEST(DdlTest, testNaiveContiguousMem) {
    NaiveContMemTest(4);
}