<h2> Page Replacement Algorithms </h2>
Design and implement a program that simulates several page replacement algorithms.
Given the sequence of page numbers range from 0 to 999 measure the number of page faults incurred in each algorithm and compare their performance.

Implemented 4 page replacement algorithms;
1. Optimal
2. FIFO
3. LRU
4. Clock

<h3> Input </h3>
Sequence of workloads in the format "R/W + integer in range[0, 999]"

<pre>
R 877
W 771
R 976
R 778
W 19
W 82
R 765
W 28
R 570
W 52
W 626
W 835
W 248
R 141
W 751
W 20
R 248
W 88
W 228
</pre>

<h3> Output </h3>
The cache size used for the algorithm
The number of cache miss
Elapsed time
Dirtycount

for each page replacement algorithm.

<h3> How to compile and run the code </h3>
<pre>
1. put "make" on the command line <br>
2. create the input test file(workload) <br>
3. run each file optimal_dirty, fifo_dirty, lru_dirty, clock_dirty with inputfile name and cachesize. (ex. ./optimal_dirty workload 100) <br>
</pre>
<h3> Implementation Environment </h3>
Platform : Ubuntu 16.04
Language : C

