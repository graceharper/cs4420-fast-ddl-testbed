# CS 4420 Fast DDL Testbed

> Benchmarking framework for common and custom row-oriented DDL implementations

## Team Information

Team #2: Fast DDL

[Teju Nareddy](https://github.com/nareddyt),
 [Sai Gundlapalli](https://github.com/graceharper),
 [Grace Harper](https://github.com/sgundlapalli11)

CS 4420 Spring 2019

## General Description 

This testbed implements scaled-down versions of Naive DDL (immediate full table copy) and Amazon's Aurora Instantaneous DDL. In both instances, the DDL implemented is adding a nullable column. Both DDL implementations are run on their own makeshift "databases."

For each DDL implementation, the testbed benchmarks how fast a full scan of the table is completed, both before and after the DDL operation is run. Addtionally, the testbed benchmarks the time for an individual tuple query (of the scan) to be completed, allowing users to get a better idea of how the cost of Aurora's DDL is amortized across each tuple. 

## Database Description

All Database code is in the `src` folder.

There is no proper database, but there are tables on which the DDL operations are run.
Each table consists of tuple groups, similar to pages for in-disk databases. There are multiple tuples to a tuple group and multiple tuple groups to a table. 

We have implemented the following types of tables so far:

1. _Naive Contiguous Memory Table_ - `naive_contiguous_mem_tuple_group.h`: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples in this table are stored contiguously in memory.
2. _Naive Random Memory Table_ - `naive_random_mem_tuple_group.h`: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples within tuple groups (pages) are stored contiguously in memory, but tuple groups themselves are located on random sections of the heap. This better represents an on-disk database, so we used this table as a baseline for our Fast DDL benchmark.
3. _Aurora Table_ - `aurora_table.h`: A mock implementation of the Amazon Aurora's Fast DDL feature, where tuple groups are lazily copied on tuple access. This was the main table under test and was compared to the benchmarks from the naive table's baseline. This was based off the following blog post - https://aws.amazon.com/blogs/database/amazon-aurora-under-the-hood-fast-ddl/

_Note_: All table implementations run in-memory but emulate disk-based systems.

_Note_: All table implementations are row-oriented (FSM).

## Benchmarks

All benchmark code is under the `tests` folder.
`tests.cpp` contains the respective implementations for the Naive and Aurora-style DDL.

### Running the Benchmarks

To run the benchmarks, begin run the following commands from the root project directory: 

```bash
mkdir build
cd build
cmake
make
cd bin
./test_suite
```

Note that executing `./test_suite` runs all gTests inside `tests.cpp`.

The output should be a table of metrics for each table type.

### Results and Analysis

See [test/README.md](test/README.md) for:

- An in-depth explanation of the benchmarks
- Our results from multiple runs of the benchmarks
- Our analysis and intuition behind the resulting metrics

## Supporting Documents

In addition to the documentation in this codebase, supporting documents include:

- [Project Proposal Presentation](https://docs.google.com/presentation/d/1oEdkolkFgda12XUJjeELfOa_b0pdHPLqFVg76Ybenbk/edit?usp=sharing)
- [Project Update Presentation](https://docs.google.com/presentation/d/102QaEbxZMtsjlURzfF2sE6JgidC0KcEO3V-eReXy_Ys/edit?usp=sharing)
- [Project Progress Report](https://drive.google.com/open?id=1OmBx_R3mu9gFxAckW7q-zrzkbeWWwoDy)