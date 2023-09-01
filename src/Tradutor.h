/* TradutorNAG_Jason Header das Funções auxiliares em C */

extern int yylineno;
void yyerror(char *s, ...);

struct expressao_log {
    char contexto1[50];
    char complemento[50];
    char contexto2[50];
};

struct corpo_list {
    char name[50];
    struct corpo_list *prox;
};

/* Listas Principais */
struct list_node {
	char name[50];
    char evento_gatilho[50];
    struct expressao_log *expressaoCont;
    struct corpo_list *listaCorpo;
	struct list_node *prox;
};

/* Cabeca Agente */
struct agent_head {
	char name_agent[50];
	struct list_node *listaCrencas;
	struct list_node *listaObjetivos;
	struct list_node *listaPlanos;
};

struct agent_head *create_agent(char *name);

void add_crenca(struct agent_head *a, char *name);
void add_objetivo(struct agent_head *a, char *name);
void add_plano(struct agent_head *a, char* nameP, char* eg, struct expressao_log *expLog, struct corpo_list *corpo);

struct expressao_log *create_exp(char *name1, char *comp, char *name2);
struct corpo_list *addCorpoList(struct corpo_list *cl, char *name);

void gera_Jason(struct agent_head *a);
