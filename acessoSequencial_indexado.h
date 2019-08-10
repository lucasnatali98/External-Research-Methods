#ifndef ACESSOSEQUENCIALINDEXADO_H
#define ACESSOSEQUENCIALINDEXADO_H
#include "registro.h"
#include "medida.h"
#define ITENSPAGINA 16

short geraTabelaIndices(int* tabela); // A tabela é um vetor de inteiros com as chaves das primeiras páginas alocado dinâmicamente.
short pesquisa(int* tabela, FILE* arquivo, int tam, Registro* r, Medida* medida); // tam = ( tipodeexecucao.qtd / ITENSPAGINA ).
short pesquisaBinaria(Registro* pagina, Registro* r, int tamanho, Medida* medida); // pesquisa binária.

#endif // ACESSOSEQUENCIALINDEXADO_H
