make:
	lex lexer.l
	yacc -d parser_temp.y
	g++ lex.yy.c y.tab.c -o out
	./out newTest.ir < Test.c
	sh shell.sh < newTest.ir > new1Test.ir
	g++ -o translate file.cpp
	./translate new1Test.ir finalNew.asm

clean:
	rm out y.tab.c y.tab.h lex.yy.c newTest.ir new1Test.ir finalNew.asm translate
	
