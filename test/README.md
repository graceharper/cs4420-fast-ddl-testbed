We run our benchmarks on three types of tables:

1. _Naive Contiguous Memory Table_: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples in this table are stored contiguously in memory.
2. _Naive Random Memory Table_: A naive MySQL table, where a DDL operation copies the entire table to a new table. All tuples within tuple groups (pages) are stored contiguously in memory, but tuple groups themselves are located on random sections of the heap. This better represents an on-disk database, so we used this table as a baseline for our Fast DDL benchmark.
3. _Aurora Table_: A mock implementation of the Amazon Aurora's Fast DDL feature, where tuple groups are lazily copied on tuple access. This was the main table under test and was compared to the benchmarks from the naive table's baseline.

We ran the following operations (in order) on each table, measuring latency as described above:

1. Create a table of 1024 integer attributes
2. Add 8 * 64 tuples to fill the table
3. Full scan to ensure all 8 * 64 tuples are persisted in memory
4. DDL: Increase the table size to store an extra 1024 integer columns
5. Full scan (multiple times) to measure latency after DDL
