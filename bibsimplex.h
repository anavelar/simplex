#ifndef BIBSIMPLEX
#define BIBSIMPLEX

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NAO 0
#define SIM 1

#define PL_AUXILIAR 0
#define PL_NORMAL 1

#define INVIAVEL 0
#define VIAVEL 1
#define ILIMITADA 2
#define OTIMA 3

// Funcoes

// --- Testes
void ImprimeMatriz(int **mat, int n, int m);
void ImprimeBaseCanonica(int* B, int n);

// --- Reais
void PreencheMatriz(int** mat, int n, int m);
void EliminaBnegativos(int** mat, int n, int m);
// Simplex na PL Auxiliar:
void PreparaPLParaSimplex(int** mat, int n, int m, int* B, int tipoPL);
int EncontraFormaDeOtimizar(int** mat, int n, int m, int tipoPL);
int ChecaSeEilimitada(int k, int** mat, int n);
int EscolhePivot(int cn, int** mat, int n, int m);
void TrocaBase(int* B, int cn, int linha);
void PivoteiaParaFormaCanonica(int cn, int linha, int** mat, int n, int m);
int Simplex(int** mat, int n, int m, int* B, int tipoPL);
// Resultado Final
void ImprimeResultadoFinal(int resultado, int** mat, int n);

#endif
