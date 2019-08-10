#ifndef ARVOREB_H
#define ARVOREB_H

#include "registro.h"
#include "medida.h"
const int M = 4;

typedef long TipoChave;

typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina {
    short n;
    Registro r[2*M];
    TipoApontador p[2*M + 1];
} TipoPagina;


TipoApontador Inicializa ();
bool Pesquisar (Registro *x , TipoApontador Ap, Medida* medida);
void InsereNaPagina(TipoApontador Ap, Registro Reg, TipoApontador Apdir);
void Ins(Registro Reg, TipoApontador Ap, short *Cresceu, Registro RegRetorno, TipoApontador *ApRetorno);
bool Insere(Registro Reg,TipoApontador *Ap);



#endif // ARVOREB_H
