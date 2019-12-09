#include <stdio.h>
#include <stdlib.h>

// Assinaturas das funcoes
void ImprimeMatrizParaTeste(int **mat, int n, int m);
void ImprimeBaseCanonica(int* B, int n);
void PreparaAuxiliarParaSimplex(int** mat, int n, int m, int* B);

// Programa
int main()
{
  int i;
  int linha; //j

  int n,m;
  int** mat;
  int* B;    //Colunas na base canonica

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

  // Construir o simplex com deteccao de ilimitada inclusa
  // Nunca vai dar ilimitada na PL Auxiliar porque ela eh viavel sempre.

  //Ao fim: Desalocar a matriz, desalocar vetor base
  // Depois: colocar tudo em float + questao 7 casas decimais
  // Tratar teste 2: matrizes não entram linearmente independentes.
  // [Ver nas anotações aqui se tem mais algo para tratar, se precisar.]

  return 0;
}

/* AUX
for(i=; i<; i++)
  mat[][] = ;
*/

// Funcoes

// Chamada no codigo:
// Testes
// ImprimeMatrizParaTeste(mat, n, m);
// ImprimeBaseCanonica(B, n);

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
