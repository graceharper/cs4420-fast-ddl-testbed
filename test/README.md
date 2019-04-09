## Benchmark Operations

We ran the following operations (in order) on each table, measuring latency as described above:

1. Create a table of 1024 integer attributes
2. Add 8 * 64 tuples to fill the table
3. Full scan to ensure all 8 * 64 tuples are persisted in memory
4. DDL: Increase the table size to store an extra 1024 integer columns
5. Full scan (multiple times) to measure latency after DDL

## Benchmark Results

This figure compares the Naive vs Aurora operation times:

![Table Operations Comparison](../images/benchmarks-operations-metrics.png "Table Operations Comparison")

This figure shows the large variance in individual tuple access times in full scan directly after a Fast DDL in the Aurora table:

![Aurora Tuple Access Times](../images/benchmarks-subsequent-scan-metrics.png "Aurora Tuple Access Times")

## Analysis

Aurora's fast DDL implementation does increase the DDL transaction time, but results in:

- a 3x increase in the next full scan
- up to a 75x delay in individual tuple access times within that full scan

This is better known as a **long tail latency** problem.
If not handled correctly, this could lead to **tail latency amplification** in upstream services.