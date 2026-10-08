#include "tda.h"


int tda_vec_CREAR(tda_vec* vec, size_t tam)
{
    vec->vec = malloc(tam * capacidad_inicial);
    if(!vec->vec)
        return err_memoria;
    vec->capacidad = capacidad_inicial;
    vec->ce = 0;
    vec->tam_elem = tam;
    return todo_ok;
}


void tda_vec_DESTRUIR(tda_vec* vec)
{
    if(vec == NULL)
        return;

    free(vec->vec);

    vec->vec = NULL;
    vec->ce = 0;
    vec->tam_elem = 0;
    vec->capacidad = 0;
}


int tda_vec_REDIMENSIONAR(tda_vec* vec)
{
    size_t newcap = vec->capacidad * factor_incremento;
    void* newdir = realloc(vec->vec, newcap * vec->tam_elem);
    if(!newdir)
        return err_memoria;
    vec->vec = newdir;
    vec->capacidad = newcap;
    return todo_ok;
}

void tda_vec_MOSTRAR(tda_vec* vec, void (*accion)(void* a))
{
    char* dirini = (char*)vec->vec;
    char* dirfin = dirini + (vec->ce * vec->tam_elem);
    for(char* i = dirini; i < dirfin; i += vec->tam_elem)
    {
        accion(i);
    }
}

/// METODOS
int tda_vec_insertar_ordenado(tda_vec* vec,
                              void* elem,
                              int (*cmp)(void* a, void* b))
{
    if(vec->ce == 0)
    {
        memcpy(vec->vec, elem, vec->tam_elem);
        vec->ce++;
        return todo_ok;
    }

    if(vec->ce == vec->capacidad)
    {
        int res = tda_vec_REDIMENSIONAR(vec);

        if(res != todo_ok)
            return res;
    }

    char* base = (char*)vec->vec;

    char* dirini = base;
    char* dirfin = base + (vec->ce - 1) * vec->tam_elem;

    while(dirini <= dirfin && cmp(elem, dirini) > 0)
    {
        dirini += vec->tam_elem;
    }

    memmove(dirini + vec->tam_elem,
            dirini,
            (vec->ce * vec->tam_elem) - (dirini - base));

    memcpy(dirini, elem, vec->tam_elem);

    vec->ce++;

    return todo_ok;
}

int tda_vec_insertar(tda_vec *v, const void *dato)
{
    void *aux;

    if(v->ce == v->capacidad)
    {
        aux = realloc(v->vec,
                     (v->capacidad + 100) * v->tam_elem);

        if(!aux)
            return 0;

        v->vec = aux;
        v->capacidad += 100;
    }

    memcpy((char*)v->vec + v->ce * v->tam_elem,
           dato,
           v->tam_elem);

    v->ce++;

    return 1;
}
