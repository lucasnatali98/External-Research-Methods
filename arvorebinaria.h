#ifndef ARVOREBINARIA_H
#define ARVOREBINARIA_H
#include "registro.h"
#include "medida.h"

typedef struct arvore {
    Registro registro;
    long esq, dir;
}RegistroBin;

short insereElemento(Registro, FILE*); // Insere elementos na ordem em que o vetor de registros está.
short pesquisa(Registro*, FILE*, int*, Medida*); // Encontra o item pesquisado caso exista.

#endif // ARVOREBINARIA_H
