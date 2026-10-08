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
int validarFechaMayor18(Sfech *fechNac, Sfech *fechProc);

/// Validaciones Clientes ///
int validarCuil (long int dni,  char tipo, char*cuil);
char * normalizar(char * cad);
int dniEsvalido(const long dni);
int nypEsvalido(const char *s);
int sexoEsvalido(const char s);
int estadoEsvalido(const char est);
int categoriaEsvalida(char* cat);
int planEsvalido(char* plan);
int emailTutorEsvalido(const char* email);
int calcular_edad(Sfech *fNac);
const char* validarSocioRetornaCadena(Ssocio* miembro, Sfech *f);

/// Manejo de cadenas para las validaciones ///
void eliminarEspaciosRepetidos(char *s);
void eliminarEspaciosExtremos(char *s);
void limpiarCadena(char *cad);
void limpiar_campo(char *cad);
char* pasarMayusculas(char *cad);

#endif // VALIDACIONES_H_INCLUDED

