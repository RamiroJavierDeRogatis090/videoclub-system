#ifndef ESTRUCTURA_H_INCLUDED
#define ESTRUCTURA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define L_MENU 100
#define MIN_YEAR 1900
#define MAX_YEAR 2025
#define REGISTRO 150
#define NOMTXT "miembros-VC.txt"
#define MAX_PATH 100

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
    char sexo;
    Sfech fechAfilia;
    char categoria[10];
    Sfech fechCuotaPaga;
    char estado;
    char plan[10];
    char emailTutor[30];
} Ssocio;



#endif // ESTRUCTURA_H_INCLUDED
