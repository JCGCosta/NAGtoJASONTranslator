all: Tradutor

Tradutor.tab.c Tradutor.tab.h: Tradutor.y
	bison -t -v -d Tradutor.y

lex.yy.c: Tradutor.l Tradutor.tab.h
	flex Tradutor.l

Tradutor: lex.yy.c Tradutor.tab.c Tradutor.tab.h
	gcc Tradutor.h Tradutor.c -o Tradutor Tradutor.tab.c lex.yy.c

clean:
	rm Tradutor Tradutor.tab.h Tradutor.tab.c lex.yy.c Tradutor.output
