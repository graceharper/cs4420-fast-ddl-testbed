//
// Created by tejun on 3/18/2019.
//

#include <iostream>
#include <chrono>
#include <array>

#include <gtest/gtest.h>
#include "naive_contiguous_mem_table.h"
#include "naive_random_mem_table.h"
#include "testing_constants.h"
#include "db_tuple.h"
//Because the table constructors use templates
//the size of the table must be a compile time constant
//hence the testing_constants.h file

#define LOG(X) std::cout << X << std::endl


/**
 * Generates a tuple of the correct size, where all elements in the tuple are set the it's id.
 *
 * @return the tuple with the correct data set
 */
template<int NumCols>
std::array<int, NumCols> generateTuple(int tupleId) {

    std::array<int, NumCols> tuple = {};

    // Set variables
    for (int i = 0; i < NumCols; i++) {
        tuple[i] = tupleId;
    }

    return tuple;

}

/**
 * Adds tuples to a table.
 * Benchmarks average tuple addition time.
 */
template<template<int> typename TableType, int NumCols>
void addTuples(TableType<NumCols> &table) {

    const auto startAdd = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_TUPLES; i++) {
        const std::array<int, NumCols> tuple = generateTuple<NumCols>(i);
        table.addTuple(tuple);
    }

    const auto endAdd = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> addTime = endAdd - startAdd;

    LOG("Add tuples to table: " << addTime.count());

}

/**
 * Scans all tuples in the table.
 * Benchmarks time to scan a tuple.
 */
template<template<int> typename TableType, int NumCols>
void scanTuples(TableType<NumCols> &table) {

    auto startScan = std::chrono::high_resolution_clock::now();
    table.startScan();

    for (int i = 0; i < NUM_TUPLES; i++) {
        const std::array<int, NumCols> &actual_tuple = table.getNextTuple();
        const std::array<int, NumCols> &expected_tuple = generateTuple<NumCols>(i);
        ASSERT_EQ(actual_tuple, expected_tuple) << "Durability broken...";
    }

    // Assert next scan throws exception
    try {
        table.getNextTuple();
        FAIL() << "Did not throw exception when scanning for extra tuples";
    } catch (const std::length_error &e) {
        // Expected
    }

    auto endScan = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> scanTime = endScan - startScan;

    LOG("Query tuples from table: " << scanTime.count());
}

template<template<int> typename TableType>
void runTest() {
    // Set up table
    TableType<SMALL_NUM_COLS> smallTable;

    // Benchmark operations
    addTuples(smallTable);
    scanTuples(smallTable);

    // Perform ddl with benchmarks
    const auto startDdl = std::chrono::high_resolution_clock::now();

    TableType<BIG_NUM_COLS> bigTable(smallTable);

    const auto endDdl = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> rDdlTime = endDdl - startDdl;
    LOG("DDL to new table: " << rDdlTime.count());

    // Benchmark operations again
    scanTuples(bigTable);
    scanTuples(bigTable);
}

TEST(DdlTest, NaiveContiguousMemory) {
    LOG("=== Naive Contiguous Memory ===");
    runTest<NaiveContiguousMemTable>();
}

TEST(DdlTest, NaiveRandomMemory) {
    LOG("=== Naive Random Memory ===");
    runTest<NaiveRandomMemTable>();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}