/*
PROJETO 1
Professor: Díbio Leandro Borges
Aluno: Ian Ferreira dos Santos - Matrícula: 10/0050280
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define PI 3.14159265
#define teste perror("ERROR!");

/*  Lista para armazenar elementos de letra de uma palavra */
typedef struct listaletras {
        char letra;
        struct listaletras *prox;
        } Lista_L;

/*  Listas de palavras e suas frequências */      
typedef struct lista {
        char *palavra;
        int contador;
        int letras; 
        struct lista *prox;
        struct lista *ant;
        } Lista_P;
        
/*  Protótipos das funções */
void lst_imprime(Lista_L *lis);
void lst_libera(Lista_L *lis);
char *lstLvetor(Lista_L *lis, int numletras);
void lst_aumentacP(Lista_P *lis);
Lista_P *ltr_lst_crt(FILE *f, Lista_P *D);
void organizador(Lista_P *lis);
void escreve(Lista_P *lis, FILE *f);
void escreve_res(int n, float pct, FILE *f);
void lst_imprimeP(Lista_P *lis);
int analisa_res(float n);
float perccomp(float n);
float calcangulo(Lista_P *lis1, Lista_P *lis2, int d1, int d2);
Lista_P *lst_insereP(Lista_P *lis, char *plv, int numletras);
Lista_L *lst_insere(Lista_L *lis, char L);
Lista_L *lst_criaL();
Lista_P *lst_criaP();
Lista_P *lst_procuraP (Lista_P *lis, char *plv, int numletras);
int contapalavras(Lista_P *lis);
void lst_liberaP(Lista_P *lis);
void limpamemP(Lista_P *lis);

/*  Programa principal */
int main()
{
    /*  Declarações */
    FILE *f1, *f2, *f3;
    Lista_P *D1, *D2, *D1nova, *D2nova;
    int d1, d2, plg;
    float resplg, percnt;
    
    /*  Abertura de arquivos */
    f1 = fopen("doc1.txt", "r");
    if (f1 == NULL) {
           printf("FALHA ao abrir doc1.txt\n");
           exit(1);
           }
    f2 = fopen("doc2.txt", "r");
    if (f2 == NULL) {
           printf("FALHA ao abrir doc2.txt\n");
           exit(2);
           }

    /*  Inicializa listas de palavras */
    D1 = lst_criaP();
    D2 = lst_criaP();
    
    /*  Leitura e criação das listas (Função mais complexa) */
    D1 = ltr_lst_crt(f1, D1);
    D2 = ltr_lst_crt(f2, D2);
    
    /*  Organiza lista de palavras em ordem decrescente de frequência */
    organizador(D1);
    organizador(D2);
    
    /*  Obtém somatório dos quadrados das frequências de palavras */
    d1 = contapalavras(D1);
    d2 = contapalavras(D2);
    
    /*  Calcula o angulo */
    resplg = calcangulo(D1, D2, d1, d2);
    
    /*  Cria arquivo do resultado */
    f3 = fopen("compararDoc.txt", "w");

    /*  Verifica se houve plágio */
    plg = analisa_res(resplg);
    percnt = perccomp(resplg);
    
    /*  Escreve no arquivo as listas e suas frequências */
    fprintf(f3, "Este arquivo é a análise dos textos em doc1.txt e doc2.txt\n\nLISTA 1:\n");
    escreve(D1, f3);
    fprintf(f3, "\nLISTA 2:\n");
    escreve(D2, f3);
    fprintf(f3, "\nRESULTADO:\n");
    escreve_res(plg, percnt, f3);
    
    /*  Fechamento de arquivos */
    fclose(f1);
    fclose(f2);
    fclose(f3);
    
    /*  Exibe resultados na tela */
    printf("Arquivos analisados: doc1.txt e doc2.txt\n");
    printf("ANGULO: %.2f graus\n", resplg);
    printf("PERCENTUAL: %.2f por cento\n", percnt);
    
    /*  Limpa memória */
    lst_liberaP(D1);
    lst_liberaP(D2);
    
  return 0;
}

/*  Funções */

/*  Insere um novo elemento na lista de letras */
Lista_L *lst_insere(Lista_L *lis, char L) {
        Lista_L *novo;
        novo = (Lista_L *) malloc(sizeof(Lista_L));
        novo->letra = L;
        novo->prox = lis;
        return novo;
        }
        
/*  Inicializa uma lista de letras */
Lista_L *lst_criaL() {
        return NULL;
        }
        
/*  Inicializa uma lista de palavras */      
Lista_P *lst_criaP() {
        return NULL;
        }

/*  Imprime uma lista de letras */       
void lst_imprime(Lista_L *lis) {
     Lista_L *p;
     for(p = lis; p != NULL; p = p->prox) {
           printf("%c\n", p->letra);
           }
           }

/*  Imprime uma lista de palavras */      
void lst_imprimeP(Lista_P *lis) {
     Lista_P *p;
     for(p = lis; p != NULL; p = p->prox) {
           printf("%s ", p->palavra);
           printf("%d\n", p->contador);
           }
           }

/*  Realiza busca na lista de palavras */         
Lista_P *lst_procuraP (Lista_P *lis, char *plv, int numletras) {
        Lista_P *p;
        int i;
        
        if (lis == NULL) return NULL;
        for (p = lis; p != NULL; p = p->prox) {
            if (p->letras == numletras) {
                              if (strcasecmp(p->palavra, plv) == 0)
                              return p;
                              }
                              }
                              return NULL;
                              }

/*  Converte lista de letras em um vetor de palavra dinamicamente alocado */                          
char *lstLvetor (Lista_L *lis, int numletras) {
     char *plvr;
     plvr = (char *) malloc((numletras+1)*sizeof(char));
     Lista_L *p;
     int i = numletras - 1;                 
     
     for (p = lis; p != NULL; p = p->prox) {
     *(plvr+i) = p->letra;  
     i--;  
     } 
     *(plvr+numletras) = '\0'; 
     return plvr;
     }

/*  Modifica a frequência da palavra em uma unidade a mais */        
void lst_aumentacP(Lista_P *lis) {
     lis->contador++;
     }

/*  Insere novo elemento de lista de palavras */    
Lista_P *lst_insereP(Lista_P *lis, char *plv, int numletras) {
       Lista_P *novo;
       int i;
       novo = (Lista_P *) malloc(sizeof(Lista_P));
       novo->palavra = (char *) malloc((numletras+1)*sizeof(char));
       strcpy(novo->palavra, plv);
       novo->contador = 1;
       novo->letras = numletras;
       novo->prox = lis;
       novo->ant = NULL;
       if (lis != NULL) 
       lis->ant = novo;
       return novo;
       }

/*  Recicla/libera a lista de letras */       
void lst_libera(Lista_L *lis) {
     Lista_L *p;
     p = lis;
     while (p != NULL) {
           Lista_L *t = p->prox;
           free(p);
           p = t;
           }
           }
           
/*  Libera as lista de palavras */       
void lst_liberaP(Lista_P *lis) {
     Lista_P *p;
     p = lis;
     while (p != NULL) {
           Lista_P *t = p->prox;
           free(p);
           p = t;
           }
           }

/*  Função mais complexa: faz leitura e cria lista de letras */         
Lista_P *ltr_lst_crt(FILE *f, Lista_P *D) {
     int numletras;
     char L, *plv;
     Lista_P *resultado;
     Lista_L *temp;
    
    /*  Leitura até o final */
    while (!feof(f)) {    
    /*  Inicializa lista de letras */
    if(feof(f)) break;
    temp = lst_criaL();
    /*  Leitura de uma palavra */
    numletras = 0;
    L = getc(f);
    /*  Verifica se o caractere coletado é alfanumérico */
    if (isalnum(L) != 0) {
    temp = lst_insere(temp, L);
    do {        
        L = getc(f); 
              if (isalnum(L) != 0) {
              temp = lst_insere(temp, L); 
              }
              numletras++;
              } while(isalnum(L) != 0);
    
    if (numletras != 0) {
    /*  Converte lista de letras em vetor de caracteres */               
    plv = lstLvetor(temp, numletras);

    /*  Procura na lista D1 */
    resultado = lst_procuraP(D, plv, numletras);
    /*  Se achar, modifica contador */
    if (resultado != NULL) lst_aumentacP(resultado);
    /*  Se não, adiciona novo elemento */
    else D = lst_insereP(D, plv, numletras);
}
    
    /*  Apaga lista de letras temporária */
    lst_libera(temp);
    /*  Libera vetor temporário de palavra */
    free(plv);
}
}
return D;
} 

/*  Faz a soma dos quadrados das frequências */                        
int contapalavras(Lista_P *lis) {
    Lista_P *p;
    int soma = 0;
    for(p = lis; p != NULL; p=p->prox) soma = soma + pow(p->contador,2);
    return soma;
}

/*  Calcula o ângulo */        
float calcangulo(Lista_P *lis1, Lista_P *lis2, int d1, int d2) {
     Lista_P *p, *q;
     int S = 0, i;
     float ANG, param, a, b;
     for(p = lis1; p != NULL; p = p->prox) {
           for(q = lis2; q != NULL; q = q->prox) {
                 if (p->letras == q->letras) {
                               if (strcasecmp(p->palavra, q->palavra) == 0) { 
                               S = S + (p->contador)*(q->contador);
                               }
                               }
                               }
                               }
                               a = S;
                               b = sqrt(d1)*sqrt(d2);
                               param = a/b;
                               ANG = acos(param) * 180.0/PI;
                               return ANG;
                               } 
                                             
/*  Escreve a lista de palavras no arquivo */                       
void escreve(Lista_P *lis, FILE *f) {
     Lista_P *p;
     for(p = lis; p != NULL; p=p->prox) {
           fprintf(f, "%s - %d\n", p->palavra, p->contador);
           }
           } 

/*  Escreve o resultado no arquivo */
void escreve_res(int n, float pct, FILE *f) {
     if (n == 1) fprintf(f, "Com %.2f por cento, provavelmente ocorreu plagio.\n", pct);
     else if (n == 0) fprintf(f, "Com %.2f por cento, provavelmente NAO ocorreu plagio.\n", pct);
     }

/*  Faz análise do ângulo e retorna um valor binário de decisão */    
int analisa_res(float n) {
     if (n < 45) {
           printf("RESULTADO: Provavelmente, ocorreu plagio\n");
           return 1;
           } else {
                  printf("RESULTADO: Provavelmente, NAO ocorreu plagio\n");
                  return 0;
                  }
                  }

/*  Ordena a lista de palavras por frequências */                
void organizador(Lista_P *lis) {
     Lista_P *p;
     char *plv;
     int cont, let, verificador = 0, i;
     
     for (p = lis; p != NULL; p = p->prox) verificador++;
     for (i = 0; i<verificador; i++) {
     for (p = lis; p->prox != NULL; p = p->prox) {
         
         if (p->contador < p->prox->contador) {
                         
         /*  BACKUP */
         plv = (char *) malloc((p->letras)*sizeof(char));
         strcpy(plv, p->palavra);
         cont = p->contador;
         let = p->letras;
         
         /*  2->1 */
         p->palavra = (char *) malloc((p->prox->letras)*sizeof(char));
         strcpy(p->palavra, p->prox->palavra);
         p->contador = p->prox->contador;
         p->letras = p->prox->letras;
         
         /*  BACKUP->2 */
         p->prox->palavra = (char *) malloc(let*sizeof(char)); 
         strcpy(p->prox->palavra, plv);
         p->prox->contador = cont;
         p->prox->letras = let; 
         
}
} 
} 
free(plv);
}      

/*  Calcula o percentual */ 
float perccomp(float n) {
      if (n == 0) {
            return 100;
            } else {
      if (n == 90) {
            return 0;
            } else {
      float perc;
      perc = 100 - ((9*n)/10);
      return perc;
      }
      }
      }

/*  Limpa memória ocupada pelas listas de palavras */
void limpamemP(Lista_P *lis) {
     Lista_P *p;
     for (p = lis; p != NULL; p = p->prox) free(p->palavra);
     }  
                       
           
     
     
                            
                            
                            
     

