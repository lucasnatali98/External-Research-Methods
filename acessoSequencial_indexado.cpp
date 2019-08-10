#include "acessoSequencial_indexado.h"
#include "registro.h"
#include <iostream>


// Gera a tabela de índices pro acesso sequencial indexado.
short geraTabelaIndices(int *tabela)
{
    FILE* arquivo = fopen("registros.bin", "rb");
    Registro r;
    int posicao = 0; // Índice da tabela que indica também a posição no arquivo daquela página.
    while((fread(&r, sizeof(Registro), 1, arquivo)) == 1){
        tabela[posicao] = r.chave;
        fseek(arquivo, ((ITENSPAGINA-1)*sizeof (Registro)), SEEK_CUR); // Procura a próxima chave pra inserir na tabela.
        posicao++;
    }
    fclose(arquivo);
    return 1;
}

// Retorna 1 se a pesquisa funcionar, 0 se não encontrar o item. (tam = tipodeexecucao.qtd / ITENSPAGINA).
short pesquisa(int *tabela, FILE *arquivo, int tam, Registro *r, Medida* medida)
{
    if((arquivo = fopen("registros.bin","r+b")) == nullptr)
    {
        cout << "\n Erro ao abrir o arquivo...";
        fclose(arquivo);
        return 0;
    }

    int qtditens; // Define a quantidade de itens na página pesquisada.
    int i = 0;

    if(tabela[i] > r->chave) return 0; // Item pesquisado é menor que a primeira chave, logo não está no arquivo.
    i++;

    while(i < tam && tabela[i] <= r->chave) i++; // Procura qual a página que o item está.

    if(i < tam) qtditens = ITENSPAGINA; // Não é a última página então tem 4 itens
    else{ // Última página
        medida->transferencias++;
        fseek(arquivo, 0, SEEK_END); // Se for a última página
        qtditens = (ftell(arquivo)/sizeof (Registro)) % ITENSPAGINA; // Calcula qtos itens tem na página caso tenha menos que o máximo
        medida->transferencias++;
        if(qtditens == 0) qtditens = ITENSPAGINA; // Se tiver o máximo, qtd itens vai ser 0. Muda pra o máximo quando isso acontece.
    }

    Registro* pagina = new Registro[qtditens];

    long deslocamento = ((i-1) * ITENSPAGINA * sizeof (Registro));
    medida->transferencias++;
    fseek(arquivo, deslocamento, SEEK_SET); // O deslocamento é em bytes (2^n).
    medida->transferencias++;
    fread(pagina, sizeof (Registro), qtditens, arquivo);

    /* pesquisa sequencial - Não apaguei. Vou deixar comentado caso precise.
     *
     * for (int i = 0; i < qtditens; ++i) {
     *     cout << pagina[i].chave << endl;
     *      if(pagina[i].chave == r->chave) {fclose(arquivo); delete[] pagina; return 1;}
     *  }
     *
    */

    // Pesquisa binária.
    if(pesquisaBinaria(pagina, r, qtditens, medida) == 1){
        delete [] pagina; return 1;
    }
    else {
        fclose(arquivo); delete [] pagina; return 0;
    }

}

short pesquisaBinaria(Registro* pagina, Registro* r, int tamanho, Medida* medida)
{
    int limiteInferior = 0;
    int limiteSuperior = tamanho-1;
    int metade;
    cout << "\n Pagina ---------> \n ";
    for (int i = 0; i < tamanho; ++i) {
        cout << "|" << pagina[i].chave;
    }
    cout << "\n";
    while(limiteInferior <= limiteSuperior){
        metade = (limiteInferior + limiteSuperior)/2;

        cout << "meio: " << metade << endl;
        cout << "valor:" << pagina[metade].chave << endl << endl;

        medida->comparacoes++;
        if(r->chave == pagina[metade].chave) return 1;
        medida->comparacoes++;
        if(r->chave < pagina[metade].chave) limiteSuperior = metade-1;
        else limiteInferior = metade+1;
    }

    return 0;
}
