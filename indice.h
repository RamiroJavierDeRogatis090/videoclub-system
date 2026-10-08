#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED
#include "macros.h"
#include "tda.h"


typedef struct
{
    unsigned nro_reg;
    long dni;
} t_reg_indice;



typedef struct
{
    char nyp[61];
    int nro_reg;
} t_reg_indice_ayn;


/*agregado por nosotros para validar pelicula*/
typedef struct
{
    unsigned nro_reg;
    int id_pelicula;
} t_reg_indice_pelicula;


typedef struct
{
    void *vindice;
    unsigned cantidad_elementos_actual;
    unsigned cantidad_elementos_maxima;
} t_indice;


void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo);
int indice_insertar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *));
int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *));
void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo);
void indice_vaciar(t_indice *indice);
int indice_lleno(const t_indice *indice);
int indice_cargar(tda_vec * vec, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *));
int indice_cargar_peliculas (tda_vec * vec, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *));
int cmp_indice(const void *a, const void *b);
int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *));
int cmp_indice_AYN (const void *a, const void *b);
int indice_cargar_AYN(tda_vec * vec, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *));
void indice_destruir(t_indice *indice);
void ordenarIndice(void *vec, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *));


/// Funcion de comparacion para auto INCREMENTAL ///
int cmp_indice_p(const void *a, const void *b);


#endif // INDICE_H_INCLUDED
