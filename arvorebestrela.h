#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H

#include "registro.h"

#define ITENS 4

typedef enum tipos{indice, folha} TipodePagina;
typedef struct pagina Pagina;
typedef struct pagina* TipoApontador;

struct pagina{
    TipodePagina tipoDePagina;
    union{
        struct{
            int numIndices;
            int chaves[ITENS];
            Pagina* ponteiros[ITENS+1];
        }PaginaIndice;
        struct{
            int numRegistros;
            Registro registros[ITENS];
        }PaginaFolha;
    }unionPagina;
};

TipoApontador criaArvoreBEstrela();
void bEstrelaInsere(Registro reg, TipoApontador *Ap);
void bEstrelaIns(Registro reg, TipoApontador Ap, short *cresceu, Registro *regRetorno, TipoApontador *ApRetorno);
void bEstrelaInsereNaPagina(TipoApontador Ap, Registro reg, TipoApontador apDir);

#endif // ARVOREBESTRELA_H
