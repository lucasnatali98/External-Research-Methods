#include "arvorebestrela.h"

/*
    Não organizei esse código pq ele ainda tá incompleto!
*/

TipoApontador criaArvoreBEstrela()
{
    TipoApontador arvore = new Pagina;
    return arvore;
}

void bEstrelaInsere(Registro Reg, TipoApontador *Ap)
{
    short Cresceu;
    Registro RegRetorno;
    Pagina *ApRetorno, *ApTemp;
    bEstrelaIns(Reg,*Ap,&Cresceu,&RegRetorno,&ApRetorno);
    cout<<"Passou da chamada recursiva"<<endl;
    if(Cresceu)
     {
         cout<<"Cresceu"<<endl;
         ApTemp = (Pagina*)malloc(sizeof(Pagina));
         ApTemp->tipoDePagina = folha;
         ApTemp->unionPagina.PaginaFolha.numRegistros=1;
         ApTemp->unionPagina.PaginaFolha.registros[0]= RegRetorno;

         // Chamar Insere na Folha
         *Ap = ApTemp;
         cout<<"Atribuiu tudo -- Insere"<<endl;
     }
    else {
        if((*Ap)->tipoDePagina == folha){
            // chamar b estrela ins
        }
    }
}

void bEstrelaInsereNaPagina(TipoApontador Ap, Registro Reg, TipoApontador Apdir)
{
    short NaoAchouPosicao;
    int k;
    k = Ap->unionPagina.PaginaIndice.numIndices;
    cout<<"Valor de K -- Insere na Pagina: "<<k<<endl;
    NaoAchouPosicao = (k>0);
    while (NaoAchouPosicao)
    {
            cout<<"Entrou no While do InsereNaPagina -- Funcao: InsereNaPagina"<<endl;
            if(Reg.chave >= Ap->unionPagina.PaginaIndice.chaves[k-1])
            {
                 cout<<"Reg.chave = "<<Reg.chave<<endl;
                 NaoAchouPosicao = false;
                 cout<<"If: Reg.chave >= Ap->r[k-1].chave"<<endl;
                break;
            }

            Ap->unionPagina.PaginaIndice.chaves[k] = Ap->unionPagina.PaginaIndice.chaves[k-1];
            Ap->unionPagina.PaginaIndice.ponteiros[k+1]= Ap->unionPagina.PaginaIndice.ponteiros[k];
            k--;

            if(k<1)
            {
               NaoAchouPosicao = false;
               cout<<"if: k<1 -- Funcao: InsereNaPagina"<<endl;
            }
    }
    Ap->unionPagina.PaginaIndice.chaves[k] = Reg.chave;
    Ap->unionPagina.PaginaIndice.ponteiros[k+1] = Apdir;
    Ap->unionPagina.PaginaIndice.numIndices++;
    cout<<"Terminou a funcao: InsereNaPagina"<<endl;
}

void bEstrelaIns(Registro reg, TipoApontador Ap, short *cresceu, Registro *regRetorno, TipoApontador *ApRetorno)
{

}
