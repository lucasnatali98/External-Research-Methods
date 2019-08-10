#include "registro.h"
#include "medida.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cmath>
#include "acessoSequencial_indexado.h"
#include "arvorebinaria.h"
#include "arvoreb.h"
using namespace std;
#define TAM 100
struct pesquisa
{
    int metodo;
    int qtd;
    int situacao;
    int chave;
};

typedef struct pesquisa Pesquisa;

int main(int argc, char* argv[]) {

    using namespace std::chrono;
    // funções do menu
    if(argc < 5 || argc > 6){
        cout << "\n Entrada incorreta. Estrutura: \n \n pesquisa.exe <método> <quantidade> <situação> <chave> (opcional)<[-P]> " << endl;
        exit(1);
    }

    /*
     * --------------------------------------------------- Definição de Variáveis ----------------------------------------------------
     *
     * Setando as variáveis que vão ser usadas pelo programa.
     *
     * -------------------------------------------------------------------------------------------------------------------------------
    */

    //Váriaveis da linha de comando
    Pesquisa tipodeexecucao;
    tipodeexecucao.metodo = atoi(argv[1]);
    tipodeexecucao.qtd = atoi(argv[2]);
    tipodeexecucao.situacao = atoi(argv[3]);
    tipodeexecucao.chave = atoi(argv[4]);

    //Tipos para realização do acesso sequencial indexado
    long tamanho = tipodeexecucao.qtd/ITENSPAGINA + 1;
    int *tabela = new int[tamanho]; // A tabela tem as primeiras chaves de cada página, então tipo int.
    Registro *reg = new Registro[tipodeexecucao.qtd];

    // Setando o registro que vai ser pesquisado.
    Registro aux;

    aux.chave = tipodeexecucao.chave;
    cout<<"chave setada: "<<aux.chave<<endl;

    TipoApontador arv;

    FILE *arquivo = nullptr; // Futuro "registros.bin".

    Medida medida; // Calcula a quantidade de transferências e comparações.
    medida.comparacoes = 0;
    medida.transferencias = 0;

    // fim da Definição de Variáveis.

    /*
     * ------------------------------------------------- Manipulação dos Registros -------------------------------------------------
     *
     * Escreve os arquivos baseado no argumento 3 - situação do arquivo.
     *
     * -> case 1: O arquivo "registros.bin" vai ser escrito ordenado começando da chave 1.
     * -> case 2: O arquivo "registros.bin" vai ser escrito ordenado inversamente começando da chave "tipodeexecução.qtd".
     * -> case 3: O arquivo "registros.bin" vai ser escrito sem ordenação, com os registros aleatórios.
     *
     * - Não tem repetição de elementos (chaves) em nenhum dos casos.
     *
     * -----------------------------------------------------------------------------------------------------------------------------
    */

    switch(tipodeexecucao.situacao){
    case 1:
        escreveRegistros(tipodeexecucao.qtd,arquivo);
        leituraRegistros(reg,arquivo);
        break;
    case 2:
        escreveRegistrosDescendente(tipodeexecucao.qtd, arquivo);
        leituraRegistros(reg,arquivo);
        break;
    case 3:
        escreveRegistrosAleatorio(tipodeexecucao.qtd, arquivo);
        leituraRegistros(reg,arquivo);
        break;
    default:
        exit(1);
    }
     cout<<endl;


    // fim da Manipulação dos Registros.

     /*
      * ---------------------------------------------- Seleção dos Métodos de Pesquisa ----------------------------------------------
      *
      * Escolhe qual dos métodos de pesquisa vai ser utilizado dentre os quatro, baseado no argumento 1 - método de pesquisa.
      *
      * -> case 1: Acesso sequencial indexado. (*)
      * -> case 2: Árvore binária em arquivo. (**)
      * -> case 3: Árvore B.
      * -> case 4: Árvore B-estrela.
      *
      * - Os métodos pesquisam baseados no arquivo "registros.bin".
      * - A chave que vai ser pesquisada é baseada no argumento 4 - chave de pesquisa.
      * - As funções de pesquisa retornam '1' caso o item tenha sido encontrado e '0' caso não exista o registro no arquivo.
      * - O asi só serve pra arquivos ordenados.
      *
      * (*) - A tabela de índices foi criada no main e é preenchida no "acessoSequencial_indexado.cpp".
      * (**) - No segundo método é criado um novo arquivo chamado "arvore.bin" que guarda os registros. representado por "arqbin".
      *
      * -----------------------------------------------------------------------------------------------------------------------------
     */

    switch (tipodeexecucao.metodo) {
    case 1:
    {

         cout << "\n ------------------------ Acesso Sequencial Indexado ------------------------ \n";

         geraTabelaIndices(tabela);
         cout << "\n Tabela de Indices -----------> \n|";
         for (int i = 0; i < tamanho; ++i) {
             cout << tabela[i] << "|";
         }
         cout << endl << endl;

         clock_t tempoInicial = clock();
         high_resolution_clock::time_point t1 = high_resolution_clock::now();
         if(pesquisa(tabela, arquivo, tamanho, &aux, &medida) == 1)
         {
             cout<<"Chave encontrada"<<endl;
         }
         else
         {
             cout<<"Chave nao encontrada"<<endl;
         }
         clock_t tempoFinal = clock();
         high_resolution_clock::time_point t2 = high_resolution_clock::now();
         duration<double> total_time = duration_cast<duration<double>>(t2-t1);
         cout << "\n -> Tempo: " << setprecision(6) << fixed << ((double)(tempoFinal-tempoInicial)/CLOCKS_PER_SEC) << " segundos! " << endl;
         cout << "\n -> Transferencias: " << medida.transferencias << "\n -> Comparacoes: " << medida.comparacoes << endl;
         cout<<" Tempo usando a chrono: "<<total_time.count()<<" Seconds!"<<endl;
    }
    break;

    case 2:
    {
        cout << "\n ------------------------ Árvore binária em arquivo ------------------------" << endl;

        FILE* arqbin;
        arqbin = fopen("arvore.bin", "w+b");

        // inserindo os elementos no arquivo binário
        for (int i = 0;i < tipodeexecucao.qtd; i++) {
            if(insereElemento(reg[i], arqbin)){
                cout << "Elemento " << i+1 << " inserido! " << endl;
            }
        }
        RegistroBin a;
        rewind(arqbin);
        int i = 0;
        while(fread(&a, sizeof (RegistroBin), 1, arqbin) == 1){
            cout << "\n linha " << i << ": " << a.esq << " " << a.registro.chave << " " << a.dir; i++;
        }
        cout << endl << endl;

        // Pesquisando por uma chave específica
        int pos; // Retorna a posição que a chave estava no arquivo.
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        clock_t tempoInicial = clock();
        if(pesquisa(&aux, arqbin, &pos, &medida)) cout << "\n Achou a chave " << aux.chave << " na posicao: " << pos << endl;
        else cout << "\n Chave " << aux.chave << " nao encontrada..." << endl;
        clock_t tempoFinal = clock();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> total_time = duration_cast<duration<double>>(t2-t1);
        cout << "\n -> Tempo: " << setprecision(6) << fixed << ((double)(tempoFinal-tempoInicial)/CLOCKS_PER_SEC) << " segundos! " << endl;
        cout << "\n -> Transferencias: " << medida.transferencias << "\n -> Comparacoes: " << medida.comparacoes << endl;
        cout<<" Tempo usando a chrono: "<<total_time.count()<<" Seconds!"<<endl;
        fclose(arqbin);
    }
    break;

    case 3:
    {
        cout << "\n ------------------------ Arvore B ------------------------ "<<endl;
        arv = Inicializa();
        cout<<"Inicializou"<<endl;
        //tempo_Inicial= clock();

        for(int i=0;i< tipodeexecucao.qtd; i++){
           Insere(reg[i],&arv);
        }


        cout<<"Inseriu"<<endl;

        clock_t tempoInicial = clock();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        if(Pesquisar(&aux, arv, &medida)==1)
          cout << "Chave de pesquisa desejada foi encontrada." << endl;
        else
        {
          cout << "Chave de pesquisa desejada nao foi encontrada."<< endl;
        }
        clock_t tempoFinal = clock();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> total_time = duration_cast<duration<double>>(t2-t1);

        cout << "\n -> Tempo: " << setprecision(6) << fixed <<((long double)(tempoFinal-tempoInicial)/CLOCKS_PER_SEC) << " segundos! " << endl;
        cout << "\n -> Transferencias: " << medida.transferencias << "\n -> Comparacoes: " << medida.comparacoes << endl;
        cout<<" Tempo usando a chrono: "<<total_time.count()<<" Seconds!"<<endl;

    break;
    }
    case 4:
    {
        cout << "\n ------------------------ árvore B-Estrela ------------------------ ";
        break;
    }

    default:
        exit(1);
    }
    // fim da Seleção dos Métodos de Pesquisa

//    tempoFinal = clock() - tempoInicial;

//    cout<<"Tempo de execucao: "<< (static_cast<float>(tempoFinal/CLOCKS_PER_SEC))<<" segundos"<<endl;

    fclose(arquivo);
    delete[] reg;
    delete [] tabela;

    cout << endl << endl;

    return 0;
}

