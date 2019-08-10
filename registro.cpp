#include "registro.h"
#include <ctime>
#include <cstring>

bool escreveRegistros(int tam, FILE* arquivo)
{
    if((arquivo = fopen("registros.bin", "w+b"))== nullptr)
    {
        cout<<"O arquivo não pode ser aberto"<<endl;
        exit(1);
    }
    Registro r;
    srand(static_cast<unsigned int>(time(nullptr)));
    char nome[500];
    for (int i = 0; i < tam; i++) {
        r.chave = i+1;
        r.dado1 = rand() % 1000000;
        for (int j = 0; j < 499; j++) {
           nome[j] = (rand()%26)+65;
        }
        nome[499] = '\0';
        strcpy(r.dado2, nome);
        fwrite(&r, sizeof(r), 1, arquivo);
    }
    fclose(arquivo);
    return true;
}


bool escreveRegistrosAleatorio(int tam, FILE *arquivo)
{
    Registro* reg = new Registro[tam];
    if((arquivo = fopen("registros.bin", "w+b"))== nullptr)
    {
        cout<<"O arquivo não pode ser aberto"<<endl;
        exit(1);
    }

    // Escreve ordenado.
    srand(static_cast<unsigned int>(time(nullptr)));
    char nome[500];
    cout<<"---- ESCRITA DE DADOS -------"<<endl;
    for (int i = 0; i < tam; i++) {
        reg[i].chave = i+1;
        reg[i].dado1 = rand() % 1000000;
        for (int i = 0; i < 499; i++){
            nome[i] = (rand()%26)+65;
        }
        nome[499] = '\0';
        strcpy(reg[i].dado2, nome);
    }

    // troca as posições dos elementos em pos1 e pos2.
    for (int i = 0; i < tam*2; ++i) {
        int pos1 = rand() % tam;
        int pos2 = rand() % tam;
        Registro aux = reg[pos1];
        reg[pos1] = reg[pos2];
        reg[pos2] = aux;
    }

    // Escreve no arquivo.
    for (int i = 0; i < tam; ++i) {
        fwrite(&reg[i], sizeof(Registro),1,arquivo);
    }
    cout << endl;

    delete [] reg;
    fclose(arquivo);
    return true;
}


bool escreveRegistrosDescendente(int tam, FILE *arquivo)
{
    if((arquivo = fopen("registros.bin", "w+b"))== nullptr)
    {
        cout<<"O arquivo não pode ser aberto"<<endl;
        exit(1);
    }
    Registro r;
    srand(static_cast<unsigned int>(time(nullptr)));
    char nome[500];
    cout<<"----- ESCRITA DE DADOS DESCENDENTEMENTE -------"<<endl;
    for (int i = 0; i < tam; i++) {

        r.chave = tam - i;
        r.dado1 = rand() % 1000000;

        for (int i = 0; i < 499; i++) {
            nome[i] = (rand()%26)+65;
        }
        nome[499] = '\0';
        strcpy(r.dado2, nome);

        fwrite(&r, sizeof(Registro),1,arquivo);
    }
    cout << endl;

    fclose(arquivo);
    return true;
}


bool leituraRegistros(Registro *reg, FILE* arquivo)
{
    Registro r;
    cout<<"--- LEITURA DE DADOS ------"<<endl;
    if((arquivo = fopen("registros.bin", "rb"))== nullptr)
    {
        cout<<"O arquivo não pode ser aberto"<<endl;
        exit(1);
    }
    int i=0;
    while(fread(&r, sizeof(Registro),1,arquivo)==1)
    {
//        cout<<"chave: "<<r.chave<<endl;
//        cout<<"dado1: "<<r.dado1<<endl;
//        cout<<"dado2: "<<r.dado2<<endl;
//        cout<<endl;
        reg[i] = r;
        i++;
    }
    fclose(arquivo);
    return true;
}
bool pesquisaRegistro(Registro *reg, int chave, int tam, FILE* arquivo)
{
    cout<<"Pesquisa"<<endl;
    Registro *aux = new Registro;
    if((arquivo = fopen("registros.bin", "rb"))== nullptr)
    {
        cout<<"O arquivo não pode ser aberto"<<endl;
        exit(1);
    }
    int flag=0;
    for (int i = 0; i < tam; ++i) {
        if(reg[i].chave == chave){

            (*aux) = reg[i];
            cout<<"Chave: "<<aux->chave<<endl;
            cout<<"Dado1: "<<aux->dado1<<endl;
            cout<<"Dado2: "<<aux->dado2<<endl;
            flag = 1;
            i = tam-1;
        }
    }
    if(flag == 0){
        cout<<"A leitura não funcionou"<<endl;
    }

    return true;
}
