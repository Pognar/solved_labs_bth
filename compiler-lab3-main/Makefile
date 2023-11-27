compiler: lex.yy.c parser.tab.o main.cc
		g++ -g -w -ocompiler parser.tab.o lex.yy.c main.cc -std=c++14
parser.tab.o: parser.tab.cc
		g++ -g -w -c parser.tab.cc -std=c++14
parser.tab.cc: parser.yy
		bison -v parser.yy
lex.yy.c: lexer.flex parser.tab.cc
		flex lexer.flex
tree: 
	dot -Tpdf tree.dot -otree.pdf

cfg: 
	dot -Tpdf cfg.dot -ocfg.pdf

clean:
	rm -f parser.tab.* lex.yy.c* compiler stack.hh tree.dot tree.pdf cfg.dot cfg.pdf run_interpreter

test : 
	make clean
	make compiler
	./compiler < a3_testfiles/a3valid/A.java
	make tree
	make cfg
	g++ -g -w -o run_interpreter interpreter.cc -std=c++14 && ./run_interpreter

curly : 
	make clean
	make compiler
	./compiler < a3_testfiles/validcurly/binarysearch.java
	make tree
	make cfg
	g++ -g -w -o run_interpreter interpreter.cc -std=c++14 && ./run_interpreter


all : 
	make clean
	make compiler
	g++ -g -w -o run_interpreter interpreter.cc -std=c++14
	echo A.java; ./compiler < a3_testfiles/a3valid/A.java; ./run_interpreter
	echo B.java; ./compiler < a3_testfiles/a3valid/B.java; ./run_interpreter
	echo C1.java; ./compiler < a3_testfiles/a3valid/C1.java; ./run_interpreter
	echo C2.java; ./compiler < a3_testfiles/a3valid/C2.java; ./run_interpreter
	echo D1.java; ./compiler < a3_testfiles/a3valid/D1.java; ./run_interpreter
	echo D2.java; ./compiler < a3_testfiles/a3valid/D2.java; ./run_interpreter
	echo D3.java; ./compiler < a3_testfiles/a3valid/D3.java; ./run_interpreter
	echo E.java; ./compiler < a3_testfiles/a3valid/E.java; ./run_interpreter
	make tree
	make cfg