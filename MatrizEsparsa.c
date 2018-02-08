 /**
 * Universidade Federal de Mato Grosso
 * Campus Universitário do Araguaia
 * Bacharelado em Ciência da Computação
 * Disciplina: Estrutura de Dados II
 * Data: 04/05/2014
 * Professor: Ivairton
 * Arquivo: MatrizEsparsa.c
 * Alunos: Reinaldo Augusto - Thiago Cabral
 *
 * O programa realiza diversas operações com matrizes esparsas.
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Estrutura que define os nós da Matriz **/

typedef struct noMtz {
   int coluna;
   int linha;
   int valor;
   struct noMtz * lado;
   struct noMtz * baixo;
} noMtz;

/** Estrutura que define os nós da lista encadeada **/

typedef struct noLista{
   int *vet;
   struct noLista *prox;
}noLista;

/** Prototipação das Funções **/

void somaDuasLinhas(noMtz ** R,noMtz * L1, noMtz * L2,int TM,int l);
void imprimeLinhaArq(noMtz * elemento, int c, FILE ** Arq);
void insereLinha (noMtz * elemento, noMtz ** cabeca);
void insereColuna(noMtz * elemento, noMtz ** cabeca);
void somaLinha(noMtz * elemento, int v, noMtz **M);
void insereListaSimples(noLista ** v,int ent[3]);
void imprimeLinha(noMtz * elemento, int c);
void removeMtz(noMtz ** M,int l,int c);
void removeMtzLinha(noMtz * L,int c);
void somaMtz(noMtz ** M, int v);
void imprimeLista(noLista * L);
void imprimeMtz(noMtz ** M);
int multLinhaporColuna(noMtz * linha, noMtz * coluna, int t);
int imprimeMtzArq(noMtz ** M, char * Arquivo);
int * buscaValor(noMtz ** M, int c, int l);
int somatorioLinha(noMtz * elemento);
int somatorioMtz(noMtz ** M);
int * descTamMtz(noMtz *M);
noMtz * multiplicaMtz(noMtz **M, noMtz **M2);
noMtz * removeMtzcoluna(noMtz * C,int l);
noMtz * somaDuasMtz(noMtz *A, noMtz *B);
noMtz * achaColuna(noMtz **M, int c);
noMtz * achaLinha(noMtz **M,int l);
noMtz * carregaMtz(char *arq);
noMtz * aloca(int l, int c);
noLista * leArquivo(char* noMtzmeArq);
noLista * removeLista(noLista ** L);

/** Função Principal **/

int main(int argc, char* argv[]) {

   int c, i = 1, flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0, flag6 = 0, flag7 = 0, flag8 = 0, flag9 = 0, flag10 = 0;
   char tmp[1000], **par[7];
   if(argc < 2){
      printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
      return -1;
   }
   if(!strcmp(argv[1],"-help")){
      sprintf(tmp, "Uso: \n\t-imp - Imprimir a matriz na tela; \n\t-s <Valor> - Somar um valor a todos os elementos existentes na matriz;");
      sprintf(tmp,"%s \n\t-e <Entrada> -Lê a matriz de um arquivo \n\t-smt - Somatório dos elementos da matriz;", tmp);
      sprintf(tmp,"%s \n\t-m <Matriz 1> <Matriz 2> - Multiplicar duas matrizes(Matriz 1 e Matriz 2 são lidas a partir de arquivos);", tmp);
      sprintf(tmp,"%s \n\t-o <Saída> - Gera um arquivo de saída; \n\t-b <Linha> <Coluna> - Buscar elmento na matriz", tmp);
      sprintf(tmp,"%s \n\t-r <Linha> <Coluna> - Remover valor da matriz\n\t-ins <Linha> <Coluna> <Valor> - Inserir valor na matriz.", tmp);
      sprintf(tmp,"%s \n\t-sm <Matriz 1> <Matriz 2> - Somar duas matrizes(Matriz 1 e Matriz 2 são lidas a partir de arquivos);",tmp);
      puts(tmp);
      return (0);
   }

   for(c = 0; c < 8; c++){
      par[c] = NULL;
   }

   do{
      if(!strcmp(argv[i],"-e")){
         /** Carregar uma matriz para a matriz resultante **/
         flag1 = 1;
         i++;
         if(argc <= i){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[0] = &argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-imp")){
         /** Imprimir a matriz resultante na tela **/
         flag2 = 1;
         i++;
      }

      else if(!strcmp(argv[i],"-s")){
         /** Soma um valor a todos elementos da matriz esparsa **/
         flag3 = 1;
         i++;
         if(argc <= i){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[1] = &argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-smt")){
         /** Imprime o somatório de todos os elementos da matriz resultante **/
         flag4 = 1;
         i++;
      }

      else if(!strcmp(argv[i],"-m")){
         /** Multiplica Matriz A pela Matriz B e salva o resultado na Matriz resultante **/
         flag5 = 1;
         i++;
         if(argc <= i+1){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[2] = calloc(2, sizeof(char*));
         par[2][0] = argv[i];
         i++;
         par[2][1] = argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-ins")){
         /** Insere um elemento na matriz resultante **/
         flag6 = 1;
         i++;
         if(argc <= i+2){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[3] = calloc(3, sizeof(char*));
         par[3][0] = argv[i];
         i++;
         par[3][1] = argv[i];
         i++;
         par[3][2] = argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-b")){
         /** Busca um elemento na Matriz resultante **/
         flag7 = 1;
         i++;
         if(argc <= i+1){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[4] = calloc(2, sizeof(char*));
         par[4][0] = argv[i];
         i++;
         par[4][1] = argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-r")){
         /** Remove um elemento da Matriz resultante **/
         flag8 = 1;
         i++;
         if(argc <= i+1){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[5] = calloc(2, sizeof(char*));
         par[5][0] = argv[i];
         i++;
         par[5][1] = argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-m")){
         /** Multiplica Matriz A pela Matriz B e salva o resultado na Matriz resultante **/
         flag10 = 1;
         i++;
         if(argc <= i+1){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[2] = calloc(2, sizeof(char*));
         par[2][0] = argv[i];
         i++;
         par[2][1] = argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-o")){
         /** Grava a matriz resultante em um arquivo **/
         flag9 = 1;
         i++;
         if(argc <= i){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[6] = &argv[i];
         i++;
      }

      else if(!strcmp(argv[i],"-sm")){
         /** Soma Matriz A com Matriz B e salva o resultado na Matriz resultante **/
         flag10 = 1;
         i++;
         if(argc <= i+1){
            printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
            return -1;
         }
         par[7] = calloc(2, sizeof(char*));
         par[7][0] = argv[i];
         i++;
         par[7][1] = argv[i];
         i++;
      }

      else{
         printf("ERRO!!\nArgumento[s] inválido[s]! Para ajuda digite %s -help \n", argv[0]);
         return -1;
      }

   }while(i < argc);

   noMtz *M = NULL ,*M2 = NULL,*M3 = NULL;
   int res = 0;

   if(flag1){
      /** Carregar uma matriz para a matriz resultante **/
      M = carregaMtz(*par[0]);
      if(M == NULL){
         printf("Arquivo de entrada inválido!\n");
         return -1;
      }
      sprintf(tmp,"Arquivo %s carregado com sucesso!!",*par[0]);
      puts(tmp);
   }

   if(flag3){
      /** Soma um valor a todos elementos da matriz resultante **/
      somaMtz(&M, atoi(*par[1]));
      sprintf(tmp,"Somado %d a Matriz resultante!", atoi(*par[1]));
      puts(tmp);
   }

   if(flag4){
      /** Imprime o somatório de todos os elementos da matriz resultante **/
      res = somatorioMtz(&M);        /**Arrumar a saída do IF no arquivo!!!!**/
      sprintf(tmp,"O somatório da matriz resultante é %d!",res);
      puts(tmp);
   }

   if(flag5){
      /** Multiplica Matriz A pela Matriz B e salva o resultado na Matriz resultante **/
      sprintf(tmp,"Carregando Matriz A do arquivo \"%s\"",par[2][0]);
      puts(tmp);
      M2 = carregaMtz(par[2][0]);
      puts("Matriz A Carregada");
      sprintf(tmp,"Carregando Matriz B do arquivo \"%s\"",par[2][1]);
      puts(tmp);
      M3 = carregaMtz(par[2][1]);
      puts("Matriz B Carregada");
      if(M2 == NULL || M3 == NULL){
         printf("Arquivo de entrada inválido!\n");
         return -1;
      }
      puts("Multiplicando...");
      M = multiplicaMtz(&M2,&M3);
      sprintf(tmp,"resultado da multiplicação na matriz resultante!");
      puts(tmp);
   }

   if(flag6){
      /** Insere um elemento na matriz resultante **/
      noMtz * novo = aloca(atoi(par[3][0]),atoi(par[3][1]));
      novo->valor = atoi(par[3][2]);
      if(M == NULL){
         M = aloca(0,0);
      }
      insereLinha(novo,&M);
      insereColuna(novo,&M);
      sprintf(tmp,"Elemento %d %d inserido!",atoi(par[3][0]),atoi(par[3][1]));
      puts(tmp);
   }

   if(flag7){
      /** Busca um elemento na Matriz resultante **/
      int * valor = buscaValor(&M,atoi(par[4][0]),atoi(par[4][1]));
      if (valor != NULL){
         sprintf(tmp,"Valor: %d!",*valor);
         puts(tmp);
      }
   }

   if(flag8){
      /** Remover um elemento da Matriz **/
      removeMtz(&M,atoi(par[5][0]),atoi(par[5][1]));
      sprintf(tmp,"Removido com sucesso!");
      puts(tmp);
   }

   if(flag10){
      /** Somar duas matrizes **/
      sprintf(tmp,"Carregando Matriz A do arquivo \"%s\"",par[7][0]);
      puts(tmp);
      M2 = carregaMtz(par[7][0]);
      puts("Matriz A Carregada");
      sprintf(tmp,"Carregando Matriz B do arquivo \"%s\"",par[7][1]);
      puts(tmp);
      M3 = carregaMtz(par[7][1]);
      puts("Matriz B Carregada");
      if(M2 == NULL || M3 == NULL){
         printf("Arquivo de entrada inválido!\n");
         return -1;
      }
      puts("Somando...");

      M = somaDuasMtz(M2, M3);
      sprintf(tmp,"Resultado da soma na matriz resultante!");
      puts(tmp);
   }

   if(flag9){
      /** Impriimir a matriz Resultante em um arquivo **/
      if(imprimeMtzArq(&M, *par[6]) == -1){
         puts("Erro ao abrir arquivo de destino!");
         return -1;
      }
      else{
         puts("Salvo com sucesso!");
      }
   }

   if(flag2){
      /** Impriimir a matriz Resultante na tela **/
      imprimeMtz(&M);
   }

   return (EXIT_SUCCESS);
}

/** Função para alocar os nós da matriz, onde o parâmetro 'l' é a linha e o parâmetro 'c' é a coluna **/

noMtz * aloca(int l, int c){
   noMtz * noMtzvo;
   noMtzvo = (noMtz*) calloc(1, sizeof (noMtz));
   noMtzvo-> coluna = c;
   noMtzvo-> linha = l;
   return noMtzvo;
}

/** Função para ler os valores que serão inseridos na matriz a partir de um arquivo, onde a entrada é uma string com o nome do arquivo **/

noLista * leArquivo(char* noMtzmeArq){
   FILE * fp;
   char c[30];
   int * vet;
   noLista * L = NULL;
   fp = fopen(noMtzmeArq, "r");
   if(fp == NULL){
      puts("Erro ao abrir o arquivo!!!");
      return NULL;
   }
   vet = calloc(3,sizeof(int));
   fscanf(fp, "%*c %*c %*c\n");
   while(!feof(fp)){
      fscanf(fp, "%d %d %d\n",&vet[0],&vet[1],&vet[2]);
      if(vet[2] != 0){
         insereListaSimples(&L,vet);                               //insere os valores lidos do arquivo em uma lista encadeada simples
      }

      vet = calloc(3,sizeof(int));
   }
   return L;
}

/** Função que insere um nó em uma lista simplesmente encadeada **/

void insereListaSimples(noLista ** v,int ent[3]){
   noLista *aux,*novo;
   if(*v == NULL){                                  //Caso a lista ainda esteja vazia
      novo = calloc(1,sizeof(noLista));
      novo->vet = ent;
      *v = novo;                                    //insere um novo nó no inicio da lista
   }else{
      aux = *v;
      while(aux->prox != NULL){                    //percorre a lista até encontrar a ultima posição
         aux = aux->prox;
      }
      novo = calloc(1,sizeof(noLista));
      novo->vet = ent;
      aux->prox = novo;                            //insere novo nó no final da lista existente
   }
}

/** Função que insere o nó em sua devida linha da matriz **/

void insereLinha (noMtz * elemento, noMtz ** cabeca){

   noMtz * aux = *cabeca, *anterior;
   while(aux->baixo != NULL){                               //enquanto não for a ultima linha da matriz
      aux = aux->baixo;                                     //vai para a próxima linha
      if(aux->linha == elemento->linha){                    //se for a linha do nó a ser inserido, sai da repetição
         break;
      }
   }
   while(aux->linha != elemento->linha){                    //enquanto não for a linha do nó a ser inserido,
      aux->baixo = aloca(aux->linha +1, aux->coluna);       //é adicionada uma nova linha a matriz
      aux = aux-> baixo;
   }

   /**Neste ponto, já possui a referência para a linha, basta alcançar a posição em que o elemento será inserido e referenciá-la**/

   while(aux->lado != NULL){                                //enquanto não for o último nó da linha,

      if(aux-> coluna > elemento->coluna){                  //se o nó que será inserido vier antes do nó atual, saia do laço
         break;
      }
      anterior = aux;                                       //guarda uma referencia para o nó atual
      aux = aux->lado;                                      //avança para o próximo nó
   }
   if(aux->coluna > elemento-> coluna){                     //se o nó que será inserido vier antes do nó atual,
      anterior->lado = elemento;                            //nó anterior referencia o novo nó e
      elemento->lado = aux;                                 //o novo nó aponta para o nó atual
   }else{
      aux->lado = elemento;                                 //caso tenha sido encontrado o fim da lista, o último nó da lista referencia o novo nó
   }
}

/** Função que insere o nó em sua devida coluna da matriz **/

void insereColuna(noMtz * elemento, noMtz ** cabeca){

   noMtz * aux = *cabeca, *anterior;

   while(aux->lado != NULL){                                //enquanto não for a última coluna da matriz
      aux = aux->lado;                                      //vai para a próxima coluna
      if(aux->coluna == elemento->coluna){                    //se for a coluna do nó a ser inserida, sai da repetição
         break;
      }
   }
   while(aux->coluna != elemento->coluna){                  //enquanto não for a coluna do nó a ser inserida,
      aux->lado = aloca(aux->linha, aux->coluna +1);        //é adicionada uma nova coluna a matriz
      aux = aux-> lado;
   }

   /**Neste ponto, já possui a referência para a coluna, basta alcançar a posição em que o elemento será inserido e referenciá-la**/

   while(aux->baixo != NULL){                               //enquanto não for o último nó da coluna,

      if(aux->linha > elemento->linha){                     //se o nó que será inserido vier antes do nó atual, sai do laço
         break;
      }
      anterior = aux;                                       //guarda a referência para o nó atual
      aux = aux->baixo;                                     //avança para o próximo nó
   }
   if(aux->linha > elemento->linha){                        //se o nó que será inserido vier antes do nó atual,
      anterior->baixo = elemento;                           //nó anterior referencia o novo nó e
      elemento->baixo = aux;                                //o novo nó aponta para o nó atual
   }else{
      aux->baixo = elemento;                                //caso tenha sido encontrado o fim da coluna, o último nó da lista referencia o novo nó
   }
}

/** Função para imprimir determinada linha da matri **/

void imprimeLinha(noMtz * elemento, int c){
   noMtz * aux = elemento;
   int i;

   for(i = 1; i < c; i++){
      if(aux == NULL){
         printf("[ 0]");

      }else{
         if(aux->coluna != i){
            printf("[ 0]");
         }else{
            printf("[%2d]", aux->valor);
            aux = aux->lado;
         }
      }
   }
   puts("");
}

/** Função que percorre a matriz linha à linha, imprimindo-a **/

void imprimeMtz(noMtz ** M){
   noMtz * aux = *M;
   int c = 0;
   if(aux == NULL){
      puts("Matriz vazia!!");
      return;
   }
   do{
      c++;
      aux = aux->lado;
   }while(aux != NULL);

   aux = *M;

   if(aux->linha == 0){
      aux = aux-> baixo;
   }
   while(aux != NULL){
      imprimeLinha(aux->lado, c);
      aux = aux->baixo;
   }
   puts("");
}

/** Remove o último elemento do buffer de entrada (uma lista encadeada simples) **/

noLista * removeLista(noLista ** L){
   noLista * aux = *L, * anterior;

   if((*L) == NULL){
      return NULL;
   }else if((*L)->prox == NULL){
      aux = *L;
      *L = NULL;
      return aux;
   }else{
      anterior = aux;
      while(aux->prox != NULL){
         anterior = aux;
         aux = aux->prox;
      }
      anterior->prox = NULL;
      return aux;
   }
}

/** Função que imprime o buffer de entrada **/

void imprimeLista(noLista * L){
   noLista * aux = L;
   if(L != NULL){
      do{
         printf("%d %d %d\n", aux->vet[0], aux->vet[1], aux->vet[2]);
         aux = aux->prox;
      }while(aux != NULL);
   }else{
      puts("Lista Vazia");
   }
}

/** Função que busca um elmento de acordo com sua coluna e linha **/

int * buscaValor(noMtz ** M, int c, int l){
   noMtz * aux = (*M)->baixo;
   if(aux == NULL){
      puts("Matriz vazia!!");
      return NULL;
   }

   while(aux != NULL){
      if(aux->linha == l){
         while(aux != NULL){
            if(aux->coluna == c){
               return &aux->valor;
            }else if(aux->coluna > c){
               return NULL;
            }
            aux = aux->lado;
         }
      }
      aux = aux->baixo;
   }
   return NULL;
}

/** Função para somar um numero a todos elementos diferentes de zero da Matriz **/

void somaMtz(noMtz ** M, int v){
   noMtz * aux = *M;
   if(aux == NULL){
      return;
   }
   if(aux->linha == 0){
      aux = aux-> baixo;
   }
   while(aux != NULL){
      somaLinha(aux->lado, v,M);
      aux = aux->baixo;
   }
}

/** Função para somar um valor a todos elementos de uma linha da Matriz **/

void somaLinha(noMtz * elemento, int v, noMtz **M){
   noMtz * aux = elemento, * anterior;
   while(aux != NULL){
      aux->valor += v;
      anterior = aux;
      aux = aux->lado;
      if(anterior->valor == 0){
         removeMtz(M,anterior->linha,anterior->coluna);
      }
   }
}

/** Função que retorna o somatório de todos os elementos de uma linha da Matriz **/

int somatorioLinha(noMtz * elemento){
   noMtz * aux = elemento;
   int Tot = 0;
   if(aux != NULL){
      do{
         //printf("[%d]",aux->valor);
         Tot += aux->valor;
         aux = aux->lado;
      }while(aux != NULL);
   }
   //puts("");
   return Tot;
}

/** Função que retorna o somatório de todos os elementos da Matriz **/

int somatorioMtz(noMtz ** M){
   noMtz * aux = *M;
   int Tot = 0;
   if(aux == NULL){
      return;
   }

   if(aux->linha == 0){
      aux = aux-> baixo;
   }
   while(aux != NULL){
      Tot += somatorioLinha(aux->lado);
      aux = aux->baixo;
   }
   return Tot;
}

/** Função para remover um elemento da matriz **/

void removeMtz(noMtz ** M,int l,int c){
   noMtz * aux = *M;
   noMtz * linha, * coluna,* removido;
   linha = achaLinha(M,l);
   coluna = achaColuna(M,c);
   removeMtzLinha(linha,c);
   removeMtzcoluna(coluna,l);
}

/** Função para remover as referencias de um elemento em uma linha **/

void removeMtzLinha(noMtz * L,int c){
   noMtz *aux = L, *anterior;
   while(aux != NULL){
      if(aux->coluna > c){
         return;
      }else if(aux->coluna == c){
         anterior->lado = aux->lado;
         return;
      }
      anterior = aux;
      aux = aux->lado;
   }
}

/** Função para remover as referencias de um elemento em uma coluna **/

noMtz * removeMtzcoluna(noMtz * C,int l){
   noMtz *aux = C, *anterior;
   while(aux != NULL){
      if(aux->linha > l){
         return;
      }else if(aux->linha == l){
         anterior->baixo = aux->baixo;
         return aux;
      }
      anterior = aux;
      aux = aux->baixo;
   }
}

/** Função que retorna o ponteiro do nó cabeca da linha l **/

noMtz * achaLinha(noMtz **M,int l){
   noMtz *aux = *M;
   while(aux != NULL){
      if(aux->linha == l){
         return aux;
      }
      aux = aux->baixo;
   }
   return NULL;
}

/** Função que retorna o ponteiro do nó cabeca da coluna c **/

noMtz * achaColuna(noMtz **M, int c){
   noMtz *aux = *M;
   while(aux != NULL){
      if(aux->coluna == c){
         return aux;
      }
      aux = aux->lado;
   }
   return NULL;
}

/** Descobre o Tamanho da Matriz **/

int * descTamMtz(noMtz *M){
   noMtz * aux = M;
   int * t = calloc(2, sizeof(int));
   while(aux->baixo != NULL){
      aux = aux->baixo;
   }
   t[0] = aux->linha;
   aux = M;
   while(aux->lado != NULL){
      aux = aux->lado;
   }
   t[1] = aux->coluna;
   return t;
}

/** Multiplica uma linha por uma coluna **/

int multLinhaporColuna(noMtz * linha, noMtz * coluna, int t){
   int cont, pri, seg, r = 0;
   for(cont = 1; cont <= t; cont++){
      if(linha != NULL && coluna != NULL){
         //printf("%dx%d = %dx%d = %d  ",linha->coluna,coluna->linha,linha->valor,coluna->valor,linha->valor*coluna->valor );
         if(linha->coluna == cont){
            pri = linha->valor;
            linha = linha->lado;
         }else{
            pri = 0;
         }
         if(coluna->linha == cont){
            seg = coluna->valor;
            coluna = coluna->baixo;
         }else{
            seg = 0;
         }
      }
      else if(linha == NULL && coluna != NULL){
         pri = 0;
         if(coluna->linha == cont){
            seg = coluna->valor;
            coluna = coluna->baixo;
         }else{
            seg = 0;
         }
      }
      else if(linha != NULL && coluna == NULL){
         if(linha->coluna == cont){
            pri = linha->valor;
            linha = linha->lado;
         }else{
            pri = 0;
         }
         seg = 0;
      }
      r += (pri*seg);
   }
   return r;
}

/** Multiplica duas matrizes e retorna a matriz resultante **/

noMtz * multiplicaMtz(noMtz **M, noMtz **M2){
   int * t1, * t2, i, j, *tmp;
   noMtz * l, * c, * res, * aux;
   noLista * L = NULL, *lAux;

   res = aloca(0,0);
   aux = res;
   t1 = descTamMtz(*M);
   t2 = descTamMtz(*M2);
   tmp = calloc(3,sizeof(int));
   //printf("A: %dx%d B: %dx%d\n",t1[0],t1[1],t2[0],t2[1]);
   if(t1[1] != t2[0]){
      return NULL;
   }else{
      for(i = 1; i <= t1[0]; i++){
         aux->baixo = aloca(i,0);
         aux = aux->baixo;
      }
      aux = res;

      for(i = 1; i <= t2[1]; i++){
         aux->lado = aloca(0,i);
         aux = aux->lado;
      }
      int x;
      for(i = 1;  i <= t1[0]; i++ ){
         for(j = 1; j <= t2[1]; j++){
            l = achaLinha(M, i);
            c = achaColuna(M2, j);
            tmp[0] = i;
            tmp[1] = j;
            tmp[2] = multLinhaporColuna(l->lado, c->baixo, t1[1]);
            //printf("L: %d C: %d V: %d\n",tmp[0],tmp[1],tmp[2]);
            aux = aloca(tmp[0],tmp[1]);
            aux->valor = tmp[2];
            insereLinha(aux, &res);
            insereColuna(aux, &res);
         }
      }
      return res;
   }
}

/** Retorna uma matriz a partir de um aquivo formatado **/

noMtz * carregaMtz(char *arq){
   noLista * L, * lAux;
   noMtz * M,* mAux;
   M = aloca(0,0);
   //puts("Lendo arquivo");
   L = leArquivo(arq);
   //puts("Inserindo Elementos");
   //puts("");
   if(L == NULL)
      return NULL;
   while(L != NULL){
      lAux = removeLista(&L);
      mAux = aloca(lAux->vet[0],lAux->vet[1]);
      mAux->valor = lAux->vet[2];
      insereLinha(mAux, &M);
      insereColuna(mAux, &M);
   }
   return M;
}

/** Salva a matriz em um arquivo **/

int imprimeMtzArq(noMtz ** M, char * Arquivo){
   noMtz * aux = *M;
   int c = 0;
   FILE * fp = fopen(Arquivo, "w");
   if(fp == NULL){
      puts("Erro ao abrir arquivo!");
      return -1;
   }
   if(aux == NULL){
      puts("Matriz vazia!");
      return -1;
   }
   do{
      c++;
      aux = aux->lado;
   }while(aux != NULL);

   aux = *M;

   if(aux->linha == 0){
      aux = aux-> baixo;
   }
   while(aux != NULL){
      imprimeLinhaArq(aux->lado, c, &fp);
      aux = aux->baixo;
   }
   return 1;
}

/** Imprime uma linha da matriz em um arquivo **/

void imprimeLinhaArq(noMtz * elemento, int c, FILE ** Arq){
   noMtz * aux = elemento;
   int i;
   char buffer[10];

   for(i = 1; i < c; i++){
      if(aux == NULL){
         fputs("0 ", *Arq);

      }else{
         if(aux->coluna != i){
            fputs("0 ", *Arq);
         }else{
            sprintf(buffer,"%d ", aux->valor);
            fputs(buffer, *Arq);
            aux = aux->lado;
         }
      }
   }
   sprintf(buffer,"\n");
   fputs(buffer, *Arq);
}

/** Soma Duas matrizes e retorna  a matriz resultante **/

noMtz * somaDuasMtz(noMtz *A, noMtz *B){
   int * TA, * TB, TC[2],l;
   noMtz * aux = A->baixo;
   noMtz * aux2 = B->baixo;
   noMtz *R;
   R = aloca(0,0);                                          //é alocado o nó cabeça de uma nova matriz que guardará o resultado da soma

   TA = descTamMtz(A);                                      //a função descTamMtz retorna um vetor de inteiros com duas posições,
   TB = descTamMtz(B);                                      //onde a primeira é o tamanho da linha e a segunda é o tamanho da coluna

   if(TA[0] > TB[0]){                                       //descobre qual matriz tem o maior numero de linhas
      TC[0] = TA[0];
   }else{
      TC[0] = TB[0];
   }
   if(TA[1] > TB[1]){                                       //descobre qual matriz tem o maior numero de colunas
      TC[1] = TA[1];                                        //
   }else{
      TC[1] = TB[1];                                        //a matriz resultante têm a dimensão Maior_quantidade_linhas x Maior_quantidade_colunas
   }

   for(l = 1; l <= TC[0]; l++){                             //laço que percorre as linhas da matriz
      somaDuasLinhas(&R,aux,aux2,TC[1],l);                  //somando os elementos de cada linha de cada matriz
      if(aux != NULL){
         aux = aux->baixo;
      }
      if(aux2 != NULL){
         aux2 = aux2->baixo;
      }
   }
   return R;                                                //retorna a matriz resultante
}

/** Soma duas linhas e retorna a lista resultante **/

void somaDuasLinhas(noMtz ** R,noMtz * L1, noMtz * L2,int TM,int l){
   int c, vt1, vt2,vtr;
   L1 = L1->lado;                                           //A linha L1 é inicializada com o seu proximo elemento, para não ser iniciada na linha de cabeçalho
   L2 = L2->lado;                                           //A linha L2 é inicializada com o seu proximo elemento, para não ser iniciada na linha de cabeçalho

   for (c = 1 ; c <= TM; c++){                              //laço que percorre todos as colunas da linha

      if(L1 != NULL && L1->coluna == c){                    //caso exista algum elemento na posição do contador para a primeira matriz,
         vt1 = L1->valor;                                   //a variavel vt1 recebe o valor do elemento
         L1 = L1->lado;                                     //e avança o número da coluna
      }else{
         vt1 = 0;                                           //caso não exista elemento na coluna c, é assumido o valor 0 para a soma
      }

      if(L2 != NULL && L2->coluna == c){                    //caso exista algum elemento na posição do contador para a segunda matriz,
         vt2 = L2->valor;                                   //a variável vt2 recebe o valor do elemento
         L2 = L2->lado;                                     //e avança o número da coluna
      }else{
         vt2 = 0;
      }                                                     //caso não exista elemento na coluna c, é assumido o valor 0 para a soma

      vtr = vt1 + vt2;                                      //são somados os valores referentes as colunas c  das matrizes

      if(vtr != 0){                                         //se o resultado da soma for diferente de 0,
         noMtz * novo = aloca(l,c);                         //um novo nó é criado e o valor é inserido na posição correta
         novo->valor = vtr;
         insereColuna(novo,R);
         insereLinha(novo,R);
      }
   }
}
