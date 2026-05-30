#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>

#include "estructura.h"
#include "indice.h"
#include "validaciones.h"

/// MANIPULACION DE ARCHIVO ///
void archtextbin(const char *ftxt, const char *fbin, const char *error, Sfech *f);
void cargar_estructura(Ssocio *s, const char *c);
void cargarerror(FILE *binerror, Ssocio *s1, char *cad_error);
int validarSocio(Ssocio *s, Sfech *f, char *cad_error);
int buscar_ultimo_archivo(char *nombreUltimo);
int elegir_archivo_base(char *archivoBase, Sfech *f);
int buscar_ultimo_archivo(char *nombreUltimo);


#endif // ARCHIVOS_H_INCLUDED
