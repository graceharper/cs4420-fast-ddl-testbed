# CS 4420 Fast DDL Testbed

> Benchmarking framework for common and custom row-oriented DDL implementations

  * [Team Information](#team-information)
  * [General Description](#general-description)
  * [Supported Table Implementations](#supported-table-implementations)
  * [Benchmarks](#benchmarks)
     * [Results and Analysis](#results-and-analysis)
     * [Running the Benchmarks](#running-the-benchmarks)
        * [Prerequisites](#prerequisites)
        * [Command Line](#command-line)
        * [CLion](#clion)
  * [Supporting Documents](#supporting-documents)

## Team Information

Team #2: Fast DDL

[Teju Nareddy](https://github.com/nareddyt),
 [Sai Gundlapalli](https://github.com/sgundlapalli11),
 [Grace Harper](https://github.com/graceharper)

CS 4420 Spring 2019

## General Description 

This testbed was created to understand pros and cons of DDL implementations across databases.
Specifically, we are concerned with implementing Fast DDL to add nullable columns to a row-oriented table.

We first created simplistic versions of table implementations from different databases.
See the [Supported Table Implementations](#supported-table-implementations) section for details on these prototypes.

We then ran benchmarks on each type of table, specifically to gather metrics for the DDL operation.
See the [Benchmarks](#benchmarks) section for methodology and correctness validation.

Finally, we documented our results and provided in-depth analysis.
See [test/README.md](test/README.md) for more information.

## Supported Table Implementations

We have implemented simplistic versions of the following types of tables (found in the `src\table` folder):

1. _Naive Contiguous Memory Table_ - `naive_contiguous_mem_tuple_group.h`: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples in this table are stored contiguously in memory.
2. _Naive Random Memory Table_ - `naive_random_mem_tuple_group.h`: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples within tuple groups (pages) are stored contiguously in memory, but tuple groups themselves are located on random sections of the heap. This better represents an on-disk database, so we used this table as a baseline for our Fast DDL benchmark.
3. _Aurora Table_ - `aurora_table.h`: A mock implementation of the Amazon Aurora's Fast DDL feature, where tuple groups are lazily copied on tuple access. This was the main table under test and was compared to the benchmarks from the naive table's baseline. This was based off the following [this AWS blog post](https://aws.amazon.com/blogs/database/amazon-aurora-under-the-hood-fast-ddl/).
4. _Amortized Aurora Table_ - `amortized_aurora_table.h`: An incremental change to `aurora.h`, where each full scan is limited in the number of tuple group materializations. This is our custom Fast DDL implementation, which reduces the _long tail latency problem_ and amortized the overhead of tuple group materialization across multiple full scans.

Each table consists of tuple groups, similar to pages for in-disk databases. 
There are multiple tuples to a tuple group and multiple tuple groups to a table.
These sizes are configurable via `src/includes/constants.h` and `test/testing_constants.h`.

_Note_: All table implementations run in-memory but emulate disk-based systems.

_Note_: All table implementations are row-oriented (FSM).

## Benchmarks

All benchmark code is under the `tests` folder.
`benchmarks.cpp` contains gTests that run benchmarks on each type of table implementation.

For each DDL implementation, the testbed benchmarks how fast a full scan of the table is completed, both before and after the DDL operation is run. 
Additionally, the testbed benchmarks the time for an individual tuple query (of the scan) to be completed.

### Results and Analysis

For your convenience, we have included results from running our benchmarks locally and an in-depth analysis of the results.
See [test/README.md](test/README.md) for more information.

### Running the Benchmarks

To verify our results and analysis, you may wish to run the benchmarks on your machine.

To run the benchmarks, you must execute the target `test_suite`.
See the instructions in the sub-sections below for information on executing this target.

The output should be a table of metrics for each table type.
See [test/README.md](test/README.md) for an interpretation of these results.

#### Prerequisites

- Unix-based terminal
- `cmake`
- `gcc` or `clang`

#### Command Line

Run the following commands to build the project, built the executable, and then run it.

```bash
mkdir build
cd build
cmake ..
make
cd bin
./test_suite
```

#### CLion

Alternatively, we support running the benchmarks from CLion.

Import the project using the pre-configured `CMakeLists.txt`.

Then execute `test/benchmarks.cpp` by right clicking on the file and selecting `Run all in benchmarks.cpp`.

## Supporting Documents

In addition to the documentation in this codebase, supporting documents include:

- [Project Proposal Presentation](https://docs.google.com/presentation/d/1oEdkolkFgda12XUJjeELfOa_b0pdHPLqFVg76Ybenbk/edit?usp=sharing)
- [Project Update Presentation](https://docs.google.com/presentation/d/102QaEbxZMtsjlURzfF2sE6JgidC0KcEO3V-eReXy_Ys/edit?usp=sharing)
- [Project Progress Report](https://drive.google.com/open?id=1OmBx_R3mu9gFxAckW7q-zrzkbeWWwoDy)