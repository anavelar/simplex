#include "bibsimplex.h"

// Testes
// -----

void ImprimeMatriz(float **mat, int n, int m){

  int i, linha;

  printf("MATRIZ:\n");

  for(linha=0; linha<(n+2); linha++){

    for(i=0; i< (m+(3*n)+1); i++)
      printf("%.7f\t", mat[linha][i]);

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

void PreencheMatriz(float** mat, int n, int m){

  int i;
  int linha;  //j

  // 1a linha: Vetor de custos -c
  for(i=0; i<n; i++)
    mat[0][i] = 0;
  for(i=n; i<(n+m); i++){
    scanf("%f ", &(mat[0][i]) );
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
      scanf("%f ", &(mat[linha][i]) );

    // A em FPI: completa com matriz Identidade
    for(i=(n+m); i<( m+(2*n) ); i++)
      mat[linha][i] = 0;
    mat[linha][m+n+(linha-2)] = 1;

    // Matriz Identidade da PL Auxiliar
    for(i=( m+(2*n) ); i<( m+(3*n) ); i++)
      mat[linha][i] = 0;
    mat[linha][m+(2*n)+(linha-2)] = 1;

    // Vetor de coeficientes b
    scanf("%f ", &(mat[linha][m+(3*n)]) );
  }

  return;
}

// Quando os coneficientes b da matriz b de coeficientes forem negativos,
// inverte a linha (multiplica por -1)-(Menos a matriz I da PL Auxiliar Claro)
void EliminaBnegativos(float** mat, int n, int m){

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
void PreparaPLParaSimplex(float** mat, int n, int m, int* B, int tipoPL){

  int i, j;
  int aux;
  float multiplicador;

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

// Internas ao Simplex
// -------------------

// Procura por Cn positivo no vetor de custos
// Retorna a coluna da matriz com cn que permite melhorar
int EncontraFormaDeOtimizar(float** mat, int n, int m, int tipoPL){

  int i;
  int inicioColunasPL, fimColunasPL, ondec;

  if(tipoPL == PL_AUXILIAR){

    inicioColunasPL = 1;
    fimColunasPL = m+(2*n);
    ondec = 1;
  }
  else {

    if(tipoPL == PL_NORMAL){
      inicioColunasPL = 1;
      fimColunasPL = m+n;
      ondec = 0;
    }
  }

  for(i=inicioColunasPL; i<(fimColunasPL+1); i++){
    if( mat[ondec][n-1+i] < 0 )
      return i;
  }

  return 0;
}

// Nao tem mais como melhorar a PL. Isto tem diferente significado
// de acordo com o tipo da PL.
int AnalisaResultado(float** mat, int n, int m, int tipoPL){

  if(tipoPL == PL_AUXILIAR){

    //Verifica o valor objetivo
    if( mat[1][m+(3*n)] < 0 )
      return INVIAVEL;

    else{
      if(mat[1][m+(3*n)] == 0)
        return VIAVEL;
    }
  }else{

    if(tipoPL == PL_NORMAL)
      return OTIMA;
  }

}

// Percorre a coluna Ak para ver se a PL eh ilimitada
int ChecaSeEilimitada(int k, float** mat, int n){

  for(int i=2; i<(n+2); i++){
    if(mat[i][n-1+k] > 0)
      return NAO;
  }

  return SIM;
}

// Escolhe o elemento a ser pivoteado - retorna a linha de Ak
int EscolhePivot(int cn, float** mat, int n, int m){

  int linhaMin = INT_MAX;
  float min = FLT_MAX;

  for(int i=2; i<(n+2); i++){
    if( (mat[i][m+(3*n)] > 0) && (mat[i][n-1+cn] > 0) ){
      if( (mat[i][m+(3*n)] / mat[i][n-1+cn]) < min ){
        min = (mat[i][m+(3*n)] / mat[i][n-1+cn]);
        linhaMin = i-1; //linha de Ak
      }
    }
  }

  return linhaMin;
}

// Inclui novo elemento selecionado na base canônica
void TrocaBase(int* B, int cn, int linha){

  B[linha-1] = cn;

  return;
}

// Para colocar a PL em forma canonica de acordo com nova base:Ab = I e Cb = 0
void PivoteiaParaFormaCanonica(int cn, int linha, float** mat, int n, int m){

  int i,j;
  float multiplicador;

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

// Simplex
int Simplex(float** mat, int n, int m, int* B, int tipoPL){

  int cn;     // Indice teorico
  int linha;  // Indice teorico
  int ehIlimitada;

  // Entrou uma PL em formato canonico, Ab = I e Cb = 0, e base dela B.

  // 1. Procura Cn para melhorar a solucao
  cn = EncontraFormaDeOtimizar(mat, n, m, tipoPL);

  if (cn == 0){     //Nao tem mais como otimizar com o Simplex
    return ( AnalisaResultado(mat, n, m, tipoPL) );
  }
  else // Existe cn otimizavel, coluna salva na variavel cn
  {
    // 2. Checa se a PL eh ilimitada
    ehIlimitada = ChecaSeEilimitada(cn, mat, n);
    if(ehIlimitada == SIM){
      ImprimeResultadoFinal(ILIMITADA, mat, n, m, B, cn);
      return ILIMITADA;
    }

    // Caso nao seja ilimitada:
    linha = EscolhePivot(cn, mat, n, m);
    TrocaBase(B, cn, linha);

    PivoteiaParaFormaCanonica(cn, linha, mat, n, m);

    return ( Simplex( mat, n, m, B, tipoPL) );
  }
}

// Imprime o resultado do Simplex executado
void ImprimeResultadoFinal(int resultado, float** mat, int n, int m, int* B, int cn){

  int i, j;

  switch(resultado){

    case INVIAVEL:
      printf("inviavel\n");
      for(i=0; i<n; i++)
        printf("%.7f ", mat[1][i]);
      break;

    case ILIMITADA:
      printf("ilimitada\n");
      // Uma Solucao
      for(i=1; i<(m+1); i++){
        // se eh variavel basica
        for(j=1; j<(n+1); j++){
          if(i == B[j-1]){
            printf("%.7f ", mat[j+1][m+(3*n)]);
            break;
          }
        }
        // se eh variavel nao basica
        if(j == (n+1))
          printf("0.0000000 ");
      }
      printf("\n");
      // Certificado de Ilimitada:
      for(i=1; i<(m+1); i++){
        if(i == cn){
          printf("1.0000000 ");
        }
        else
        {
          // se eh variavel basica
          for(j=1; j<(n+1); j++){
            if(i == B[j-1]){
              printf("%.7f ", ((-1) * mat[j+1][n-1+cn]));
              break;
            }
          }
          // se eh variavel nao basica
          if(j == (n+1))
            printf("0.0000000 ");
        }
      }
      break;

    case OTIMA:
      printf("otima\n");
      // Valor Objetivo:
      printf("%.7f\n", mat[1][m+(3*n)]);
      // Uma Solucao Otima : solucao basica dela ----- sem as variaveis de folga?**********
      // Analisa cada coluna de A (eliminando as variaveis de folga)
      for(i=1; i<(m+1); i++){
        // Procura essa coluna na base
        for(j=1; j<(n+1); j++){
          if(B[j-1] == i){
            printf("%.7f ", mat[j+1][m+(3*n)]);
            break;
          }
        }
        // Se nao esta na base:
        if(j == (n+1))
          printf("0.0000000 ");
      }
      printf("\n");
      // Certificado de Otimalidade:
      for(i=0; i<n; i++)
        printf("%.7f ", mat[1][i]);
      break;
  }

  return;
}
