#include "indice.h"
#include "estructura.h"
#include "validaciones.h"

//toma memoria para 100 elementos e inicializa la estructura a índice vacío.
void indice_crear(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    indice->cap = nmemb;
    indice->cant = 0;
    indice->vindice = (void *)malloc(nmemb * tamanyo);
    if(indice->vindice == NULL)
    {
        printf("No se pudo reservar memoria");
        return;
    }
}

//redimensiona el tamaño del indice.
void indice_redimensionar(t_indice *indice, size_t nmemb, size_t tamanyo)
{
    size_t nuevoTotal = nmemb * INCREMENTO; //multiplica para darle mas de 30%
    void *nuevos = (void *)realloc(indice->vindice, nuevoTotal * tamanyo); //redimensiono vector
    if(!nuevos)
        return;
    indice->vindice = nuevos;
    indice->cap = nuevoTotal;
}

//inserta en orden según la clave.
int indice_insertar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    if(indice_lleno(indice))
        indice_redimensionar(indice, indice->cap, tamanyo);

    void *pos = indice->vindice;
    size_t i = 0;
    while(i < indice->cant && cmp(registro, pos) > 0)
    {
        pos = (char *)pos + tamanyo;
        i ++;
    }

    memmove((char *)pos + tamanyo, pos, (indice->cant - i) * tamanyo);
    memcpy(pos, registro, tamanyo);
    indice->cant ++;

    return OK;
}

//elimina el registro del indice.
int indice_eliminar(t_indice *indice, const void *registro, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int pos = indice_buscar(indice, registro, indice->cant, tamanyo, cmp);

    if(pos == NO_EXISTE)
    {
        printf("No se encontró el dni buscado");
        return ERROR;
    }

    char *base = (char *)indice->vindice;
    char *destino = base + pos * tamanyo;
    if (pos < indice->cant - 1)
        memmove(destino, destino + tamanyo, (indice->cant - pos - 1) * tamanyo);

    indice->cant --;

    return OK;
}

//indica se el índice está vacío.
int indice_vacio(const t_indice* indice)
{
    return indice->cant == 0;
}

//indica si ya no se pueden añadir más registros.
int indice_lleno(const t_indice *indice)
{
    return indice->cant == indice->cap;
}

//si la clave existe deja e registro en registro.
int indice_buscar (const t_indice *indice, const void *registro, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    int ini = 0, fin=indice->cant - 1, medio;

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

//libera la memoria utilizada por el índice.
void indice_vaciar(t_indice *indice)
{
    free(indice->vindice);
}

//carga el array desde un archivo ordenado.
int indice_cargar(const char *path, t_indice *indice, void *vreg_ind, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    FILE *arch = fopen(path, "rb");
    if(!arch)
        return ERROR;

    t_reg_indice reg_ind, reg_ind_buscar;
    unsigned nro_reg = 0;

    while (fread(vreg_ind, sizeof(Ssocio), 1, arch) == 1)
    {
        Ssocio *reg = (Ssocio*) vreg_ind;

        if (reg->estado == 'A')
        {
            reg_ind.dni = reg->dni;
            reg_ind.nro_reg = nro_reg;
            reg_ind_buscar.dni = reg->dni;

            if (indice_buscar(indice, &reg_ind_buscar, indice->cant, tamanyo, cmp) == NO_EXISTE)
                indice_insertar(indice, &reg_ind, tamanyo, cmp);
        }
        nro_reg++;
    }

    fclose(arch);
    return OK;
}

void ordenarIndice(void *vec, size_t nmemb, size_t tamanyo, int (*cmp)(const void *, const void *))
{
    for(int i = 0; i < nmemb; i++)
        for(int j = 0; j < nmemb - 1; j++)
            if(cmp(vec + j * tamanyo,  vec + (j + 1) * tamanyo) > 0)
                intercambiar(vec + j * tamanyo,  vec + (j + 1) * tamanyo, tamanyo);
}

void intercambiar(void *a, void *b, size_t tam)
{
    void *aux =malloc(tam);
    if(!aux)
        exit(1);

    memcpy(aux, a, tam);
    memcpy(a, b, tam);
    memcpy(b, aux, tam);

    free(aux);
}
