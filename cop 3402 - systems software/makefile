all: clean compile

compile: compile.o pl0-lex.o pl0-parser.o pl0-vm.o
	 gcc -o compile pl0-lex.o pl0-parser.o pl0-vm.o compile.o

compile.o: 
	gcc -c compile.c

pl0-lex.o: tokens.h pl0-lex.h
	   gcc -c pl0-lex.c


pl0-parser.o: tokens.h pl0-parser.h
	      gcc -c pl0-parser.c

pl0-vm.o: pl0-vm.h
	  gcc -c pl0-vm.c


clean:
	rm -rf *.o compile