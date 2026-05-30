#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>

#include "validaciones.h"
#include "estructura.h"
#include "indice.h"
#include "archivos.h"


void switchmenu(t_indice *indi, Sfech *, const char *nombin);
char menu(char m[][L_MENU],const char *tit);
char opcion(char m[][L_MENU], const char *tit, const char *msj);
void resuelveA(t_indice *indi, FILE *archbin, Ssocio *s, Sfech *hoy,char *cad_error);
void resuelveB(t_indice *indi, FILE*archbin, Ssocio *s);
void resuelveC(t_indice *indi, FILE*archbin,Ssocio *s,Sfech *hoy);
void resuelveD(t_indice *indi, FILE*archbin, Ssocio *s);
void resuelveE(t_indice *indi, FILE*archbin, Ssocio *socio);
void resuelveF(t_indice *indi, FILE *archbin, Ssocio *socio);


#endif // MENU_H_INCLUDED
