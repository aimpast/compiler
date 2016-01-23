compiler:syntax.o debug.o lexical.o main.o
	gcc -g -o compiler main.o syntax.o debug.o lexical.o

semantics:database.o semantics.o
	gcc -g -o db database.o semantics.o

main.o:main.c
	gcc -g -c main.c

syntax.o:syntax.c def.h
	gcc  -g -c syntax.c

debug.o:debug.c def.h
	gcc  -g -c debug.c

lexical.o:lexical.c def.h
	gcc  -g -c lexical.c

database.o:database.c database.h def.h
	gcc  -g -c database.c

semantics.o:semantics.c def.h
	gcc  -g -c semantics.c

lexical.c:def.h
syntax.c:def.h
debug.c:def.h

clean:
	rm compiler *.o
