#ifndef ESTRUCTURA_H_INCLUDED
#define ESTRUCTURA_H_INCLUDED


typedef struct
{
    int d;
    int m;
    int a;
} Sfech;

typedef struct
{
    long dni;
    char nyp[60];
    Sfech fechNac;
    char cuil[12];
    char sexo;
    Sfech fechAfilia;
    char categoria[10];
    Sfech fechCuotaPaga;
    char estado;
    char plan[10];
    char emailTutor[30];
} Ssocio;


typedef struct
{
    int idPelicula;
    char titulo[61];
    char genero[21];
    int stock;
    char estado;

} Spelicula;


/// Estructura agregada para resuelve H
typedef struct
{
    long dni;
    int idPelicula;
    int cantAlquileres;
    char activo;  ///S Alquilada y N no alquilada.
} SAlquiler;

#endif // ESTRUCTURA_H_INCLUDED
