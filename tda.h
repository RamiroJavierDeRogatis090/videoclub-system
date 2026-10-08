#ifndef TDA_H_INCLUDED
#define TDA_H_INCLUDED
#include "macros.h"




typedef struct
{
    void* vec;
    size_t ce;
    size_t tam_elem;
    size_t capacidad;
} tda_vec;

int tda_vec_CREAR(tda_vec* vec, size_t tam);
void tda_vec_DESTRUIR(tda_vec* vec);
int tda_vec_REDIMENSIONAR(tda_vec* vec);
void tda_vec_MOSTRAR(tda_vec* vec, void (*accion)(void* a));
int tda_vec_insertar_ordenado(tda_vec* vec, void* elem, int (*cmp)(void* a, void* b));
int tda_vec_insertar(tda_vec *v, const void *dato);



#endif // TDA_H_INCLUDED
