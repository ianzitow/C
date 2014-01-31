/*
Estruturas de Dados (2/2010)
PROJETO VI
Professor: Díbio Leandro Borges
Aluno: Ian Ferreira dos Santos - 10/0050280
*/

#include<stdlib.h>
#include<stdio.h>
#include<stdio_ext.h>
#include<ctype.h>
#define MAXN 16
#define MAXR 16

/* TAD: Nós */
typedef struct lista1 {
    char L;
    float newR, oldR;
    int n_adjs;
    struct lista2 *lis;
    struct lista1 *prox;
} Nos;

/* TAD: Adjacências */
typedef struct lista2 {
    struct lista1 *lis;
    struct lista2 *prox;
    char L;
} Adjs;

/* Protótipos das funções */
void gerador(Nos **);
void insere1(Nos **, char);
Adjs *insere2 (Adjs *, Nos *);
void interacao (Nos *);
void imprime (Nos *);
void escreve (FILE *, Nos *);
void libera(Nos *);
void pagerank(Nos *);
void bubblesort(Nos **);

/* Programa principal */
int main () {
    Nos *lis_nos = NULL;
    FILE *f;
    int i;

    /* Inicializa lista de nós */
    gerador(&lis_nos);
    /* Interage com o usuário para inserção de adjacências */
    interacao(lis_nos);
    /* Calcula o "pagerank" em MAXR rodadas */
    for(i = 0; i < MAXR; i++) pagerank(lis_nos);
    /* Reordena lista */
    bubblesort(&lis_nos);
    /* Exibe na tela o grafo e os resultados */
    imprime(lis_nos);
    /* Cria ou sobrescreve um arquivo contendo o grafo e os resultados dos cálculos */
    f = fopen("miniweb.txt", "w");
    escreve(f, lis_nos);
    fclose(f);
    /* Com tudo finalizado, a memória ocupada pelo programa é liberada */
    libera(lis_nos);

    return 0;
}

/* Funções */
/* Gera lista de nós com auxílio da função insere1() */
void gerador(Nos **lis) {
    char L = 'A';
    int i;
    for (i = 0; i < MAXN; i++) {
        insere1(lis, L);
        L++;
    }
}
void insere1(Nos **lis, char L) {
    Nos *p;
    Nos *novo = (Nos *)malloc(sizeof(Nos));
    novo->L = L;
    novo->oldR = novo->newR = 1;
    novo->n_adjs = 0;
    novo->lis = NULL;
    novo->prox = NULL;
    if ((*lis) == NULL) (*lis) = novo;
    else {
        p = (*lis);
    while(p->prox != NULL) p = p->prox;
    p->prox = novo;
    }
}
/* Insere elementos em um dado nó */
Adjs *insere2 (Adjs *lis2, Nos *lis1) {
    Adjs *novo;
    novo = (Adjs *)malloc(sizeof(Adjs));
    novo->lis = lis1;
    novo->prox = lis2;
    return novo;
}
/* Interage com o usuário para inserção de adjacências */
void interacao (Nos *lis) {
    int i = 0, *FLAG;
    char L[3*MAXN];
    Nos *p = lis, *V[MAXN];

    while (p != NULL) {
        V[i] = p;
        i++;
        p = p->prox;
    }

    printf("Bem-vindo!\n");

    p = lis;
    while (p != NULL) {
        printf("Digite adjacencias para a letra %c: ", p->L);
        __fpurge(stdin);
        fgets(L, 3*MAXN, stdin);
        FLAG = calloc(MAXN, sizeof(int));

        i = 0;
        while((L[i] != '\0') && (i < (3*MAXN))) {
            L[i] = toupper(L[i]);
            if(isalpha(L[i]) && (L[i] != (p->L)) && (FLAG[L[i]-65] != 1)) {
                    p->lis = insere2(p->lis, V[L[i]-65]);
                    p->n_adjs++;
                    FLAG[L[i]-65] = 1;
            }
            i++;
        }
        free(FLAG);
        p = p->prox;
    }
}
/* Exibe na tela o grafo e os resultados */
void imprime (Nos *lis) {
    Adjs *q;
    Nos *p;
    printf("OK, este eh o grafo:\n");
    for(p = lis; p != NULL; p = p->prox) {
        printf("%c - ", p->L);
        for (q = p->lis; q != NULL; q = q->prox) printf("%c ", q->lis->L);
        printf("(PageRank: %f)", p->newR);
        printf("\n");
    }
}
/* Escreve no arquivo o grafo e os resultados */
void escreve (FILE *f, Nos *lis) {
    Nos *p;
    Adjs *q;
    for (p = lis; p != NULL; p = p->prox) {
        fprintf(f, "%c - ", p->L);
        for (q = p->lis; q != NULL; q = q->prox) fprintf(f, "%c ", q->lis->L);
        fprintf(f, "(PageRank: %f)", p->newR);
        fprintf(f, "\n");
    }
}
/* Limpa a memória */
void libera(Nos *lis) {
    Nos *p, *t;
    Adjs *q, *u;
    p = lis;
    while (p != NULL) {
        t = p->prox;
        q = p->lis;
        while (q != NULL) {
            u = q->prox;
            free(q);
            q = u;
        }
        free(p);
        p = t;
    }
}
/* Faz "ranqueamento" dos nós */
void pagerank(Nos *lis) {
    Nos *p;
    Adjs *q;
    int i;
    for(p = lis; p != NULL; p = p->prox) {
        for(q = p->lis; q != NULL; q = q->prox) {
            q->lis->newR += 0.85*(p->oldR)/(p->n_adjs);
        }
    }
    for(p = lis; p != NULL; p = p->prox) p->oldR = p->newR;
}
/* Reordena lista em ordem decrescente de acordo com os postos obtidos */
void bubblesort(Nos **lis){
	Nos *p_ant, *t, *p;
	int flag = 1;

	while(flag == 1) {
	    flag = 0;
		p = (*lis);
		t = p->prox;
		p_ant = NULL;
		while (t != NULL) {
		    if((p->newR) < (t->newR)) {
				flag = 1;
				if(p_ant == NULL) {
				    p->prox = t->prox;
					t->prox = p;
					(*lis) = t;
					p = t;
				} else {
					p->prox = t->prox;
					t->prox = p;
					p_ant->prox = t;
					p = t;
				}
			}
			p_ant = p;
			p = p->prox;
			t = p->prox;
		}
	}
}


