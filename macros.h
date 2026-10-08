#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED
#include <stdlib.h>

/// Bibliotecas ///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
///Defines MENU
#define L_MENU 100
///Defines Fechas
#define MIN_YEAR 1900
#define MAX_YEAR 2026
#define REGISTRO 150
///Defines Nombres de archivos
#define NOMTXTC "Miembros.csv"
#define NOMTXTP "Titulos1.csv"
#define MIEMBROSVC "miembros-VC-"
#define MIEMBROSVP "peliculas-VC-"
#define MAX_PATH 100
/// Defines de Retornos
#define NO_EXISTE -1
#define err_memoria 2
#define todo_ok 0
#define CANTIDAD_ELEMENTOS 100
#define OK 1
#define ERROR 0
/// Defines de TDA
#define INCREMENTO 1.3
#define capacidad_inicial 100
#define factor_incremento 1.5
/// Defines retornos con tipo de error
#define ERR_DNI                 1
#define ERR_CUIL                2
#define ERR_NYP                 3
#define ERR_SEXO                4
#define ERR_ESTADO              5
#define ERR_CATEGORIA           6
#define ERR_FEC_AFIL            7
#define ERR_FEC_CUOTA           8
#define ERR_FEC_NAC             9
#define ERR_EDAD                10
#define ERR_AFIL_NAC            11
#define ERR_AFIL_PROCESO        12
#define ERR_CUOTA_PROCESO       13
#define ERR_CUOTA_AFIL          14
#define ERR_PLAN                15
#define ERR_EMAIL_TUTOR         16
#define SIN_ERROR               0
/// Defines para las peliculas
#define ERROR_ID_AUTOINCREMENTAL 1
#define ERROR_GENERO             2
#define ERROR_STOCK              3
#define ERROR_ESTADO             4

#endif // MACROS_H_INCLUDED
