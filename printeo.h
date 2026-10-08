#ifndef PRINTEO_H_INCLUDED
#define PRINTEO_H_INCLUDED
#include "tda.h"
#include "estructura.h"
void mostrarAuditoria(long auditoria[][2], int cantErrores);
void mostrarValidos(const tda_vec* validos);
const char* descripcionError(int cod);
void mostrarAuditoriaP(long auditoria[][2], int cantErrores);
const char* descripcionErrorp(int tipo);
void mostrarMatrizAuditoria(const char *titulo, long mat[][2], int filas);

void mostrarValidosPelicula(tda_vec *v);

#endif // PRINTEO_H_INCLUDED
