#include <stdio.h>
#include <stdlib.h>

//Assinaturas das funcoes aqui acho: testar
int EpossivelMelhorar (float** eVetorCusto, int** eNaoBase, int numVariaveis);

int main()
{
  int i,j;
  int indiceid;

  int numRestricoes;
  int numVariaveis;
  float* vetorCusto;
  float** ab;
  int* base;
  int* naobase;

  // 1 - Leitura dos dados e PL em FPI
  scanf("%d %d\n", &numRestricoes, &numVariaveis);

  //Vetor Custo, com valor objetivo da PL no final
  vetorCusto = (float*) malloc((numVariaveis+numRestricoes+1)*(sizeof(float)));
  for(i=0; i<numVariaveis; i++){
    scanf("%f ", &(vetorCusto[i]));
  }
  for(i=numVariaveis; i<(numVariaveis+numRestricoes); i++){
    vetorCusto[i] = 0;
  }
  vetorCusto[numVariaveis+numRestricoes] = 0;

  //Matriz Ab, ja construida incluindo as variaveis de folga
  indiceid = numVariaveis;
  ab = (float**) malloc(numRestricoes*sizeof(float*));
  for(i=0; i<numRestricoes; i++){
    ab[i] = (float*) malloc((numRestricoes+numVariaveis+1)*sizeof(float));

    for(j=0; j<numVariaveis; j++){
      scanf("%f ", &(ab[i][j]));
    }

    for(j=numVariaveis; j<(numVariaveis+numRestricoes); j++){
      if(j != indiceid){
        ab[i][j] = 0;
      }else{
        ab[i][j] = 1;
      }
    }
    indiceid++;

    scanf("%f ", &ab[i][numVariaveis+numRestricoes]);
  }

  // ****** CUIDADO COM ERRO
  // 2 - Colocar a PL em forma canonica
  // Ja esta: so definir a base e nao base aqui
  base = (int*) malloc(numRestricoes*sizeof(int));
  for(i=0; i<numRestricoes; i++){
    base[i] = numVariaveis + i;
  }

  naobase = (int*) malloc(numVariaveis*sizeof(int));
  for(i=0; i<numVariaveis; i++){
    naobase[i] = i;
  }

  // 4 - Ve se tem como melhorar a solucao basica: via funcao
  // (funcao porque vai repetir infinitamente)
  int EpossivelMelhorar(&vetorCusto, &naobase, numVariaveis);


  //Impressao ao fim: numeros devem ser escritos com no maximo
  // 7 casas decimais
  //Ao fim:desalocar estruturas como vetor custo, ab...
  return 0;
}

// FUNCOES

// PASSO 4
int EpossivelMelhorar (float** eVetorCusto, int** eNaoBase, int numVariaveis)
{
  int i;

  // Percorre Cn
  for(i=0; i<numVariaveis; i++){
    if( (*eVetorCusto)[(*eNaoBase)[i]] > 0 ){
      return ((*eNaoBase)[i]);
    }
  }

  return (-1);
}
