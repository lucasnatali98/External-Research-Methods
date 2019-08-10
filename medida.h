#ifndef MEDIDA_H
#define MEDIDA_H

struct medida{
    int comparacoes; // Número de comparações entre os elementos.
    int transferencias; // Transferências entre a memória principal e secundária.
};

typedef struct medida Medida;

#endif // MEDIDA_H
