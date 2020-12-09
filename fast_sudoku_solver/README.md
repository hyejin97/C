<h2> Fast Sudoku Solver Contest </h2>
This project aims to implement the efficient 9X9 sudoku solver. 

<h3> Input </h3>
There are two sets of test cases, one is mini-set and the other is large-set.
Mini-set has 100 cases. Each case consists of 9 lines of numbers(row) and 9 columns divided by a blank for every 3 columns. There is an empty line between the cases. Zero's(0) in the cases denote the blanks in sudoku problems.
<br>
Large-set has same structure with Mini-set, with 100,000 cases.
<br>
<pre>
100
074 130 805 
156 270 943 
938 065 127 
047 693 510 
523 817 604 
619 004 730 
002 750 489 
781 042 356 
095 386 201 

709 482 106 
641 090 823 
005 631 947 
402 956 008 
000 317 462 
076 240 519 
053 169 274 
920 570 681 
067 020 395 
</pre>

<h3> Output </h3>
The algorithm solves each testcase and fills the blanks(zeros) in the board.

<h3> Result </h3>
Mini-set : 0.005sec
<br>
Large-set : 0.7sec
<br>
The performance of this algorithm is computed on the following environment.
<br>
<pre>
Architecture : x86_64
Model : Intel(R) Core(TM) i5-7360U CPU @ 2.30GHz
Thread per core : 1
Core per socket : 1
Sockets : 2
L1d cache : 32K
L1i cache : 32K
L2 cache : 256K
L3 cache : 4096K
</pre>

