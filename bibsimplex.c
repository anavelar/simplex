#include "bibsimplex.h"

// Testes
// -----

void ImprimeMatriz(int **mat, int n, int m){

  int i, linha;

  printf("MATRIZ:\n");

  for(linha=0; linha<(n+2); linha++){

    for(i=0; i< (m+(3*n)+1); i++)
      printf("%d\t", mat[linha][i]);

    printf("\n");

    if(linha < 2)
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

// Funcoes mesmo
// -------------

void PreencheMatriz(int** mat, int n, int m){

  int i;
  int linha;  //j

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

  return;
}

// Quando os coneficientes b da matriz b de coeficientes forem negativos,
// inverte a linha (multiplica por -1)-(Menos a matriz I da PL Auxiliar Claro)
void EliminaBnegativos(int** mat, int n, int m){

  int linha;

  for(linha=2; linha<(n+2); linha++){
    if(mat[linha][m+(3*n)] < 0){
      //Inverte a linha toda, menos a matriz I da Auxiliar
      for(int i=0; i<(m+(2*n)); i++)
        mat[linha][i] *= (-1);
      mat[linha][m+(3*n)] *= (-1);
    }
  }

  return;
}

// Na PL Auxiliar, define quem sera a base canonica e coloca a base
// em formato canonico: Ab = I e Cb = 0.
void PreparaPLParaSimplex(int** mat, int n, int m, int* B, int tipoPL){

  int i, j;
  int aux;
  int multiplicador;

  if(tipoPL == PL_AUXILIAR){
    // Define a base canonica: na PL Auxiliar ela sempre começa
    // da identidade à direita
    aux = m+n;
    for(i=0; i<n; i++){
      aux++;
      B[i] = aux;
    }

    // Coloca em forma canonica: Ab = I ok, agora Cb = 0
    for(i=0; i<n; i++){
      for(j=0; j<(m+(3*n)+1); j++){
        mat[1][j] -= mat[i+2][j];
        // Vetor de custos real:
        if( (j>(n-1)) && (j<(m+(2*n))) )
          mat[0][j] -= mat[i+2][j];
      }
    }

    return;
  }
  else
  {
    if(tipoPL == PL_NORMAL){
      // Ja temos a base canonica, precisamos Cb = 0 para iniciar o simplex.

      // Checa se todos custos da base B estao iguais a zero
      for(i=0; i<n; i++){

        // Para cada coluna na Base:
        if( mat[0][n-1+B[i]] != 0 ){

          // Custo < 0
          if( mat[0][n-1+B[i]] < 0 ){

            multiplicador = ( (-1) * (mat[0][n-1+B[i]]) );

            // Para a PL extendida:
            for(j=0; j<n; j++)
              mat[1][j] += (multiplicador * mat[i+2][j]) ; //*************
            // Para C:
            for(j=n; j<(m+(2*n)); j++)
              mat[0][j] += (multiplicador * mat[i+2][j]);
            // Para vetor de coeficientes b:
            mat[1][m+(3*n)] += ( multiplicador * mat[i+2][m+(3*n)] );

          }
          else // Custo > 0 ou Custo == 0
          {
            // Custo > 0
            if( mat[0][n-1+B[i]] > 0 ){

              multiplicador = mat[0][n-1+B[i]];

              // Para a PL extendida:
              for(j=0; j<n; j++)
                mat[1][j] -= (multiplicador * mat[i+2][j]) ; //*************
              // Para C:
              for(j=n; j<(m+(2*n)); j++)
                mat[0][j] -= (multiplicador * mat[i+2][j]);
              // Para vetor de coeficientes b:
              mat[1][m+(3*n)] -= ( multiplicador * mat[i+2][m+(3*n)] );

            }
            // else: if( mat[0][n-1+B[i]] == 0 ): ai nao precisa fazer nada.
          }
        }
        //else: Se esse custo for zero, nao precisa fazer nada.

      }
    }
  }
}

// Internas ao Simplex na PL Auxiliar
// ----------------------------------

// Procura por Cn positivo no vetor de custos
// Retorna a coluna da matriz com cn que permite melhorar
int EncontraFormaDeOtimizar(int** mat, int n, int m){

  int i;

  for(i=1; i<(m+(2*n)+1); i++){
    if( mat[1][n-1+i] < 0 )
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

// Escolhe o elemento a ser pivoteado - retorna a linha de Ak
int EscolhePivot(int cn, int** mat, int n, int m){

  int min = INT_MAX;

  for(int i=2; i<(n+2); i++)
    if( (mat[i][m+(3*n)] > 0) && (mat[i][n-1+cn] > 0) )
      if( (mat[i][m+(3*n)] / mat[i][n-1+cn]) < min )
        min = i-1; //linha de Ak

  return min;
}

// Inclui novo elemento selecionado na base canônica
void TrocaBase(int* B, int cn, int linha){

  B[linha-1] = cn;

  return;
}

// Para colocar a PL em forma canonica de acordo com nova base:Ab = I e Cb = 0
void PivoteiaParaFormaCanonica(int cn, int linha, int** mat, int n, int m){

  int i,j;
  int multiplicador;

  // Primeiro: transformar o elemento pivot em 1
  mat[linha+1][n-1+cn] = (mat[linha+1][n-1+cn] / mat[linha+1][n-1+cn]);

  // Segundo: faz Cb = 0 (Ck = 0)
  multiplicador = (-1) * mat[1][n-1+cn];
  for(j=0; j<(m+(3*n)+1); j++){
    mat[1][j] += (multiplicador * mat[linha+1][j]);

    //Vetor custos tb:
    if( (j>(n-1)) && (j<(m+(2*n))) )
      mat[0][j] += (multiplicador * mat[linha+1][j]);
  }

  // Terceiro: Ab = I (Ak = Coluna da Identidade que caiu para ela)
  for(i=2; i<(n+2); i++){
    if(i != (linha+1)){
      // Para Cada Linha
      if(mat[i][n-1+cn] > 0){
        multiplicador = mat[i][n-1+cn];
        for(j=0; j<(m+(3*n)+1); j++)
          mat[i][j] -= (multiplicador * mat[linha+1][j]);
      }
      else
      {
        if(mat[i][n-1+cn] < 0){
          multiplicador = ((-1) * mat[i][n-1+cn]);
          for(j=0; j<(m+(3*n)+1); j++)
            mat[i][j] += (multiplicador *  mat[linha+1][j]);
        }
        // else: if(mat[i][n-1+cn] == 0)
        // ai nao precisa mexer nessa linha.
      }
    }
  }

  return;
}

// Simplex Executado na PL Auxiliar
int SimplexAuxiliar(int** mat, int n, int m, int* B){

  int cn;     // Indice teorico
  int linha;  // Indice teorico
  int ehIlimitada;

  // Entrou uma PL em formato canonico, Ab = I e Cb = 0, e base dela B.

  // 1. Procura Cn para melhorar a solucao
  cn = EncontraFormaDeOtimizar(mat, n, m);

  if (cn == 0){ //Nao tem mais como otimizar com o Simplex
    //Verifica o valor objetivo
    if( mat[1][m+(3*n)] < 0 ) // PL Aux inviavel
      return INVIAVEL;
    else
      if(mat[1][m+(3*n)] == 0) // PL Aux viavel
        return VIAVEL;
  }
  else // Existe cn otimizavel, coluna salva na variavel cn
  {
    // 2. (Não usado na PL Auxiliar) checar se a PL eh ilimitada
    ehIlimitada = ChecaSeEilimitada(cn, mat, n);
    if(ehIlimitada == SIM){
      // Nao tratar no caso de PL Auxiliar:
      // Elas sao sempre viaveis e com solucao otima.
      // No caso de Simplex p PL Normal age (retorna
      // do simplex para imprimir e finalizar).

      //teste
      printf("EH ILIMITADA.\n");

      // return ILIMITADA;
      // E TRATA LA FORA
    }

    // Caso nao seja ilimitada:
    linha = EscolhePivot(cn, mat, n, m);
    TrocaBase(B, cn, linha);

    PivoteiaParaFormaCanonica(cn, linha, mat, n, m);

    // ******************** PODE ESTAR AQUI TB
    return ( SimplexAuxiliar( mat, n, m, B) );
  }
}

// Imprime o resultado do Simplex executado
void ImprimeResultadoFinal(int resultado, int** mat, int n){

  switch(resultado){

    case INVIAVEL:
      printf("inviavel\n");
      for(int i=0; i<n; i++)
        printf("%d ", mat[1][i]);
      break;

    case ILIMITADA:
      //DO
      break;

    case OTIMA:
      //DO
      break;
  }

  return;
}
