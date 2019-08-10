#ifndef REGISTRO_H
#define REGISTRO_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

typedef struct registro {
    int chave;
    long int dado1;
    char dado2[500];
}Registro;

bool leituraRegistros(Registro *reg, FILE* arquivo);
bool escreveRegistros(int tam, FILE* arquivo);
bool escreveRegistrosAleatorio(int tam, FILE* arquivo);
bool escreveRegistrosDescendente(int tam, FILE* arquivo);
bool pesquisaRegistro(Registro *reg, int chave, int tam, FILE* arquivo);

#endif // REGISTRO_H
