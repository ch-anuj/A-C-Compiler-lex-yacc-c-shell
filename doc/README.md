TO MAKE & RUN TEST CASES
========================

## Copy and paste the testcase into the file Test.c
## Next, run the following commands.

```
    $ make
```
## A new file with name finalNew.asm is created.
## Run the file using QTSPIM.

----------------------
TO CLEAN
----------------------

```    
    $ make clean
```

--------------------------
INSTRUCTIONS IN MAKE FILE
--------------------------

```
	$ make:
	$	lex lexer.l
	$	yacc -d parser_temp.y
	$	g++ lex.yy.c y.tab.c -o out
	$	./out newTest.ir < Test.c
	$	sh shell.sh < newTest.ir > new1Test.ir
	$	g++ -o translate file.cpp
	$	./translate new1Test.ir finalNew.asm

	$ clean:
	$	rm out y.tab.c y.tab.h lex.yy.c newTest.ir new1Test.ir finalNew.asm translate

```

--------------------------
LANGUAGE FEATURES
--------------------------

*   Data Types - int only:

*   Operators:
    int:
    - Arithmetic -> +, -, /, \*, %
    - Relational -> <, >, <=, >=, ==, !=
    - Logical ops -> &&, ||
    - Assignment -> =

*   Boolean Expressions:

*   If-else:

*   Loops:
      for:
      while:
      dowhile:

*   Functions and Func Calls:

--------------------------
FEATURES NOT SUPPORTED
--------------------------

* Float and Double types.
* Post/Pre increment/decrement.
* Structs and Enums,
* Pointers.
* Comments.
* Switch Case.
* One and multidimensional arrays.
* Function Overloading.

---------
AUTHORS:
---------

1) Anuj Chauhan -- 150119
2) Vipin Chhillar -- 150805
