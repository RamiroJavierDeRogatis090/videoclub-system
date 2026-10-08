#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "indice.h"
#include "tda.h"
#include "estructura.h"

///Funciones del menu///
char menu(char m[][L_MENU],const char *tit);
void switchmenu(t_indice *indiC,t_indice* indiP,t_indice* indiAYN,tda_vec*vector_alquileres,tda_vec*vector_validosClientes,tda_vec*vector_validosPeliculas,
                long Matriz_auditoriaP[][2], long Matriz_auditoriaC[][2],Sfech *f, int erroresC, int erroresP );

char opcion(char m[][L_MENU], const char *tit, const char *msj);

/// Funciones del menu
void resuelveA(t_indice *indi,t_indice*indiAYP,tda_vec * vectorValidos, Ssocio *s, Sfech *hoy);
void resuelveB (t_indice *indiP,tda_vec * vectorValidos, Spelicula *p);
void resuelveC(t_indice *indi,t_indice*indiAYP, tda_vec *vectorValidos);
void resuelveD (t_indice *indi, tda_vec *vectorValidos);
void resuelveE(t_indice *indi, tda_vec *vectorValidos, Sfech *hoy);
void resuelveF (t_indice *indiP, tda_vec *vectorValidos);
void resuelveG (t_indice *indi, tda_vec *vectorValidos);
void resuelveH(t_indice *indiC,t_indice *indiP,tda_vec *vecSocios,tda_vec *vecPeliculas,tda_vec *vecAlquileres);
void resuelveI(t_indice *indi, tda_vec *vectorValidos);
void resuelveJ (t_indice *indiAYN, tda_vec *vectorValidos);
void resuelveK (long matC [][2], long matP [][2], int erroresC, int erroresP);
const char* validarPeliculaRetornaCadena(Spelicula *p);


#endif // MENU_H_INCLUDED
