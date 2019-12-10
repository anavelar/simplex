#ifndef BIBSIMPLEX
#define BIBSIMPLEX

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

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
void ImprimeMatriz(float **mat, int n, int m);
void ImprimeBaseCanonica(int* B, int n);

// --- Reais
void PreencheMatriz(float** mat, int n, int m);
void EliminaBnegativos(float** mat, int n, int m);
// Simplex:
void PreparaPLParaSimplex(float** mat, int n, int m, int* B, int tipoPL);
int EncontraFormaDeOtimizar(float** mat, int n, int m, int tipoPL);
int AnalisaResultado(float** mat, int n, int m, int tipoPL);
int ChecaSeEilimitada(int k, float** mat, int n);
int EscolhePivot(int cn, float** mat, int n, int m);
void TrocaBase(int* B, int cn, int linha);
void PivoteiaParaFormaCanonica(int cn, int linha, float** mat, int n, int m);
int Simplex(float** mat, int n, int m, int* B, int tipoPL);
// Resultado Final
void ImprimeResultadoFinal(int resultado, float** mat, int n, int m, int* B, int cn);

#endif
