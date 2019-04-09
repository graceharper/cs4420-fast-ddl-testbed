
## TEAM
Benchmarking DDL
Teju Nareddy, Sai Gundlapalli, Grace Harper 


## General Description 
"Benchmarking framework for common and custom row-oriented DDL implementations"   

This testbed implements scaled-down versions of Naive DDL (immediate full table copy) and Amazon's Aurora Instantaneous DDL. In both instances, the DDL implemented is adding a nullable column. Both DDL implementations are run on their own makeshift "databases."

For each DDL implementation, the testbed benchmarks how fast a full scan of the table is completed, both before and after the DDL operation is run. Addtionally, the testbed benchmarks the time for an individual tuple query (of the scan) to be completed, allowing users to get a better idea of how the cost of Aurora's DDL is amortized across each tuple. 


## Database Description
All Database code is in the `src` folder.  
There is no proper database, but there are tables on which the DDL operations are run. The Naive and Aurora implementations each have separate tables associated with them. 

Each table consists of tuple groups, similar to the FSM logical tiles in class. There are multiple tuples to a tuple group and multiple tuple groups to a table. 


For the naive DDL, in order to consider both contiguous and non-contiguous memory, there are two types of naive tables:   
`naive_contiguous_mem_tuple_group.tpp` and `naive_random_mem_tuple_group.tpp`   

Aurora has its own table implementation:   
`aurora_table.tpp`

## Benchmarks
All benchmark code is under the `tests` folder.   
`tests.cpp` contains the respective implementations for the Naive and Aurora-style DDL  
These are the tests that get run in test_suite (see Setup & Running Section of README)



## Setup & Running 
To run the testbed suite, begin in the `cs4420-fast-ddl-testbed` directory and run the following: 

`mkdir build`  
`cd build`   
`cmake`  
`make`  
`cd bin`  
`./test_suite`  


