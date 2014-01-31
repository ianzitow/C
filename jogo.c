/* PROJETO 3 - Estruturas de Dados - Turma E - 2/2010 */
/* Professor: Díbio Leandro Borges */
/* Aluno: Ian Ferreira dos Santos - 10/0050280 */

#include<stdlib.h>
#include<stdio.h>
#include<time.h>

/* Protótipos */
typedef struct arvgen {
    char tabuleiro[3][3];
    int jogada;
    struct arvgen *prox;
    struct arvgen *filho;
} ArvGen;

int quemcomeca();
void arv_libera(ArvGen *a);
int define_dif();
void exibir_tab(char tab[3][3]);
void tab_cria(char tab[3][3]);
void jogador_inicia(char tab[3][3]);
void comp_inicia(char tab[3][3]);
ArvGen *arv_build(char tab[3][3], int nivelJog);
void arv_expande(ArvGen *p, int pNivel, int profund);
ArvGen *gerarListaNohs (char tab[3][3], int jogada);
ArvGen *arv_novonoh (ArvGen *old, char tab[3][3], int i, int j, int jogada);
int avaliarPosicao (char tab[3][3], char jogador);
void caminhoMiniMax (ArvGen *pnd, char jogador, ArvGen *pMelhor, int *pValor);
void jogador_joga (char tab[3][3]);

/* Programa principal */
int main () {
    int QC, l, c;
    ArvGen *AG;
    char tab[3][3];
    int i, j, nivelJog, turno, *ganho = (int *)malloc(sizeof(int));

    /* Semente para geradores aleatorios */
    srand(time(NULL));

    /* Interação */
    printf("-------------------| JOGO DA VELHA |-------------------\n");
    printf("Bem-vindo! Por favor, escolha um nivel de dificuldade: ");
    /* Nível de dificuldade do jogo */
    nivelJog = define_dif();
    /* Quem começa? */
    QC = quemcomeca();
    if (QC == 0) printf("STATUS: Voce comeca!\n");
    else printf("STATUS: Computador comeca!\n");

    /* Inicia o tabuleiro */
    tab_cria(tab);

    /* Inicia o jogo */
    switch(QC) {
        case 0:
        jogador_inicia(tab);
        break;
        case 1:
        comp_inicia(tab);
        break;
    }

    /* Exibe o tabuleiro atual */
    exibir_tab(tab);

    /* Constroi arvore */
    AG = arv_build (tab, nivelJog);

    /* Game Play */
    if (QC == 0) turno = 1;
    else turno = 0;
    do{
        if (turno == 0) {
            printf("STATUS: Eh a sua vez!\n");
            jogador_joga(AG->tabuleiro);
            turno = 1;
        } else {
            *ganho = avaliarPosicao(AG->tabuleiro, 'X');
            printf("STATUS: Vez do computador!\n");
            caminhoMiniMax(AG, 'X', AG->filho, ganho);
            printf("STATUS: Ganho de uso: %d\n", *ganho);
            turno = 0;
        }
        exibir_tab(AG->tabuleiro);
    } while(detecta_vitoria(AG->tabuleiro) != 1);

    arv_libera(AG);

return 0;
}

/* TAD: Arvore de Decisão */
void arv_libera(ArvGen *a) {
    ArvGen *p = a->filho, *t;
    while(p != NULL) {
        t = p->prox;
        arv_libera(p);
        p = t;
    }
    free(a);
}

/* Funções */
int quemcomeca() {
    return rand() % 2;
}
ArvGen *arv_build(char tab[3][3], int nivelJog) {
    int i,j;
    ArvGen *pArvore = (ArvGen *) malloc(sizeof(ArvGen));
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            pArvore->tabuleiro[i][j] = tab[i][j];
        }
    }
    pArvore->jogada = 1;
    pArvore->filho = NULL;
    pArvore->prox = NULL;
    arv_expande(pArvore, 0, nivelJog);
    printf("STATUS: Arvore criada. Tudo pronto!\n");
    return pArvore;
}
void arv_expande(ArvGen *p, int pNivel, int profund) {
    ArvGen *q;
    if (pNivel < profund) {
        q = gerarListaNohs (p->tabuleiro, p->jogada);
        p->filho = q;
        while (q != NULL) {
            if (p->jogada == 1) q->jogada= -1;
            else q->jogada = 1;
            q->filho = NULL;
            arv_expande(q, pNivel + 1, profund);
            q = q->prox;
            }
    }
}
ArvGen *gerarListaNohs (char tab[3][3], int jogada) {
    int i, j;
    ArvGen *p, *a = NULL;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (tab[i][j] == ' ') {
            a = arv_novonoh(a, tab, i, j, jogada);
            }
        }
    }
    return a;
}
ArvGen *arv_novonoh (ArvGen *old, char tab[3][3], int i, int j, int jogada) {
    ArvGen *a = (ArvGen *)malloc(sizeof(ArvGen));
    int l, c;
    for (l = 0; l < 3; l++) {
        for (c = 0; c < 3; c++) {
            a->tabuleiro[l][c] = tab[l][c];
        }
    }
    switch (jogada) {
                    case 1: a->tabuleiro[i][j] = 'O';
                    break;
                    case -1: a->tabuleiro[i][j] = 'X';
                    break;
                }
    a->filho = NULL;
    a->prox = old;
    a->jogada = jogada;
    return a;
}
int define_dif() {
    int n;
    scanf("%d", &n);
    return n;
}
void exibir_tab(char tab[3][3]) {
    int i, j, contador = 0;
    char *position = (char *) malloc(9*sizeof(char));
    for (i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            *(position+contador) = tab[i][j];
            contador++;
        }
    }
    printf("%c|%c|%c\n", *(position+0), *(position+1), *(position+2));
    printf("-----\n");
    printf("%c|%c|%c\n", *(position+3), *(position+4), *(position+5));
    printf("-----\n");
    printf("%c|%c|%c\n", *(position+6), *(position+7), *(position+8));
    free(position);
}
void tab_cria(char tab[3][3]) {
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            tab[i][j] = ' ';
        }
    }
}
void jogador_inicia(char tab[3][3]) {
    int l, c;
        printf("Escolha uma linha (0 a 2): ");
        scanf("%d", &l);
        printf("Escolha uma coluna (0 a 2): ");
        scanf("%d", &c);
        tab[l][c] = 'O';
}
void comp_inicia(char tab[3][3]) {
    /* Maior probabilidade de vitória */
    tab[1][1] = 'X';
}
int avaliarPosicao (char tab[3][3], char jogador) {
    int i, numX = 0, numO = 0;

    /* Verifica marcações e espaços disponíveis para a mesma */
    for (i = 0; i < 3; i++) {
        /* Verifica linhas */
        if ((tab[i][0] == tab[i][1])&&(tab[i][2] == tab[i][0])) {
            numO++;
            numX++;
        }
        else if ((tab[i][0] != ' ') || (tab[i][1] != ' ') || (tab[i][2] != ' ')) {
            if ((tab[i][0] == 'X') || (tab[i][1] == 'X') || (tab[i][2] == 'X')) numX++;
            else if ((tab[i][0] == 'O') || (tab[i][1] == 'O') || (tab[i][2] == 'O')) numO++;
        }
        /* Verifica colunas */
        if ((tab[0][i] == tab[1][i])&&(tab[2][i] == tab[0][i])) {
            numO++;
            numX++;
        }
        else if ((tab[0][i] != ' ') || (tab[1][i] != ' ') || (tab[2][i] != ' ')) {
            if ((tab[0][i] == 'X') || (tab[1][i] == 'X') || (tab[2][i] == 'X')) numX++;
            else if ((tab[0][i] == 'O') || (tab[1][i] == 'O') || (tab[2][i] == 'O')) numO++;
        }
    }
    /* Verifica diagonais */
    if ((tab[0][0] == tab[1][1])&&(tab[2][2] == tab[0][0])) {
        numO++;
        numX++;
    }
    else if ((tab[0][0] != ' ') || (tab[1][1] != ' ') || (tab[2][2] != ' ')) {
        if ((tab[0][0] == 'X') || (tab[1][1] == 'X') || (tab[2][2] == 'X')) numX++;
        else if ((tab[0][0] == 'O') || (tab[1][1] == 'O') || (tab[2][2] == 'O')) numO++;
    }
    if ((tab[0][2] == tab[1][1])&&(tab[2][0] == tab[1][1])) {
        numO++;
        numX++;
    }
    else if ((tab[0][2] != ' ') || (tab[1][1] != ' ') || (tab[2][0] != ' ')) {
        if ((tab[0][2] == 'X') || (tab[1][1] == 'X') || (tab[2][0] == 'X')) numX++;
        else if ((tab[0][2] == 'O') || (tab[1][1] == 'O') || (tab[2][0] == 'O')) numO++;
    }
    if (jogador == 'X') return (numX - numO);
    else return (numO - numX);
}
void caminhoMiniMax (ArvGen *pnd, char jogador, ArvGen *pMelhor, int *pValor) {
    ArvGen *p, pMelhor2;
    int val;
    if (pnd->filho == NULL) {
        if(*pValor = avaliarPosicao (pnd->tabuleiro, jogador)) pMelhor = pnd;
    } else {
        p = pnd->filho;
        caminhoMiniMax (p, jogador, pMelhor, pValor);
        pMelhor = p;
        if (pnd->jogada == -1)
        *pValor = -(*pValor);
        p = p->prox;

        while (p != NULL) {
            caminhoMiniMax(p, jogador, &pMelhor2, &val);
            if (pnd->jogada == -1) val = -val;
            if (val > *pValor) {
                *pValor = val;
                pMelhor = p;
            }
            p = p->prox;
        }
            if (pnd->jogada == -1)
            *pValor = -(*pValor);
}
}
void jogador_joga (char tab[3][3]) {
    int l, c;
    printf("Escolha uma linha (0 a 2): ");
    scanf("%d", &l);
    printf("Escolha uma coluna (0 a 2): ");
    scanf("%d", &c);
    while (tab[l][c] != ' ') {
        printf("STATUS: Jogada invalida! Espaco ja ocupado! Tente novamente.\n");
        printf("Escolha uma linha (0 a 2): ");
        scanf("%d", &l);
        printf("Escolha uma coluna (0 a 2): ");
        scanf("%d", &c);
    }
    tab[l][c] = 'O';
}
int detecta_vitoria(char tab[3][3]) {
    int l, c, contador = 0;
            /* Verifica linhas */
            for (l = 0; l < 3; l++) {
                if ((tab[l][0] == 'X')&&(tab[l][1] == 'X')&&(tab[l][2]  == 'X')) {printf("STATUS: Computador te venceu! HAHAHA\n"); return 1; }
                if ((tab[l][0] == 'O')&&(tab[l][1] == 'O')&&(tab[l][2]  == 'O')) {printf("STATUS: Parabens! Voce venceu!\n"); return 1; }
            }
            /* Verifica colunas */
            for (c = 0; c < 3; c++) {
                if ((tab[0][c] == 'X')&&(tab[1][c] == 'X')&&(tab[2][c]  == 'X')) {printf("STATUS: Computador te venceu! HAHAHA\n"); return 1; }
                if ((tab[0][c] == 'O')&&(tab[1][c] == 'O')&&(tab[2][c]  == 'O')) {printf("STATUS: Parabens! Voce venceu!\n"); return 1; }
            }
            /* Verifica diagonais */
            if ((tab[0][0] == 'X')&&(tab[1][1] == 'X')&&(tab[2][2]  == 'X')) {printf("STATUS: Computador te venceu! HAHAHA\n"); return 1; }
            if ((tab[0][0] == 'O')&&(tab[1][1] == 'O')&&(tab[2][2]  == 'O')) {printf("STATUS: Parabens! Voce venceu!\n"); return 1; }
            if ((tab[0][2] == 'X')&&(tab[1][1] == 'X')&&(tab[2][0]  == 'X')) {printf("STATUS: Computador te venceu! HAHAHA\n"); return 1; }
            if ((tab[0][2] == 'O')&&(tab[1][1] == 'O')&&(tab[2][0]  == 'O')) {printf("STATUS: Parabens! Voce venceu!\n"); return 1; }
            /* Verifica empate */
            for (l = 0; l < 3; l++) {
                for (c = 0; c < 3; c++) {
                    if (tab[l][c] == ' ') contador++;
                }
            }
            if (contador == 0) printf("STATUS: Empate! Jogo encerrado.\n");
            /* Solidario return para jogo continuar */
            return 0;
}
