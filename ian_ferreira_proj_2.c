/*
PROJETO 2
ALUNO: Ian Ferreira dos Santos
MATRÍCULA: 10/0050280
PROFESSOR: Díbio Leandro Borges
*/

#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define teste perror("ERRO");

/* Structs de listas e filas */
typedef struct lista {
    char letra1, letra2;
    int num1, num2, num3, num4;
    char tipo;
    int prioridade;
    struct lista *prox;
} Lista, No;

typedef struct fila {
    No *ini;
    No *fim;
} Pista;

/* Protótipos das funções */
char bancoletras (int n);
int gerador (void);
char vootipo(int n);
Lista *lst_cria();
Lista *lst_novo(Lista *lis, char L);
void lst_imprime(Lista *lis);
Lista *org_lst(Lista *lis);
Lista *lst_insere (Lista *p, Lista *lis);
Pista *fila_cria();
No* ins_fim(No* fim, Lista* v);
void insere_fila(Pista* f, Lista *v);
void imprime_fila (Pista *f);
void libera_fila (Pista *f);
void libera_lst (Lista *lis);

/* Programa principal */
int main () {
    int NVoos, NPousos, NDecolagens, i, dtr, j, contador, aux;
    Lista *voos, *pl;
    Pista *p1, *p2, *p3;
    /* Para mostrar hora local */
    time_t tm;
    struct tm *ptr;
    char str[60];

    /* Semente para a função gerador() */
    srand(time(NULL));

    /* Define a quantidade de voos numa unidade de tempo */
    do{
        NPousos = gerador() % 65;
        NDecolagens = gerador() % 65;
        NVoos = NPousos + NDecolagens;
    } while (NVoos > 64);

    /* Inicializa a lista de voos */
    voos = lst_cria();
    /* Inicializa as filas de voos */
    p1 = fila_cria();
    p2 = fila_cria();
    p3 = fila_cria();

    /* Insere elementos na lista de pedidos aleatoriamente    
    for(i = 0; i < NPousos; i++) voos = lst_novo(voos, 'P');
    for(j = 0; j < NDecolagens; j++) voos = lst_novo(voos, 'D'); */
    i = j = 0;
    do{
    aux = gerador() % 2;
    switch (aux) {
          case 0: 
               if (i < NPousos) { 
               voos = lst_novo(voos, 'P');
               i++; 
               }
          break;
          case 1: 
               if (j < NVoos) { 
               voos = lst_novo(voos, 'D');
               j++; 
               }
          break;
     }
     } while ((i + j) != NVoos);

    /* Coloca P0 na frente */
    voos = org_lst(voos);

    /* Distribui voos */
    /* Variável que garante a distribuição de P0 para cada fila */
    contador = 0;
    for(pl = voos; pl != NULL; pl = pl->prox) {
        if (pl->prioridade == 0) {
        contador = contador % 3;
        switch (contador) {
            case 0: insere_fila(p1, pl);
            contador++;
            break;
            case 1: insere_fila(p2, pl);
            contador++;
            break;
            case 2: insere_fila(p3, pl);
            contador++;
            break;
        }
        } else if (pl->prioridade != 0) {
            /* A Pista 3 aceita apenas decolagens ou P0 */
            if (pl->tipo == 'D') dtr = gerador() % 3;
            /* Caso não seja uma decolagem, distribui para Pistas 1 e 2 */
            else dtr = gerador() % 2;
            switch (dtr) {
            case 0: insere_fila(p1, pl);
            break;
            case 1: insere_fila(p2, pl);
            break;
            case 2: insere_fila(p3, pl);
            break;
        }
        }
    }

    printf("Aeroporto Internacional de Brasilia\n");
    tm = time(NULL);
    ptr = localtime(&tm);
    printf("Hora atual: ");
    strftime(str, 100, "%X \n", ptr);
    printf("%s\n", str);
    printf("Fila de pedidos:\n");
    lst_imprime(voos);
    printf("NVoos: %d voos\n", NVoos);
    printf("NPousos: %d voos\n", NPousos);
    printf("NDecolagens: %d voos\n", NDecolagens);
    printf("\nPista 1:\n");
    imprime_fila(p1);
    printf("\nPista 2:\n");
    imprime_fila(p2);
    printf("\nPista 3:\n");
    imprime_fila(p3);
    printf("\n");
    /* Para liberar a memória ocupada pelas filas e listas */
    libera_fila(p1);
    libera_fila(p2);
    libera_fila(p3);
    libera_lst(voos);

return 0;
}

/* Funções */
/* Banco de letras para o sorteio */
char bancoletras (int n) {
    char alfabeto[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    return alfabeto[n];
}
/* Tipos de voo para sorteio */
char vootipo(int n) {
    char voo[2] = {'P', 'D'};
    return voo[n];
}
/* Gerador de números aleatórios */
int gerador (void){
    return rand();
}
/* Inicializadora de lista */
Lista *lst_cria() {
    return NULL;
}
/* Cria um novo elemento da lista de pedidos */
Lista *lst_novo(Lista *lis, char L) {
    Lista *novo = (Lista *) malloc(sizeof(Lista));
    novo->letra1 = bancoletras(gerador() % 25);
    novo->letra2 = bancoletras(gerador() % 25);
    novo->num1 = gerador() % 10;
    novo->num2 = gerador() % 10;
    novo->num3 = gerador() % 10;
    novo->num4 = gerador() % 10;
    novo->tipo = L;
    if (novo->tipo == 'P') novo->prioridade = gerador() % 13;
    else if (novo->tipo == 'D') novo->prioridade = (gerador() % 25) + 24;
    novo->prox = lis;
    return novo;
}

/* Função para imprimir uma lista */
void lst_imprime(Lista *lis) {
    Lista *p;
    for(p = lis; p != NULL; p = p->prox) {
        printf("%c", p->letra1);
        printf("%c", p->letra2);
        printf("%d", p->num1);
        printf("%d", p->num2);
        printf("%d", p->num3);
        printf("%d", p->num4);
        printf(" %c%d\t", p->tipo, p->prioridade);
    }
    printf("\n");
}

/* Função para colocar P0 no início da lista */
Lista *org_lst(Lista *lis) {
    Lista *p, *ant = NULL;
    for(p = lis; p != NULL; p = p->prox) {
        if ((p->tipo == 'P' )&&(p->prioridade == 0)) {
            if (ant != NULL) ant->prox = p->prox;
            p->prox = lis;
            lis = p;
            if (ant != NULL) p = ant;
        }
        ant = p;
    }
    return lis;
}

/* Função para criar um novo elemento nas listas de filas */
Lista *lst_insere (Lista *p, Lista *lis) {
    Lista *novo = (Lista *) malloc(sizeof(Lista));
    novo->letra1 = p->letra1;
    novo->letra2 = p->letra2;
    novo->num1 = p->num1;
    novo->num2 = p->num2;
    novo->num3 = p->num3;
    novo->num4 = p->num4;
    novo->tipo = p->tipo;
    novo->prioridade = p->prioridade;
    novo->prox = lis;
    return novo;
}

/* Inicializa a fila */
Pista *fila_cria() {
    Pista *f = (Pista*) malloc(sizeof(Pista));
    f->ini = f->fim = NULL;
    return f;
}

/* Função para inserir elementos no final da fila */
void insere_fila(Pista* f, Lista *v) {
    f->fim = ins_fim(f->fim, v);
    if (f->ini == NULL) f->ini = f->fim;
}

/* Função auxiliar para inserir elementos de fila */
No* ins_fim(No* fim, Lista* v) {
    No *p = (No *) malloc(sizeof(No));
    p->letra1 = v->letra1;
    p->letra2 = v->letra2;
    p->num1 = v->num1;
    p->num2 = v->num2;
    p->num3 = v->num3;
    p->num4 = v->num4;
    p->tipo = v->tipo;
    p->prioridade = v->prioridade;
    p->prox = NULL;
    if (fim != NULL) fim->prox = p;
    return p;
}

/* Função para imprimir uma fila */
void imprime_fila (Pista *f) {
    No *p;
    for(p = f->ini; p != NULL; p = p->prox) {
        printf("%c", p->letra1);
        printf("%c", p->letra2);
        printf("%d", p->num1);
        printf("%d", p->num2);
        printf("%d", p->num3);
        printf("%d", p->num4);
        printf(" %c%d\t", p->tipo, p->prioridade);
}
}

/* Função para liberar memória ocupada pelas filas */
void libera_fila (Pista *f) {
    No *q = f->ini;
    while (q != NULL) {
        No *t = q->prox;
        free(q);
        q = t;
}
free(f);
}

/* Função para liberar memória ocupada pelas listas */
void libera_lst (Lista *lis) {
    Lista *p = lis;
    while (p != NULL) {
        Lista *t = p->prox;
        free(p);
        p = t;
}
}
