/*
PROJETO 4
Professor: Díbio Leandro Borges
Aluno 1: Ian Ferreira dos Santos - Matrícula: 10/0050280
Aluno 2: Rodrigo Trevizol Viam Gaddini - Matrícula: 10/0055079
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include<ctype.h>
#define MAX 10
#define teste perror("TESTE!");

/* Listas encadeadas */
typedef struct listaL {
        char letra;
        struct listaL *prox;
} ListaL;

typedef struct listaP {
    char *palavra;
    int numL;
    int arquivos[MAX + 1];
    int qtdearq;
    struct listaP *prox;
} ListaP;

/* Protótipos das funções */
void arq_rgstr(char [][10], int);
ListaP *ltr_lst_crt(FILE *, ListaP *, int);
void lst_liberaL(ListaL *);
ListaP *lst_criaP();
ListaL *lst_criaL();
ListaL *lst_insereL(ListaL *, char);
char *lstLvetor (ListaL *, int);
ListaP *lst_procuraP (ListaP *, char *, int);
void lst_add(ListaP *, int);
ListaP *lst_insereP(ListaP *, char *, int, int);
ListaP *lst_inserePesp(ListaP *, char *, int);
void arq_cria_IndInvert(ListaP *, FILE *);
void lst_liberaP(ListaP *);
int arq_leitura(FILE *, ListaP **) ;
int arq_busca(ListaP *, int);
int numtoprimo(int);
int calck(char *, int, int);

/* Programa principal */
int main () {
    FILE *f;
    char nome[MAX][10], plv[30];
    ListaP *LP, **hashing, *p;
    int i, qtde, primo, chave, flag;

    /* PRIMEIRA PARTE */
    /* Chama uma função para registrar os nomes dos arquivos na matriz por referência */
    arq_rgstr(nome, MAX);
    LP = lst_criaP();
    /* Abertura de arquivo, um por vez para catalogar palavras */
    for (i = 0; i < MAX; i++) {
		f = fopen(nome[i], "r");
		/* Leitura e escrita */
		if (f != NULL) LP = ltr_lst_crt(f, LP, i + 1);
		/* Fecha o arquivo processado somente leitura */
		fclose(f);
    }
    f = fopen("IndInvert.txt", "w");
    arq_cria_IndInvert(LP, f);
    fclose(f);
    lst_liberaP(LP);
    /* FIM DA PRIMEIRA PARTE */

    /* SEGUNDA PARTE */
    f = fopen("IndInvert.txt", "r");
    qtde = arq_leitura(f, &LP);
    fclose(f);
    /* FIM DA SEGUNDA PARTE */

    /* TERCEIRA PARTE */
    primo = numtoprimo(qtde);
    hashing = calloc(primo,sizeof(ListaP *));
    p = LP;
    do {
        chave = calck(p->palavra, p->numL, primo);
        LP = p->prox;
        p->prox = hashing[chave];
        hashing[chave] = p;
        p = LP;
    } while (p!=NULL);
    /* FIM DA TERCEIRA PARTE */

    /* QUARTA PARTE */
    do {
        printf("Bem-vindo! Para sair digite $\n");
        printf("Pesquisar: ");
        fflush(stdin);
        scanf("%s", plv);
        if (plv[0] != '$') {
        qtde = strlen(plv);
        for(i = 0; i < qtde; i++) {
            plv[i] = tolower(plv[i]);
        }
        chave = calck(plv, qtde, primo);
        flag = 0;
        for(p = hashing[chave]; p != NULL; p = p->prox) {
            if(!strcmp(p->palavra, plv)) {
                flag = 1;
                printf("Palavra encontrada nos arquivos:\n");
                for(i = 0; i < p->qtdearq; i++) {
                    printf("Doc%d.txt ", p->arquivos[i]);
                }
                printf("\n");
                break;
            }
        }
        if (flag == 0) printf("Palavra nao encontrada nos arquivos.\n");
        }
    } while (plv[0] != '$');

    /* FIM DA QUARTA PARTE */

    for(i = 0; i < primo; i++) {
        if (hashing[i] != NULL) lst_liberaP(hashing[i]);
    }


return 0;
}

/* Funções */
/*  Função mais complexa: faz leitura e cria lista de letras */
int calck(char *plv, int N, int primo){
    int key = 0, i;
    for (i = 0; i < N; i++) {
        key = key + (*(plv+i));
    }
    return (key % primo);
}
ListaP *ltr_lst_crt(FILE *f, ListaP *D, int i) {
     int numletras;
     char L, *plv;
     ListaP *resultado;
     ListaL *temp;

    /*  Leitura até o final */
    while (!feof(f)) {
		/*  Inicializa lista de letras */
		if(feof(f)) break;
		temp = lst_criaL();
		/*  Leitura de uma palavra */
		numletras = 0;
		L = getc(f);
		/*  Verifica se o caractere coletado é alfanumérico */
		if (isalnum(L)) {
			temp = lst_insereL(temp, L);
			do {
				L = getc(f);
				      if (isalnum(L) != 0) {
				      temp = lst_insereL(temp, L);
				      }
				      numletras++;
			} while(isalnum(L) != 0);

			if (numletras != 0) {
				/*  Converte lista de letras em vetor de caracteres */
				plv = lstLvetor(temp, numletras);
				/*  Procura na lista */
				resultado = lst_procuraP(D, plv, numletras);
				/*  Se achar, modifica contador */
				if (resultado != NULL) {
					if (arq_busca(resultado, i) != 0) lst_add(resultado, i);
				}
				/*  Se não, adiciona novo elemento */
				else D = lst_insereP(D, plv, numletras, i);
			}
			/*  Apaga lista de letras temporária */
			lst_liberaL(temp);
			/*  Libera vetor temporário de palavra */
			free(plv);
		}
	}
return D;
}
void arq_rgstr(char nome[][10], int max) {
    int i;
    /* Registrar arquivos com numeracao até 10 */
    if (max <= 9) {
    /* Inicia de 0 até 9 */
    for (i = 0; i < 9; i++) {
        nome[i][0] = 'd';
        nome[i][1] = 'o';
        nome[i][2] = 'c';
        nome[i][3] = i+49;
        nome[i][4] = '.';
        nome[i][5] = 't';
        nome[i][6] = 'x';
        nome[i][7] = 't';
        nome[i][8] = '\0';
    }
    }
    /* Registrar arquivos com numeracao maior que 10 */
    else if (max > 9) {
    /* Para um algarismo da numeração */
    for (i = 0; i < 9; i++) {
        nome[i][0] = 'd';
        nome[i][1] = 'o';
        nome[i][2] = 'c';
        nome[i][3] = i+49;
        nome[i][4] = '.';
        nome[i][5] = 't';
        nome[i][6] = 'x';
        nome[i][7] = 't';
        nome[i][8] = '\0';
    }
    /* Para dois algarismos da numeração */
    for (i = 9; i < max; i++) {
        nome[i][0] = 'd';
        nome[i][1] = 'o';
        nome[i][2] = 'c';
        nome[i][3] = ((i+1) / 10) + 48;
        nome[i][4] = ((i+1) % 10) + 48;
        nome[i][5] = '.';
        nome[i][6] = 't';
        nome[i][7] = 'x';
        nome[i][8] = 't';
        nome[i][9] = '\0';
    }
    }
}
void lst_liberaL(ListaL *lis) {
     ListaL *p;
     p = lis;
     while (p != NULL) {
           ListaL *t = p->prox;
           free(p);
           p = t;
           }
           }
ListaP *lst_criaP() {
    return NULL;
}
ListaL *lst_criaL() {
    return NULL;
}
ListaL *lst_insereL(ListaL *lis, char L) {
        ListaL *novo;
        novo = (ListaL *) malloc(sizeof(ListaL));
        novo->letra = L;
        novo->prox = lis;
        return novo;
}
char *lstLvetor (ListaL *lis, int numletras) {
     char *plvr;
     plvr = (char *) malloc((numletras+1)*sizeof(char));
     ListaL *p;
     int i = numletras - 1;

     for (p = lis; p != NULL; p = p->prox) {
     *(plvr+i) = tolower(p->letra);
     i--;
     }
     *(plvr+numletras) = '\0';
     return plvr;
}
ListaP *lst_procuraP (ListaP *lis, char *plv, int numletras) {
        ListaP *p;
        if (lis == NULL) return NULL;
        for (p = lis; p != NULL; p = p->prox) {
            if (p->numL == numletras) {
                              if (!strcmp(p->palavra, plv))
                              return p;
                              }
                              }
                              return NULL;
}
void lst_add(ListaP *p, int i) {
    p->arquivos[p->qtdearq] = i;
    p->qtdearq++;
}
ListaP *lst_insereP(ListaP *lis, char *plv, int numletras, int i) {
       ListaP *novo;
       novo = (ListaP *) malloc(sizeof(ListaP));
       novo->palavra = (char *) malloc((numletras+1)*sizeof(char));
       strcpy(novo->palavra, plv);
       novo->arquivos[0] = i;
       novo->qtdearq = 1;
       novo->numL = numletras;
       novo->prox = lis;
       return novo;
}
ListaP *lst_inserePesp(ListaP *lis, char *plv, int numletras) {
       ListaP *novo;
       novo = (ListaP *) malloc(sizeof(ListaP));
       novo->palavra = (char *) malloc((numletras+1)*sizeof(char));
       strcpy(novo->palavra, plv);
       novo->qtdearq = 0;
       novo->numL = numletras;
       novo->prox = lis;
       return novo;
}
void arq_cria_IndInvert(ListaP *LP, FILE *f) {
    int j;
    ListaP *p;
    for(p = LP; p != NULL; p = p->prox) {
        fprintf(f, "%s ", p->palavra);
        for (j = 0; j < p->qtdearq; j++) {
        fprintf(f, "%d ", p->arquivos[j]);
        }
        fprintf(f, "\n");
    }
}
void lst_liberaP(ListaP *lis) {
    ListaP *p;
    p = lis;
    while (p != NULL) {
        ListaP *t = p->prox;
        free(p->palavra);
        free(p);
        p = t;
}
}
int arq_leitura(FILE *f, ListaP **LP) {
    char L, *plv;
    int numletras, palavras = 0;
    ListaL *temp;
    ListaP *D;

    D = lst_criaP();

    while (!feof(f)) {
        if(feof(f)) break;
        numletras = 0;
        temp = lst_criaL();
        do {
            L = getc(f);
            if(isalpha(L) != 0) {
                temp = lst_insereL(temp, L);
                numletras++;
                }
        } while ((L != ' ')&& (L != EOF));
        plv = lstLvetor(temp, numletras);
        if (numletras != 0) palavras++;
        lst_liberaL(temp);

        D = lst_inserePesp(D, plv, numletras);

        do {
            L = getc(f);
            if (isdigit(L)){
                if(L=='1'){
                    L = getc(f);
                    if(L=='0'){
                        lst_add(D, 10);
                    } else lst_add(D, 1);
                } else lst_add(D, L-48);
            }
        } while ((L != '\n') && (L != EOF));
    }

    (*LP) = D;
    return palavras;
}
int arq_busca(ListaP *p, int j) {
    int i;
    for (i = 0; i < MAX + 1; i++) {
        if (j == p->arquivos[i]) return 0;
    }
    return 1;
}
int numtoprimo(int N) {
    int i, prim=0, flag;
    do{
        flag=0;
        for(i=2;i<=sqrt(N);i++){
            if((N%i) == 0)flag++;
            if(flag>1) break;
        }
        if(flag==1)prim=N;
        else N++;
    } while (prim==0);
    return prim;
}
