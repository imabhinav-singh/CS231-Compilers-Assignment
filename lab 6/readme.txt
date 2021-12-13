Please follow the following instructions to compile and run the code:- 
1) g++ parser.cc -o parse.o
2) flex lexanalyzer.l
3) gcc lex.yy.c -ll -o analyze.o
4) ./analyze.o > input.txt
   The above command will require you to enter your desired string. Press Ctrl+D after you finish.
5) ./parse.o grammar.txt input.txt
    
Here, grammar.txt file contains the grammar in the following format:- 
<number of terminals in your grammar> <number of non-terminals n your grammar> <number of productions>
<a space separated list of terminals>
<a space separated list of non-terminals>
<start symbol>
Following lines will contain productions where each production will be on a new line.

For writing productions, keep the following points in mind:- 
1) epsilon is recognised as epsilon
2) If your grammar has epsilon, then count it as a terminal and write "epsilon" wherever it appears.
3) When entering productions, give spaces between each variable
4) Also, all productions having same lhs will be on the same line with rhs(s) separated by |

For, example:-
E : E + T | E - T | T
T : T * F | T / F | F
F : id | num | ( E )
The above grammar has 8 terminals, 3 non-terminals, and 3 productions

2 1 1
0 1
B
B
B : B 0 $2*B.val | B 1 $2*B.val+1 | 1 $1
