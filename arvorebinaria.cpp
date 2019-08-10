#include "arvorebinaria.h"
#include <iostream>

short insereElemento(Registro reg, FILE *arquivo) // Esse "arquivo" é o arquivo binário com a árvore, não o "registros.bin".
{
    RegistroBin temp; // Guarda temporariamente o registro escrito no arquivo binário.
    long pos = 1; // Guarda qual a posição do último elemento comparado.
    fseek(arquivo, 0, SEEK_END); // Vai pro fim do arquivo pra pegar a posição do último elemento.
    long final = ftell(arquivo)/ sizeof (RegistroBin); // Pega a quantidade de itens presente no arquivo.

    rewind(arquivo);
    while(!feof(arquivo)){
        if(fread(&temp, sizeof(RegistroBin), 1, arquivo) != 0){ // Caso tenha alguma coisa na árvore.
            if(reg.chave == temp.registro.chave) return 0; // Registro já existe na árvore!

            // Caso a chave seja maior que o registro atual
            else if (reg.chave > temp.registro.chave) {
                if(temp.dir == -1){ // Caso esse aqui seja o pai do elemento a ser inserido.
                    temp.dir = final; // Ponteiro da direita agora aponta pra última posição do arquivo.

                    // Preciso dar um fseek pra trás pra poder sobrescrever a linha que acabei de ler com temp.dir atualizado.
                    fseek(arquivo, -1*sizeof (RegistroBin), SEEK_CUR);
                    fwrite(&temp, sizeof (RegistroBin), 1, arquivo); // Sobreescrevo o registro com o ponteiro da direita atualizado.
                    fseek(arquivo, final*sizeof(RegistroBin), SEEK_SET); // Agora sim pode ir pro final.

                    // Cria um registro localmente e escreve ele no arquivo.
                    RegistroBin novo;
                    novo.registro = reg;
                    novo.dir = -1;
                    novo.esq = -1;
                    fwrite(&novo, sizeof (RegistroBin), 1, arquivo);
                    return 1;
                }
                else { // O elemento lido já tem um filho a direita.
                    pos = temp.dir;
                    fseek(arquivo, pos*sizeof (RegistroBin), SEEK_SET);
                }
            }

            // Caso a chave seja menor que o registro atual
            else {
                if(temp.esq == -1){
                    // Mesmo esquema da direita só que pra esquerda.
                    temp.esq = final; // Ponteiro da esquerda aponta pra última posição.
                    fseek(arquivo, -1*sizeof (RegistroBin), SEEK_CUR);
                    fwrite(&temp, sizeof (RegistroBin), 1, arquivo);
                    fseek(arquivo, final*sizeof(RegistroBin), SEEK_SET);
                    RegistroBin novo;
                    novo.registro = reg;
                    novo.dir = -1;
                    novo.esq = -1;
                    fwrite(&novo, sizeof (RegistroBin), 1, arquivo);
                    return 1; // Elemento inserido
                }
                else {
                    pos = temp.esq;
                    fseek(arquivo, pos*sizeof (RegistroBin), SEEK_SET);
                }
            }
        }
        else { // Primeiro elemento da árvore.
            RegistroBin novo;
            novo.registro = reg;
            novo.dir = -1;
            novo.esq = -1;
            fwrite(&novo, sizeof (RegistroBin), 1, arquivo);
            return 1; // Primeiro elemento inserido!
        }
    }
    return 0; // Deu tudo errado se chegar aqui.
}

short pesquisa(Registro* reg, FILE *arquivo, int *pos, Medida* medida)
{
    RegistroBin temp;
    rewind(arquivo);
    *pos = 1;

    while(!feof(arquivo)){
        fread(&temp, sizeof(RegistroBin), 1, arquivo);
        medida->transferencias++;

        medida->comparacoes++;
        if(reg->chave == temp.registro.chave) return 1; // Achamos o elemento!

        medida->comparacoes++;
        if (reg->chave > temp.registro.chave) {
            if(temp.dir == -1) return 0; // Não tem o registro no arquivo já que um elemento diferente não tem filho mais.
            *pos = temp.dir;
            fseek(arquivo, temp.dir * sizeof (RegistroBin), SEEK_SET);
            medida->transferencias++;
        }

        else {
            if(temp.esq == -1) return 0; // Mesma coisa de cima.
            *pos = temp.esq;
            fseek(arquivo, temp.esq * sizeof (RegistroBin), SEEK_SET);
            medida->transferencias++;
        }
    }
}
