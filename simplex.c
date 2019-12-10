#include "bibsimplex.h"

int main()
{
  int i;
  int linha; //j

  int n,m;
  int** mat;
  int* B;    //Colunas na base canonica

  int resultado = INT_MAX;

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
  PreparaPLParaSimplex(mat, n, m, B, PL_AUXILIAR);

  resultado = SimplexAuxiliar(mat, n, m, B);

  if(resultado == INVIAVEL){

    // A PL eh inviavel: imprime resultado e certificado e finaliza execucao
    ImprimeResultadoFinal(ehviavel, mat, n);
    // Desaloca as coisassssssssssssss ***********************************************
    return 0;
  }else{
    if(resultado == VIAVEL){
      // PASSO 4: Se a PL eh viavel, ve se tem otimo ou se eh ilimitada:
      // Simplex na PL mesmo

      PreparaPLParaSimplex(mat, n, m, B, PL_NORMAL);

      //resultado = Simplex();

    }
  }


  // Construir o simplex com deteccao de ilimitada inclusa
  // Nunca vai dar ilimitada na PL Auxiliar porque ela eh viavel sempre.
  // SimplexAuxiliar();

  //Ao fim: Desalocar a matriz, desalocar vetor base
  // Depois: colocar tudo em float + questao 7 casas decimais
  // Tratar teste 2: matrizes não entram linearmente independentes.
  // Teste 4 da seg fault para PL Auxuliar, antes de fazer PL Original
  // [Ver nas anotações aqui se tem mais algo para tratar, se precisar.]

  return 0;
}

/* ------ Testes
// Testes:
printf("Etapa tal\n");
ImprimeMatriz(mat, n, m);
ImprimeBaseCanonica(B, n);
*/

/* ---- AUX
---------------------
for(i=; i<; i++)
  mat[][] = ;
-------------------
IDEIA: primeiro o simplex funciona para PL Auxiliar, depois funciona para PL
Normal adaptado, se for o caso 2, uma coisa de cada vez.
---------------------
*/
