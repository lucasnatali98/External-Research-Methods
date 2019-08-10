#include "arvoreb.h"
#include <iostream>
#include "registro.h"
using namespace std;


TipoApontador Inicializa ()
{
    TipoApontador Arvore = nullptr;
    return Arvore;
}

bool Pesquisar(Registro *x , TipoApontador Ap, Medida* medida){
    long i = 1;

    if(Ap == nullptr){
        cout<< " registro nao esta presente na arvore"<< endl;
        return 0;
    }
    medida->comparacoes++;
    while ( i< Ap->n && x->chave > Ap->r[i-1].chave){
        i++;
        medida->comparacoes++;
    }

    medida->comparacoes++;
    if(x->chave == Ap->r[i-1].chave){
       *x= Ap->r[i-1];
       cout<< "o registro encontrado eh" << x->chave << endl;
       return 1;
    }

    medida->comparacoes++;
    if(x->chave < Ap->r[i-1].chave){
        Pesquisar(x,Ap->p[i-1], medida);
    }
    else{
        Pesquisar(x,Ap->p[i], medida);
    }

    return true;

}

void InsereNaPagina(TipoApontador Ap, Registro Reg, TipoApontador Apdir){

    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    cout<<"Valor de K -- Insere na Pagina: "<<k<<endl;
    NaoAchouPosicao = (k>0);
    while (NaoAchouPosicao)
    {
            if(Reg.chave >= Ap->r[k-1].chave)
            {
                 NaoAchouPosicao = false;
                break;
            }

            Ap->r[k] = Ap->r[k-1];
            Ap->p[k+1]= Ap->p[k];
            k--;

            if(k<1)
            {
               NaoAchouPosicao = false;
            }
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = Apdir;
    Ap->n++;
 }

void Ins(Registro Reg, TipoApontador Ap, short *Cresceu, Registro *RegRetorno, TipoApontador *ApRetorno){
     long i =1;
     long j;
     TipoApontador ApTemp;
     if(Ap == nullptr) // caso esteja em um nodo folha
      {

         *Cresceu = true;
         (*RegRetorno)= Reg;    //olhar dps
         (*ApRetorno) = nullptr;
         return;
      }

     while(i < Ap->n && Reg.chave > Ap->r[i-1].chave){
         i++;
     }

     if(Reg.chave == Ap->r[i-1].chave)
     {
         cout<<"O registro já está presente "<<endl;
         *Cresceu = false;
         return;
     }

     if(Reg.chave < Ap->r[i-1].chave) //compara a chave com a anterior da pagina
     {
         i--;
     }

     cout<<endl;
    Ins(Reg,Ap->p[i],Cresceu,RegRetorno,ApRetorno);
     if(!(*Cresceu)) return;
     if(Ap->n < 2*M) /*pagina tem espaco*/
     {
         InsereNaPagina(Ap,*RegRetorno, *ApRetorno);
         *Cresceu = false;
         return;
     }
     /* Overflow: Pagina tem que ser dividida */
     ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
     ApTemp->n=0;
     ApTemp->p[0] = nullptr;

     if(i < M+1)
     {
        InsereNaPagina(ApTemp,Ap->r[(2*M)-1],Ap->p[2*M]);
        Ap->n--;
        InsereNaPagina(Ap,*RegRetorno,*ApRetorno);
     }
     else
     {
        InsereNaPagina(ApTemp,*RegRetorno,*ApRetorno);
     }

    for(j = M + 2; j<= 2*M; j++)
    {
        InsereNaPagina(ApTemp,Ap->r[j-1],Ap->p[j]);
    }

    Ap->n= M;
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
 }

bool Insere(Registro Reg,TipoApontador *Ap){ // Olhar se esse Reg precisa ser um ponteiro;

     short Cresceu;
     Registro RegRetorno;
     TipoPagina *ApRetorno, *ApTemp;
     Ins(Reg,*Ap,&Cresceu,&RegRetorno,&ApRetorno);
     if(Cresceu)
      {
          ApTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
          ApTemp->n=1;
          ApTemp->r[0]= RegRetorno;
          ApTemp->p[1]= ApRetorno;
          ApTemp->p[0] = *Ap;
          *Ap = ApTemp;
          return true;
      }
    return false;
}
