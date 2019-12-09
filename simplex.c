#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NAO 0
#define SIM 1
#define INVIAVEL 0
#define VIAVEL 1
#define ILIMITADA 2
#define OTIMA 3
#define ALEATORIO_POSITIVO_GRANDE 1000

// Assinaturas das funcoes
//Testes
void ImprimeMatrizParaTeste(int **mat, int n, int m);
void ImprimeBaseCanonica(int* B, int n);
// Reais
void PreparaAuxiliarParaSimplex(int** mat, int n, int m, int* B);
int EncontraFormaDeOtimizar(int** mat, int n, int m);
int ChecaSeEilimitada(int k, int** mat, int n);
void TrocaBase(int* B, int cn, int linha);
int EscolhePivot(int cn, int** mat, int n, int m);
int SimplexAuxiliar(int** mat, int n, int m);
void ImprimeResultadoFinal(int resultado, int** mat, int n);

// Programa
int main()
{
  int i;
  int linha; //j

  int n,m;
  int** mat;
  int* B;    //Colunas na base canonica

  int ehviavel = ALEATORIO_POSITIVO_GRANDE;

  // PASSO 1: Construcao da Estrutura de Dados
  // -----------------------------------------

  // Cria a matriz
  scanf("%d %d ", &n, &m);
  mat = (int **) malloc( (n+2) * sizeof(int*) );
  for(i=0; i<(n+2); i++)
    mat[i] = (int *) malloc( (m+(3*n)+1) * sizeof (int) );

  // 1a linha: Vetor de custos -c
  for(i=0; i<n; i++)
    mat[0][i] = 0;
  for(i=n; i<(n+m); i++){
    scanf("%d ", &(mat[0][i]) );
    mat[0][i] = mat[0][i] * (0-1) ;
  }
  for(i=(n+m); i<(m+(3*n)+1); i++)
    mat[0][i] = 0;

  // 2a linha: Vetor de custos da matriz extendida, da PL Auxiliar em FPI
  // e valor objetivo
  for(i=0; i<( m+(2*n) ); i++)
    mat[1][i] = 0;
  for(i=(m+(2*n)); i<( m+(3*n) ); i++)
    mat[1][i] = 1;
  mat[1][m+(3*n)] = 0;

  // Outras linhas: Matriz extendida, A em FPI, PL Auxiliar e vetor b
  // Cada linha:
  for(linha=2; linha<(n+2); linha++){

    //Matriz Identidade da matriz extendida
    for(i=0; i<n; i++)
      mat[linha][i] = 0;
    mat[linha][linha-2] = 1;

    //Matriz A
    for(i=n; i<(n+m); i++)
      scanf("%d ", &(mat[linha][i]) );

    // A em FPI: completa com matriz Identidade
    for(i=(n+m); i<( m+(2*n) ); i++)
      mat[linha][i] = 0;
    mat[linha][m+n+(linha-2)] = 1;

    // Matriz Identidade da PL Auxiliar
    for(i=( m+(2*n) ); i<( m+(3*n) ); i++)
      mat[linha][i] = 0;
    mat[linha][m+(2*n)+(linha-2)] = 1;

    // Vetor de coeficientes b
    scanf("%d ", &(mat[linha][m+(3*n)]) );
  }

  // Vetor que armazena colunas na base canonica da vez
  B = (int*) malloc( n*(sizeof(int)) );

  // PASSO 2: Checa se coeficientes b < 0
  // -----------------------

  for(linha=2; linha<(n+2); linha++){
    if(mat[linha][m+(3*n)] < 0){
      //Inverte a linha toda, menos a matriz I da Auxiliar
      for(i=0; i<(m+(2*n)); i++)
        mat[linha][i] *= (-1);
      mat[linha][m+(3*n)] *= (-1);
    }
  }

  // PASSO 3: Vê se a PL é viável: Simplex na PL Auxiliar
  // ----------------------------------------------------

  PreparaAuxiliarParaSimplex(mat, n, m, B);

  ehviavel = SimplexAuxiliar(mat, n, m);

  if(ehviavel == INVIAVEL){
    ImprimeResultadoFinal(ehviavel, mat, n);
    return 0;
  }else{
    if(ehviavel == VIAVEL){
      // Chama o simplex da PL normal pra PL normal:
      // A PL eh ilimitada ou otima
    }
  }


  // Construir o simplex com deteccao de ilimitada inclusa
  // Nunca vai dar ilimitada na PL Auxiliar porque ela eh viavel sempre.
  // SimplexAuxiliar();

  //Ao fim: Desalocar a matriz, desalocar vetor base
  // Ao fim de tudo: modularizar mais.
  // Depois: colocar tudo em float + questao 7 casas decimais
  // Tratar teste 2: matrizes não entram linearmente independentes.
  // [Ver nas anotações aqui se tem mais algo para tratar, se precisar.]

  return 0;
}

/* AUX
for(i=; i<; i++)
  mat[][] = ;
*/
/*
IDEIA: primeiro o simplex funciona para PL Auxiliar, depois funciona para PL
Normal adaptado, se for o caso 2, uma coisa de cada vez.
*/
// Funcoes

// Testes
// -----

// Chamada no codigo:
// Testes
// ImprimeMatrizParaTeste(mat, n, m);
// ImprimeBaseCanonica(B, n);
void ImprimeMatrizParaTeste(int **mat, int n, int m){

  int i, linha;

  printf("MATRIZ:\n");

  for(linha=0; linha<(n+2); linha++){
    for(i=0; i< (m+(3*n)+1); i++)
      printf("%d\t", mat[linha][i]);
    printf("\n");
  }

  return;
}

void ImprimeBaseCanonica(int* B, int n){

  printf("B = ");

  for(int i=0; i<n; i++)
    printf("%d ", B[i]);

  printf("\n");

  return;
}

// Reais
// -----

// Coloca a base em forma canonica: Ab = I ok, agora Cb = 0:
void PreparaAuxiliarParaSimplex(int** mat, int n, int m, int* B){

  int i, j;
  int aux;

  // Define a base canônica: na PL Auxiliar ela sempre começa
  // da identidade à direita
  aux =  = m+n;
  for(i=0; i<n; i++){
    aux++;
    B[i] = aux;
  }

  // Coloca em forma canonica: Ab = I ok, agora Cb = 0
  for(i=0; i<n; i++){
    for(j=0; j<(m+(3*n)+1); j++){
      mat[1][j] -= mat[i+2][j];
    }
  }

  return;
}

// Simplex da Auxiliar
// -------------------

// Procura por Cn positivo no vetor de custos
// Retorna a coluna da matriz com cn que permite melhorar
int EncontraFormaDeOtimizar(int** mat, int n, int m){

  int i;

  for(i=1; i<(m+(2*n)+1); i++){
    if( mat[1][(n-1)+i] < 0 )
      return i;
  }

  return 0;
}

// Percorre a coluna Ak para ver se a PL eh ilimitada
int ChecaSeEilimitada(int k, int** mat, int n){

  for(int i=2; i<(n+2); i++){
    if(mat[i][n-1+k] > 0)
      return NAO;
  }

  return SIM;
}

// Inclui novo elemento selecionado na base canônica
void TrocaBase(int* B, int cn, int linha){

  B[linha-1] = cn;

  return;
}

// Escolhe o elemento a ser pivoteado - retorna a linha de Ak
int EscolhePivot(int cn, int** mat, int n, int m){

  int min = INT_MAX;

  for(int i=2; i<(n+2); i++)
    if( (mat[i][m+(3*n)] > 0) && (mat[i][n-1+cn] > 0) )  //*********************
      if( (mat[i][m+(3*n)] / mat[i][n-1+cn]) < min )
        min = i-1; //linha de Ak

  return min;
}

// Simplex da Auxiliar
int SimplexAuxiliar(int** mat, int n, int m){

  int cn;
  int ehIlimitada;
  int linha;

  // Entrou uma PL em formato canonico, Ab = I e Cb = 0, e base dela B.

  // 1. Procura Cn para melhorar a solucao
  cn = 0;
  cn = EncontraFormaDeOtimizar(mat, n, m);

  if (cn == 0){ //Nao tem mais como otimizar com o Simplex
    //Verifica o valor objetivo
    if( mat[1][m+(3*n)] < 0 ) // PL Aux inviavel
      return INVIAVEL;
    else{
      if(mat[1][m+(3*n)] == 0) // PL Aux viavel
        return VIAVEL;
    }
  }else // Existe cn otimizavel, coluna salva na variavel cn
  {
    // Não usado na PL Auxiliar: checar se a PL eh ilimitada
    ehIlimitada = ChecaSeEilimitada(cn, mat, n, m);
    if(ehIlimitada == SIM){
      // Nao tratar no caso de PL Auxiliar:
      // Elas sao sempre viaveis e com solucao otima.
      // No caso de Simplex p PL Normal age (retorna
      // do simplex para imprimir e finalizar).

      // return ILIMITADA;
      // E TRATA LA FORA
    }

    // Caso nao seja ilimitada:
    linha = EscolhePivot(cn, mat, n, m);
    TrocaBase(B, cn, linha);

    PivoteiaParaFormaCanonica(cn, linha, mat, n, m, B);


  }
}

// Imprime Resultado para finalizar o programa
void ImprimeResultadoFinal(int resultado, int** mat, int n){

  switch(resultado){
    case INVIAVEL:
      printf("inviavel\n");
      for(int i=0; i<n; i++){
        printf("%d ", mat[1][i]);
      }
      break;
    case ILIMITADA:
      //DO
      break;
    case OTIMA;
      //DO
      break;
  }

  return;
}
