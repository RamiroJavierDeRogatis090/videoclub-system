#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include "estructura.h"
#include "indice.h"

/// VALIDACIONES DE FECHA ///
void fechaActual(Sfech* f);
int esBisiesto(Sfech *f);
int validarFecha(Sfech *f);
int fechaMenor(Sfech*f1, Sfech*f2);
int validarFechaMenorDiez(Sfech *fechNac, Sfech *fechProc);

///FUNCIONES DE BIBLIOTECA STRING.H ///
int my_strcmpi(const char *comparacion,const char *original);
char* mystrchr(char*s1,int letra);
char mytoupper(char c);
char mytolower(char c);
size_t mystrlen(const char *s);
char* mystrcpy(char*s1,const char*s2);

/// VALIDACIONES DE LOS CAMPOS ///
int dniEsvalido(const long dni);
int nypEsvalido(const char *s);
int sexoEsvalido(const char s);
int estadoEsvalido(const char est);
int categoriaEsvalida(char* cat);
int planEsvalido(char* plan);
int emailTutorEsvalido(const char* email);
int calcular_edad(Sfech *fNac);
void eliminarEspaciosRepetidos(char *s);
void eliminarEspaciosExtremos(char *s);
void normalizar(char *cad);
void limpiarCadena(char *cad);
void limpiar_campo(char *cad);
char* pasarMayusculas(char *cad);


#endif // VALIDACIONES_H_INCLUDED
