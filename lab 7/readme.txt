To run part 1 (to generate 3-address code) :-
1) yacc -d yacc.y
2) flex.l
3) gcc lex.yy.c y.tab.c
4) ./a.out

To run part 2 (to generate DAG from 3-address code) :-
1) Write your expression grammar input in input.txt 
2) yacc -d yacc.y
3) flex.l
4) gcc lex.yy.c y.tab.c
5) ./a.out < input.txt > output.txt
6) python2.7 dag.py