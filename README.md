
## TEAM
Benchmarking DDL
Teju Nareddy, Sai Gundlapalli, Grace Harper 


## Description 
"Benchmarking framework for common and custom row-oriented DDL implementations"   

This testbed implements scaled-down versions of Naive DDL (immediate full table copy) and Amazon's Aurora Instantaneous DDL. In both instances, the DDL implemented is adding a nullable column. Both are run on their own makeshift "databases."

For each DDL implementation, the testbed benchmarks how fast a full scan of the table is completed, both before and after the DDL operation is run. Addtionally, the testbed benchmarks at the tuple level, allowing users to get a better idea of how the cost of Aurora is amortized across tuples. 


## DDL Themselves
There is no proper database, but there are tables on which the DDL operations are run. The Naive and Aurora implementations each have separate tables associated with them. 

Each table consists of tuple groups, similar to the FSM logical tiles in class. There are multiple tuples to a tuple group and multiple tuple groups to a table. 


For the naive DDL, in order to consider both contiguous and non-contiguous memory, there are two types of naive tables: 
naive_contiguous_mem_tuple_group.tpp, naive_random_mem_tuple_group.tpp.   

Aurora has its own table implementation: aurora_table.tpp.



## Setup & Running 
To run the testbed suite, begin in the `cs4420-fast-ddl-testbed` directory and run the following: 

`mkdir build`  
`cd build`   
`cmake`  
`make`  
`cd bin`  
`./test_suite`  


