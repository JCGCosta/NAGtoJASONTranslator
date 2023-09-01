%{ 
	/* Analisador Léxico TradutorNAG_Jason */
    #include "Tradutor.h"
    #include "Tradutor.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
%}

%option yylineno
%option noyywrap

LETTER [a-zA-Z_]
DIGIT [0-9]
ID {LETTER}({LETTER}|{DIGIT})*

%%



"crencas:" 	                    { return CRENCAS; }
"objetivos:"	                { return OBJETIVOS; }
"planos:"	                    { return PLANOS; }
"E"|"OU"|"NAO"                  { yylval.op = strdup(yytext); return OPERADOR; }
"("|")"|"{"|"}"|";"             { return yytext[0]; }
{ID}		                    { yylval.id = strdup(yytext); return ID; }
"\n"	                        { return EOL; }
<<EOF>>                         { return 0; }
[ \t]		                    ;
.			                    { printf("Caracter desconhecido: %s\n", yytext); return 0; }

%%

/* definido pelo analisador léxico */
extern FILE * yyin;  

int main(int argc, char ** argv)
{
	/* se foi passado um nome de arquivo */
	if (argc > 1)
	{
		FILE * file;
		file = fopen(argv[1], "r");
		if (!file)
		{
			exit(1);
		}
		
		/* entrada ajustada para ler do arquivo */
		yyin = file;
	}
    else
    {
        printf("Nenhum arquivo de entrada encontrado.");
        exit(1);
    }

	yyparse();
}
