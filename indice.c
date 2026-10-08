#include "indice.h"
#include "estructura.h"
#include "tda.h"


//si la clave existe deja e registro en registro.
int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int ini = 0, fin=indice->cantidad_elementos_actual - 1, medio;

    while(ini <= fin)
    {
        medio = (ini + fin) / 2;

        void *elem_medio = (char *)indice->vindice + medio * tamanyo;
        int comp = cmp(registro, elem_medio);

        if (comp == 0)
            return medio;           // Encontrado
        else if (comp < 0)
            fin = medio - 1;        // Buscar en la mitad izquierda
        else
            ini = medio + 1;     // Buscar en la mitad derecha
    }

    return NO_EXISTE;
}


int indice_insertar(t_indice *indice, const void *registro, size_t tamanyo,
                    int (*cmp)(const void *, const void *))
{
    if(indice_lleno(indice))
        indice_redimensionar(indice, indice->cantidad_elementos_maxima, tamanyo);

    size_t i = 0;
    while(i < indice->cantidad_elementos_actual &&
            cmp(registro, (char*)indice->vindice + i * tamanyo) > 0)
    {
        i++;
    }

    void *pos = (char*)indice->vindice + i * tamanyo;

    memmove((char*)pos + tamanyo, pos,
            (indice->cantidad_elementos_actual - i) * tamanyo);

    memcpy(pos, registro, tamanyo);
    indice->cantidad_elementos_actual++;

    return OK;
}


//compara los indices.
int cmp_indice_p(const void *a, const void *b)
{
    const t_reg_indice_pelicula *ra = (const t_reg_indice_pelicula *)a;
    const t_reg_indice_pelicula *rb = (const t_reg_indice_pelicula *)b;
    if (ra->id_pelicula < rb->id_pelicula)
        return -1;
    if (ra->id_pelicula > rb->id_pelicula)
        return 1;
    return 0;
}

//indica se el �ndice est� vac�o.
int indice_vacio(const t_indice* indice)
{
    return indice->cantidad_elementos_actual == 0;
}

//indica si ya no se pueden a�adir m�s registros.
int indice_lleno(const t_indice *indice)
{
    return indice->cantidad_elementos_actual == indice->cantidad_elementos_maxima;
}


void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    size_t nuevoTotal = nmemb * INCREMENTO; //multiplica para darle mas de 30%
    void *nuevos = (void *)realloc(indice->vindice, nuevoTotal * tamanyo); //redimensiono vector
    if(!nuevos)
        return;
    indice->vindice = nuevos;
    indice->cantidad_elementos_maxima = nuevoTotal;
}


void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->cantidad_elementos_maxima = nmemb;
    indice->cantidad_elementos_actual = 0;
    indice->vindice = (void *)malloc(nmemb * tamanyo);
    if(indice->vindice == NULL)
    {
        printf("No se pudo reservar memoria");
        return;
    }
}

void indice_vaciar(t_indice *indice)
{
    free(indice->vindice);
}


/// Compara los indice
//compara los indices.
int cmp_indice(const void *a, const void *b)
{
    const t_reg_indice *ra = (const t_reg_indice *)a;
    const t_reg_indice *rb = (const t_reg_indice *)b;
    if (ra->dni < rb->dni)
        return -1;
    if (ra->dni > rb->dni)
        return 1;
    return 0;
}

//carga el array desde un archivo ordenado.
int indice_cargar(tda_vec * vec, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    Ssocio *inicio = (Ssocio*)vec->vec;
    Ssocio *fin = inicio + vec->ce;
    t_reg_indice reg_ind, reg_ind_buscar;
    unsigned nro_reg = 0;

    while (inicio<fin)
    {
        Ssocio *reg = inicio;

        if (reg->estado == 'A' || reg->estado == 'a')
        {
            reg_ind.dni = reg->dni;
            reg_ind.nro_reg = nro_reg;
            reg_ind_buscar.dni = reg->dni;

            if (indice_buscar(indice, &reg_ind_buscar, indice->cantidad_elementos_actual, tamanyo, cmp) == NO_EXISTE)
                indice_insertar(indice, &reg_ind, tamanyo, cmp);
        }
        inicio++;
        nro_reg++;
    }

    return OK;
}


int indice_cargar_AYN(tda_vec* vec, t_indice* indice, void* vreg_ind, size_t tamanyo, int (*cmp)(const void*, const void*))
{

    Ssocio *inicio = (Ssocio*)vec->vec;
    Ssocio *fin = inicio + vec->ce;

    t_reg_indice_ayn reg_ind, reg_ind_buscar;
    unsigned nro_reg = 0;
    Ssocio *reg= inicio;

    while (inicio < fin)
    {
        reg = inicio;

        if (reg->estado == 'A' || reg->estado == 'a')
        {
            strcpy(reg_ind.nyp, reg->nyp);
            reg_ind.nro_reg = nro_reg;

            strcpy(reg_ind_buscar.nyp, reg->nyp);


            if (indice_buscar(indice, &reg_ind_buscar, indice->cantidad_elementos_actual, tamanyo, cmp) == NO_EXISTE)
            {
                indice_insertar(indice, &reg_ind, tamanyo, cmp);
            }
        }
        inicio++;
        nro_reg++;
    }

    return OK;
}


void indice_destruir(t_indice *indice)
{
    if(indice == NULL)
        return;

    free(indice->vindice);

    indice->vindice = NULL;
    indice->cantidad_elementos_actual = 0;
    indice->cantidad_elementos_maxima = 0;
}


////
int indice_cargar_peliculas (tda_vec * vec, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    Spelicula *inicio = (Spelicula*)vec->vec;
    Spelicula *fin = inicio + vec->ce;
    t_reg_indice_pelicula reg_ind, reg_ind_buscar;
    unsigned nro_reg = 0;

    while (inicio<fin)
    {
        Spelicula *reg = inicio;

        if (reg->estado == 'A' || reg->estado == 'a' )
        {
            reg_ind.id_pelicula = reg->idPelicula;
            reg_ind.nro_reg = nro_reg;
            reg_ind_buscar.id_pelicula = reg->idPelicula;

            if (indice_buscar(indice, &reg_ind_buscar, indice->cantidad_elementos_actual, tamanyo, cmp) == NO_EXISTE)
                indice_insertar(indice, &reg_ind, tamanyo, cmp);
        }
        nro_reg++;
        inicio++;
    }

    return OK;
}


int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int pos = indice_buscar(indice, registro, indice->cantidad_elementos_actual, tamanyo, cmp);

    if(pos == NO_EXISTE)
    {
        printf("No se encontro el dni buscado");
        return ERROR;
    }

    char *base = (char *)indice->vindice;
    char *destino = base + pos * tamanyo;
    if (pos < indice->cantidad_elementos_actual - 1)
        memmove(destino, destino + tamanyo, (indice->cantidad_elementos_actual - pos - 1) * tamanyo);

    indice->cantidad_elementos_actual --;

    return OK;
}



int cmp_indice_AYN (const void *a, const void *b)
{
    const t_reg_indice_ayn *ra = (const  t_reg_indice_ayn *)a;
    const t_reg_indice_ayn *rb = (const t_reg_indice_ayn *)b;
    return strcmp (ra->nyp,rb->nyp);
}
