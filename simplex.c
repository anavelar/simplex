#include "bibsimplex.h"

int main()
{
  int i;
  int linha; //j

  int n,m;
  int** mat;
  int* B;    //Colunas na base canonica

  int ehviavel = INT_MAX;

  // PASSO 1: Construcao da Estrutura de Dados

  // Cria a matriz - PL extendida, PL em FPI e PL Auxiliar na matriz
  scanf("%d %d ", &n, &m);
  mat = (int **) malloc( (n+2) * sizeof(int*) );
  for(i=0; i<(n+2); i++)
    mat[i] = (int *) malloc( (m+(3*n)+1) * sizeof (int) );
  PreencheMatriz(mat, n, m);

  // Cria o vetor colunas na base canonica
  B = (int*) malloc( n*(sizeof(int)) );

  // PASSO 2: Checa se coeficientes b > 0
  EliminaBnegativos(mat, n, m);

  // PASSO 3: Vê se a PL é viável: Simplex na PL Auxiliar
  PreparaPLAuxiliarParaSimplex(mat, n, m, B);

  ehviavel = SimplexAuxiliar(mat, n, m, B);

  if(ehviavel == INVIAVEL){
    ImprimeResultadoFinal(ehviavel, mat, n);
    // Desaloca as coisassssssssssssss
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

  //teste de execucao
  printf("Acabou, executou, falous.\n");

  return 0;
}

/* ---- AUX
---------------------
for(i=; i<; i++)
  mat[][] = ;
---------------------
// Testes:
// ImprimeMatriz(mat, n, m);
// ImprimeBaseCanonica(B, n);
-------------------
IDEIA: primeiro o simplex funciona para PL Auxiliar, depois funciona para PL
Normal adaptado, se for o caso 2, uma coisa de cada vez.
---------------------
*/
