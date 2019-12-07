#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i;
  int linha; //j

  int n,m;
  int** mat;

  // Inicio: construcao da Estrutura de Dados
  // ----------------------------------------
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
    mat[linha][linha-1] = 1;

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

    // Vetor b
    scanf("%d ", &(mat[linha][m+(3*n)]) );
  }

  //Criar função de impressao so pra testar a leitura

  //Ao fim: Desalocar a matriz
  return 0;
}

/*
for(i=; i<; i++)
  mat[][] = ;
*/
