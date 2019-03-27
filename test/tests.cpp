//
// Created by tejun on 3/18/2019.
//

#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <array>

#include <aurora_table.h>
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

    // Keep track of metrics
    std::chrono::duration<double> queryTimeMax(0);
    std::chrono::duration<double> queryTimeAvg(0);
    std::chrono::duration<double> queryTimeMin(999999);
    std::chrono::duration<double> queryTimeTotal(0);

    for (int i = 0; i < NUM_TUPLES; i++) {
        // Run query with metrics
        auto startQuery = std::chrono::high_resolution_clock::now();

        // Actually do operation
        const std::array<int, NumCols> tuple = generateTuple<NumCols>(i);
        table.addTuple(tuple);

        // Stop timing
        auto endQuery = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> queryTime = endQuery - startQuery;

        // Update metrics
        queryTimeMax = std::max(queryTimeMax, queryTime);
        queryTimeMin = std::min(queryTimeMin, queryTime);
        queryTimeTotal += queryTime;
    }

    // Calculate average
    queryTimeAvg = queryTimeTotal / NUM_TUPLES;

    LOG("addTuple:\t\t" << queryTimeTotal.count() * 1000
                        << "\t" << queryTimeMax.count() * 1000
                        << "\t" << queryTimeAvg.count() * 1000
                        << "\t" << queryTimeMin.count() * 1000);

}

/**
 * Default query tuple function.
 * Note that small_table is not used by default.
 */
template<template<int> typename TableType, int NumCols, int PrevNumCols>
std::array<int, NumCols> &getTuple(TableType<NumCols> &table, TableType<PrevNumCols> &small_table) {
    return table.getNextTuple();
}

/**
 * Template specialization for Aurora after DDL.
 */
template<>
std::array<int, BIG_NUM_COLS> &getTuple<AuroraTable, BIG_NUM_COLS, SMALL_NUM_COLS>(AuroraTable<BIG_NUM_COLS> &table,
                                                                                   AuroraTable<SMALL_NUM_COLS> &small_table) {
    return table.getNextTuple(small_table);
}

/**
 * Scans all tuples in the table.
 * Benchmarks time to scan a tuple.
 */
template<template<int> typename TableType, int NumCols, int PrevNumCols>
void scanTuples(TableType<NumCols> &table, TableType<PrevNumCols> &small_table) {

    // Keep track of metrics
    std::chrono::duration<double> queryTimeMax(0);
    std::chrono::duration<double> queryTimeAvg(0);
    std::chrono::duration<double> queryTimeMin(999999);
    std::chrono::duration<double> queryTimeTotal(0);

    // Start scan
    table.startScan();

    // Run through each tuple in scan
    for (int i = 0; i < NUM_TUPLES; i++) {

        // Run query with metrics
        auto startQuery = std::chrono::high_resolution_clock::now();

        // Note this automatically uses the default or specialized versions as needed
        const std::array<int, NumCols> &actual_tuple = getTuple<TableType, NumCols, PrevNumCols>(table, small_table);

        auto endQuery = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> queryTime = endQuery - startQuery;

        // Durability check
        const std::array<int, NumCols> &expected_tuple = generateTuple<NumCols>(i);
        ASSERT_EQ(actual_tuple, expected_tuple) << "Durability broken...";

        // Update metrics
        queryTimeMax = std::max(queryTimeMax, queryTime);
        queryTimeMin = std::min(queryTimeMin, queryTime);
        queryTimeTotal += queryTime;
    }

    // Calculate average
    queryTimeAvg = queryTimeTotal / NUM_TUPLES;

    // Assert next scan throws exception
    try {
        getTuple<TableType, NumCols, PrevNumCols>(table, small_table);
        FAIL() << "Did not throw exception when scanning for extra tuples";
    } catch (const std::length_error &e) {
        // Expected
    }

    LOG("getNextTuple:\t" << queryTimeTotal.count() * 1000
                          << "\t" << queryTimeMax.count() * 1000
                          << "\t" << queryTimeAvg.count() * 1000
                          << "\t" << queryTimeMin.count() * 1000);
}

template<template<int> typename TableType>
void runTest() {
    // Log metrics information
    std::cout << std::setprecision(6) << std::fixed;
    LOG("\t\t\t\tTotal(ms)\tMax(ms)\t\tAvg(ms)\t\tMin(ms)");

    // Set up table
    TableType<SMALL_NUM_COLS> smallTable;

    // Benchmark operations
    addTuples(smallTable);
    for (int i = 0; i < 5; i++) {
        scanTuples(smallTable, smallTable);
    }

    // Perform ddl with benchmarks
    const auto startDdl = std::chrono::high_resolution_clock::now();

    TableType<BIG_NUM_COLS> bigTable(smallTable);

    const auto endDdl = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> rDdlTime = endDdl - startDdl;
    LOG("ddl:\t\t\t" << rDdlTime.count() * 1000);

    // Benchmark operations again
    for (int i = 0; i < 5; i++) {
        scanTuples(bigTable, smallTable);
    }
}

TEST(DdlTest, NaiveContiguousMemory) {
    LOG("=== Naive Contiguous Memory ===");
    runTest<NaiveContiguousMemTable>();
}

TEST(DdlTest, NaiveRandomMemory) {
    LOG("=== Naive Random Memory ===");
    runTest<NaiveRandomMemTable>();
}

TEST(DdlTest, Aurora) {
    LOG("=== Aurora Implementation ===");
    runTest<AuroraTable>();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}