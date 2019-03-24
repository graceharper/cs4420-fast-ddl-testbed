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

class DDLTest {
    public:
    //constructor --  # tables to create, tableSize
    DDLTest() {
        setup();
    }
    std::array<int, TABLESIZE> defaultTuple;
    ~DDLTest() = default;

    //initiates the default tuple that gets added during queries
    void setup(){
        for(int i = 0; i < TABLESIZE; i++){
            defaultTuple[i] = i;
        }
    }

    //creates a NaiveContinuousMemoryTable and runs tests on it
    void NaiveContMemTest(const int numRows, int fast) {
        //TODO multiple tables
        if(numRows > (TABLESIZE - 2)) {
            std::cout << "too many rows requested" << std::endl;
            return;
        }
        //set up table
        NaiveContiguousMemTable<TABLESIZE>* ctable = new NaiveContiguousMemTable<TABLESIZE>();
        for(int i = 0; i < numRows; i ++) {
            ctable->addTuple(defaultTuple);
        }

        if(fast) {
            LOG("fast not yet implememnted");
        } else {
            regularcDDLadd(ctable);
            regularcDDLscan(ctable);
        }


    }

    //creates a NaiveRandomMemoryTable and runs tests on it
    void NaiveRandMemTest(const int fast, const int numTables, const int numRows){
        //TODO multiple tables
        NaiveRandomMemTable<TABLESIZE> *rtable = new NaiveRandomMemTable<TABLESIZE>();
        for(int i = 0; i < numRows; i ++) {
            rtable->addTuple(defaultTuple);
        }

    }





    //Benchmarks a Regular DDL's affects on adding a tuple.
    void regularcDDLadd(NaiveContiguousMemTable<TABLESIZE>* ctable) {

        //query before ddl
        auto startq1 = std::chrono::high_resolution_clock::now();
        ctable->addTuple(defaultTuple);
        auto added = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> q1time = added - startq1;

        //run ddl
        auto startDDL = std::chrono::high_resolution_clock::now();
        NaiveContiguousMemTable<TABLESIZE>* newctable(ctable);
        auto endDDl = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> rDDLtime = endDDl - startDDL;

        //query after ddl
        auto startq2 = std::chrono::high_resolution_clock::now();
        ctable->addTuple(defaultTuple);
        auto endq2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> q2time = endq2 - startq2;

        LOG("Query 1 Time:" << q1time.count());
        LOG("DDL Time: " << rDDLtime.count());
        LOG("Query 2 Time: " << q2time.count());

    }

    //Benchmarks a Regular DDL's affects on adding a tuple.
    void regularcDDLscan(NaiveContiguousMemTable<TABLESIZE>* ctable) {


        //query before ddl
        auto startq1 = std::chrono::high_resolution_clock::now();
        ctable->startScan();
        //messy
        while(true) {
            try {
                ctable->getNextTuple();
            } catch (std::length_error &e){
                LOG(e.what());
                break;
            }
        }
        auto added = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> q1time = added - startq1;

        //run ddl
        auto startDDL = std::chrono::high_resolution_clock::now();
        NaiveContiguousMemTable<TABLESIZE>* newctable(ctable);
        auto endDDl = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> rDDLtime = endDDl - startDDL;
        LOG("Query 1 Time:" << q1time.count());
        LOG("DDL Time: " << rDDLtime.count());
    }
};



DDLTest *ddlTest;

TEST(DDLTest, testContiguousMemRegDDLadd) {
    ddlTest->NaiveContMemTest(4, 0);
}

int main(int argc, char *argv[]) {
    //not sure what this does
    ddlTest = new DDLTest();
    ::testing::InitGoogleTest(&argc, argv);
    int executionResult = RUN_ALL_TESTS();
}

//create many tables
//