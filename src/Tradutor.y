%{
	/* Analisador Sintático TradutorNAG_Jason */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "Tradutor.h"
    #include "Tradutor.tab.h"
    int yylex();

    struct agent_head *agent;
    char *currentEventoGat;
    struct expressao_log *currentExp;
    struct corpo_list *currentCorpo = NULL;
%}

%union {
    char *id;
    char *op;
}

%token <id> ID
%token <op> OPERADOR
%token CRENCAS OBJETIVOS PLANOS
%token EOL

%start list_stmt
%%

/* LIDANDO COM AS CRENCAS */

nome_crenca: ID { add_crenca(agent, $1); }
    ;

lista_crencas: /* Vazio */
	| nome_crenca ';' lista_crencas 
    | '{' lista_crencas '}'  
	;

/* LIDANDO COM OS OBJETIVOS */

nome_objetivo: ID { add_objetivo(agent, $1); }
    ;

lista_objetivos: /* Vazio */
	| nome_objetivo ';' lista_objetivos 
    | '{' lista_objetivos '}'
	;

/* LIDANDO COM OS PLANOS */

evento_gatilho: ID              { currentEventoGat = $1; }
    ;

expressaoLogica: ID OPERADOR ID	{ currentExp = create_exp($1, $2, $3); }
	| OPERADOR ID               { currentExp = create_exp("", $1, $2); }  
    ;

contexto: /* Vazio */
    | ID                        { currentExp = create_exp("", "", $1); }
    | expressaoLogica	          
    ;

formula_corpo: /* Vazio */
    | ID ';' formula_corpo { currentCorpo = addCorpoList(currentCorpo, $1); }     
    ;

corpo: '{' formula_corpo '}'
    ;

nome_plano: ID '(' evento_gatilho ';' contexto ';' corpo ')' 
            { 
                add_plano(agent, $1, currentEventoGat, currentExp, currentCorpo); 
                currentCorpo = NULL;
            }
    ;

lista_planos: /* Vazio */
    | nome_plano ';' lista_planos
    | '{' lista_planos '}'	       
    ;

/* LIDANDO COM OS COMANDOS */ 

nome_agente: ID { agent = create_agent($1); }
    ;

stmt:                         
	| PLANOS '{' lista_planos '}' 
	| OBJETIVOS	'{' lista_objetivos '}'
    | nome_agente CRENCAS '{' lista_crencas '}'
	;

list_stmt: { gera_Jason(agent); }
	| stmt EOL list_stmt
	| '0'
	;

%%


