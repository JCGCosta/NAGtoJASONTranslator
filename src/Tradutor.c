/* TradutorNAG_Jason Funções auxiliares em C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Tradutor.h"
int yyparse();

void yyerror(char *s, ...)
{
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

struct agent_head *create_agent(char* name)
{
	struct agent_head *newAgent = malloc(sizeof(struct agent_head));
	if (!newAgent){
		yyerror("Sem Espaco");
		exit(1);
	}
	strcpy(newAgent->name_agent, name);
	newAgent->listaCrencas = NULL;
	newAgent->listaObjetivos = NULL;
	newAgent->listaPlanos = NULL;
    printf("AGENTE <%s>\n", newAgent->name_agent);
	return newAgent;
}

void add_crenca(struct agent_head *a, char* name)
{
	struct list_node *newCrenca = malloc(sizeof(struct list_node));
	if (newCrenca)
    {
		strcpy(newCrenca->name, name);
        newCrenca->prox = a->listaCrencas;
        a->listaCrencas = newCrenca;
        printf("CRENCA <%s>\n", newCrenca->name);       
	}
    else
    {
        yyerror("Sem Espaco");
		exit(1);
    }
}

void add_objetivo(struct agent_head *a, char* name)
{
	struct list_node *newObjetivo = malloc(sizeof(struct list_node));
	if (newObjetivo)
    {
		strcpy(newObjetivo->name, name);
        newObjetivo->prox = a->listaObjetivos;
        a->listaObjetivos = newObjetivo;
        printf("OBJETIVO <%s>\n", newObjetivo->name);
	}
    else
    {
        yyerror("Sem Espaco");
		exit(1);
    }
}

void add_plano(struct agent_head *a, char* nameP, char* eg, struct expressao_log *expLog, struct corpo_list *corpo)
{
	struct list_node *newPlano = malloc(sizeof(struct list_node));
	if (newPlano)
    {
		strcpy(newPlano->name, nameP);
        strcpy(newPlano->evento_gatilho, eg);
        newPlano->expressaoCont = expLog;
        newPlano->listaCorpo = corpo;    
    
        newPlano->prox = a->listaPlanos;
        a->listaPlanos = newPlano;

        printf("PLANO <%s> :", newPlano->name);
        printf(" GATILHO -> %s", newPlano->evento_gatilho);
        printf(" | CONTEXTO -> %s %s %s", newPlano->expressaoCont->contexto1, newPlano->expressaoCont->complemento, newPlano->expressaoCont->contexto2);
        printf(" | CORPO -> ");        
        struct corpo_list *auxcorpo = newPlano->listaCorpo;        
        while (auxcorpo != NULL) 
        {
            printf("<%s> ", auxcorpo->name);
            auxcorpo = auxcorpo->prox;
        }
        printf("\n");
	}
    else
    {
        yyerror("Sem Espaco");
		exit(1);
    }
}

struct expressao_log *create_exp(char *name1, char *comp, char *name2)
{
    struct expressao_log *newExp = malloc(sizeof(struct expressao_log));
    if (newExp)
    {
        strcpy(newExp->contexto1, name1);
        strcpy(newExp->complemento, comp);
        strcpy(newExp->contexto2, name2);

        return newExp;
	}
    else
    {
        yyerror("Sem Espaco");
		exit(1);
    }
}

struct corpo_list *addCorpoList(struct corpo_list *cl, char *name)
{
    struct corpo_list *newCorpoValue = malloc(sizeof(struct corpo_list));
    if (newCorpoValue)
    {
        strcpy(newCorpoValue->name, name);
        newCorpoValue->prox = NULL;
        if (cl == NULL) return newCorpoValue;
        newCorpoValue->prox = cl;
        cl = newCorpoValue;
        return cl;
	}
    else
    {
        yyerror("Sem Espaco");
		exit(1);
    }
}

void gera_Jason(struct agent_head *a)
{
    FILE *fptr;
    struct list_node *auxCrenca = a->listaCrencas;
    struct list_node *auxObjetivos = a->listaObjetivos;
    struct list_node *auxPlanos = a->listaPlanos;
    struct corpo_list *auxcorpo; 

    char finalAddress[100] = "./";
    char termination[] = ".asl";
    strcat(finalAddress, a->name_agent);
    strcat(finalAddress, termination);

    fptr = fopen(finalAddress, "w");
    while (auxCrenca != NULL)
    {
        fprintf(fptr, "%s.\n", auxCrenca->name);
        auxCrenca = auxCrenca->prox;
    }
    fprintf(fptr, "\n");
    while (auxPlanos != NULL)
    {
        fprintf(fptr, "+%s : ", auxPlanos->evento_gatilho);
        if (!strcmp(auxPlanos->expressaoCont->complemento, "E")) fprintf(fptr, "%s & %s\n", auxPlanos->expressaoCont->contexto1, auxPlanos->expressaoCont->contexto2);
        if (!strcmp(auxPlanos->expressaoCont->complemento, "OU")) fprintf(fptr, "%s | %s\n", auxPlanos->expressaoCont->contexto1, auxPlanos->expressaoCont->contexto2);
        if (!strcmp(auxPlanos->expressaoCont->complemento, "NAO")) fprintf(fptr, "not %s\n", auxPlanos->expressaoCont->contexto2);
        if (!strcmp(auxPlanos->expressaoCont->complemento, "")) fprintf(fptr, "%s\n", auxPlanos->expressaoCont->contexto2);
        
        auxcorpo = auxPlanos->listaCorpo;
        int i = 0;
        fprintf(fptr, "  <- ");  
        while (auxcorpo != NULL)
        {
            if (i == 0) 
            {
                fprintf(fptr, "!%s", auxcorpo->name);
                i = 1;
            }
            else fprintf(fptr, "     !%s", auxcorpo->name);
            if (auxcorpo->prox != NULL) fprintf(fptr, ";\n");
            else fprintf(fptr, ".\n");
            auxcorpo = auxcorpo->prox;
        }     
        fprintf(fptr, "\n");   
        auxPlanos = auxPlanos->prox;
    }
    
    fclose(fptr);
}
