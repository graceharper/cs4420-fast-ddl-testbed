//
// Created by tejun on 3/18/2019.
//

#include <gtest/gtest.h>
#include <chrono>
#include <array>
#include <iostream>
#include <fstream>
#include <dirent.h>

#include "includes/table/aurora_table.h"
#include "includes/table/amortized_aurora_table.h"
#include "includes/table/naive_contiguous_mem_table.h"
#include "includes/table/naive_random_mem_table.h"
#include "testing_constants.h"
#include "includes/tuple/db_tuple.h"
//Because the table constructors use templates
//the size of the table must be a compile time constant
//hence the testing_constants.h file

const std::string OUTPUT_DIR = "./benchmark_output/";

#define LOG(X) std::cout << X << std::endl

std::array<int, SMALL_NUM_COLS> unmaterialized_tuple;
std::array<int, BIG_NUM_COLS> materialized_tuple;

void delete_files_in_folder(const std::string &folder_path) {
    DIR *folder = opendir(folder_path.c_str());
    struct dirent *next_file;

    while ((next_file = readdir(folder)) != nullptr) {
        // build the path for each file in the folder
        std::string file_path = folder_path + next_file->d_name;
        remove(file_path.c_str());
    }
    closedir(folder);
}

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

    std::cout << "addTuple:\t\t" << std::flush;

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

    LOG(queryTimeTotal.count() * 1000
                << "\t" << queryTimeMax.count() * 1000
                << "\t" << queryTimeAvg.count() * 1000
                << "\t" << queryTimeMin.count() * 1000
                << "\t" << NUM_TUPLES);

}

/**
 * Default query tuple function.
 * Note that small_table is not used by default.
 */
template<template<int> typename TableType, int NumCols, int PrevNumCols>
std::array<int, NumCols> &getTuple(TableType<NumCols> &table, TableType<PrevNumCols> &small_table,
                                   std::chrono::time_point<std::chrono::high_resolution_clock> &end_query) {

    std::array<int, NumCols> &tuple = table.getNextTuple();
    end_query = std::chrono::high_resolution_clock::now();
    return tuple;
}

/**
 * Template specialization for Aurora after DDL.
 */
template<>
std::array<int, BIG_NUM_COLS> &getTuple<AuroraTable, BIG_NUM_COLS, SMALL_NUM_COLS>(
        AuroraTable<BIG_NUM_COLS> &table,
        AuroraTable<SMALL_NUM_COLS> &small_table,
        std::chrono::time_point<std::chrono::high_resolution_clock> &end_query) {

    std::array<int, BIG_NUM_COLS> &tuple = table.getNextTuple(small_table);
    end_query = std::chrono::high_resolution_clock::now();
    return tuple;
}

/**
 * Template specialization for Amortized Aurora after DDL.
 */
template<>
std::array<int, BIG_NUM_COLS> &getTuple<AmortizedAuroraTable, BIG_NUM_COLS, SMALL_NUM_COLS>(
        AmortizedAuroraTable<BIG_NUM_COLS> &table,
        AmortizedAuroraTable<SMALL_NUM_COLS> &small_table,
        std::chrono::time_point<std::chrono::high_resolution_clock> &end_query) {

    bool materialized = true;
    std::array<int, BIG_NUM_COLS> &tuple = table.getNextTuple<SMALL_NUM_COLS>(small_table, unmaterialized_tuple,
                                                                              materialized);
    end_query = std::chrono::high_resolution_clock::now();

    // Check if it was successfully materialized
    if (materialized) {
        return tuple;
    }

    // Otherwise must modify contents for durability check in caller
    for (int i = 0; i < SMALL_NUM_COLS; i++) {
        materialized_tuple[i] = unmaterialized_tuple[i];
    }
    for (int i = SMALL_NUM_COLS; i < BIG_NUM_COLS; i++) {
        materialized_tuple[i] = materialized_tuple[i - 1];
    }

    return materialized_tuple;
}

template<template<int> typename TableType, int NumCols, int PrevNumCols>
void startScan(TableType<NumCols> &table, TableType<PrevNumCols> &small_table) {
    table.startScan();
}

/**
 * Template specialization for Amortized Aurora after DDL.
 */
template<>
void startScan<AmortizedAuroraTable, BIG_NUM_COLS, SMALL_NUM_COLS>(AmortizedAuroraTable<BIG_NUM_COLS> &table,
                                                                   AmortizedAuroraTable<SMALL_NUM_COLS> &small_table) {
    table.startScan(small_table);
}

/**
 * Scans all tuples in the table.
 * Benchmarks time to scan a tuple.
 */
template<template<int> typename TableType, int NumCols, int PrevNumCols>
void scanTuples(TableType<NumCols> &table, TableType<PrevNumCols> &small_table, int scanNum, std::ofstream &outfile) {

    std::cout << "getNextTuple:\t" << std::flush;

    // Keep track of metrics
    std::chrono::duration<double> queryTimeMax(0);
    std::chrono::duration<double> queryTimeAvg(0);
    std::chrono::duration<double> queryTimeMin(999999);
    std::chrono::duration<double> queryTimeTotal(0);

    // Start scan
    startScan<TableType, NumCols, PrevNumCols>(table, small_table);

    // Run through each tuple in scan
    for (int i = 0; i < NUM_TUPLES; i++) {

        // Run query with metrics
        auto startQuery = std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> endQuery;

        // Note this automatically uses the default or specialized versions as needed
        const std::array<int, NumCols> &actual_tuple =
                getTuple<TableType, NumCols, PrevNumCols>(table, small_table, endQuery);
        const std::chrono::duration<double> queryTime = endQuery - startQuery;

        // Durability check
        const std::array<int, NumCols> &expected_tuple = generateTuple<NumCols>(i);
        ASSERT_EQ(actual_tuple, expected_tuple) << "Durability broken...";

        // Write query time to file
        outfile << queryTime.count() * 1000 << std::endl;

        // Update metrics
        queryTimeMax = std::max(queryTimeMax, queryTime);
        queryTimeMin = std::min(queryTimeMin, queryTime);
        queryTimeTotal += queryTime;
    }

    // Calculate average
    queryTimeAvg = queryTimeTotal / NUM_TUPLES;

    // Assert next scan throws exception
    try {
        std::chrono::time_point<std::chrono::high_resolution_clock> endQuery;
        getTuple<TableType, NumCols, PrevNumCols>(table, small_table, endQuery);
        FAIL() << "Did not throw exception when scanning for extra tuples";
    } catch (const std::length_error &e) {
        // Expected
    }

    LOG(queryTimeTotal.count() * 1000
                << "\t" << queryTimeMax.count() * 1000
                << "\t" << queryTimeAvg.count() * 1000
                << "\t" << queryTimeMin.count() * 1000
                << "\t" << NUM_TUPLES
                << "\t\t" << "Full Table Scan #" << scanNum);
}

template<template<int> typename TableType>
void runTest() {
    // Log initial information
    const std::string test_name = testing::UnitTest::GetInstance()->current_test_info()->name();
    LOG("========= " << test_name << " Implementation =========");
    LOG("");

    LOG("=== General Config ===");
    LOG("PreDDL Num Columns:\t\t\t\t\t\t" << SMALL_NUM_COLS);
    LOG("PostDDL Num Columns:\t\t\t\t\t" << BIG_NUM_COLS);
    LOG("Full Scans before DDL:\t\t\t\t\t" << NUM_FULL_SCANS_PRE_DDL);
    LOG("Full Scans after DDL:\t\t\t\t\t" << NUM_FULL_SCANS_POST_DDL);
    LOG("Number Tuple Groups:\t\t\t\t\t" << NUMBER_TUPLE_GROUPS);
    LOG("Number Tuples Per Group:\t\t\t\t" << NUMBER_TUPLES_PER_GROUP);
    LOG("Total Number Tuples:\t\t\t\t\t" << NUM_TUPLES);
    LOG("");

    LOG("=== Amortized Aurora Config ===");
    LOG("Tuple Group Materializations Per Scan:\t" << MAX_MATERIALIZATIONS_PER_QUERY);
    LOG("");

    // Delete all files in folder
    const std::string output_folder_name = OUTPUT_DIR + test_name;
    delete_files_in_folder(output_folder_name);

    // Setup formatting
    std::cout << std::setprecision(4) << std::scientific;
    LOG("\t\t\t\tTotal(ms)\tMax(ms)\t\tAvg(ms)\t\tMin(ms)\t\tCount\tNotes");

    // Set up table
    TableType<SMALL_NUM_COLS> smallTable;

    // Benchmark operations
    addTuples(smallTable);
    for (int i = 0; i < NUM_FULL_SCANS_PRE_DDL; i++) {
        const std::string file_name = output_folder_name + "/pre-dll-" + std::to_string(i) + ".out";
        std::ofstream outfile(file_name);
        outfile << "getNextTupleTime(ms)" << std::endl;
        scanTuples(smallTable, smallTable, i, outfile);
        outfile.close();
    }

    // Perform ddl with benchmarks
    std::cout << "addColumns:\t\t" << std::flush;
    const auto startDdl = std::chrono::high_resolution_clock::now();

    TableType<BIG_NUM_COLS> bigTable(smallTable);

    const auto endDdl = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> rDdlTime = endDdl - startDdl;
    LOG(rDdlTime.count() * 1000
                << "\t\t\t\t\t\t\t\t\t\t" << 1);

    // Benchmark operations again
    for (int i = 0; i < NUM_FULL_SCANS_POST_DDL; i++) {
        const std::string file_name = output_folder_name + "/post-dll-" + std::to_string(i) + ".out";
        std::ofstream outfile(file_name);
        outfile << "getNextTupleTime(ms)" << std::endl;
        scanTuples(bigTable, smallTable, i, outfile);
        outfile.close();
    }

    LOG("Done");
}

//TEST(DdlTest, NaiveContiguousMemory) {
//    LOG("========= Naive Contiguous Memory =========");
//    runTest<NaiveContiguousMemTable>();
//}

TEST(DdlTest, NaiveRandomMemory) {
    runTest<NaiveRandomMemTable>();
}

TEST(DdlTest, Aurora) {
    runTest<AuroraTable>();
}

TEST(DdlTest, AmortizedAurora) {
    runTest<AmortizedAuroraTable>();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}