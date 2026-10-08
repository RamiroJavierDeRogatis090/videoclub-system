#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED
#include "estructura.h"
#include "indice.h"
#include "tda.h"

/// Generales de manejo de archivos ///
int contarLineas(const char *path);
int elegir_archivo_base(char *archivoBase,Sfech *fechaProceso,const char *prefijo);
int buscar_ultimo_archivo(char *nombreUltimo,const char *prefijo);

///Funciones de auditoria clientes ///
int validarSocio(Ssocio* miembro, Sfech *f);
void cargar_estructura( Ssocio* miembro,char* cad);
void auditoriaClientes (const char *ftxt,tda_vec * vec,Sfech *f,long matriz [][2], int *cantErrores, int maxErrores);

/// Funciones de auditoria de peliculas ///
void auditoriaPeliculas(const char *ftxt,tda_vec*vec, long matriz [][2], int *cantErrores, int maxErrores);
void cargar_estructuraP(Spelicula *s, const char *c);
int validarPelicula(Spelicula *p, int *ultimoId);
void escribirErroresTxt(FILE *fp, const int *vec, const char *vecN[], int tam, const char *titulo);

/// Funcion para mostrar los archivos binarios ///
void mostrarArchivoBinario(const char *fbin);
void mostrarPeliculasValidas(const char* fbin);

/// Funciones necesarias para la pesistencia de disco ///
int cargaBinarioEnMemoriaC(const char *nomBinC, tda_vec *vecClientes);
int cargaBinarioEnMemoriaP (const char *nomBinP, tda_vec *vecPeliculas);
int cargaBinarioEnMemoriaA(const char *nomBinA, tda_vec *vecClientes);



/// Persistencia en disco


void persistenciaEnDisco(const char *nomBinC,
                         const char *nomBinP,
                         const char *errtxtC,
                         const char *errtxtP,
                         const char *nomBinA,
                         tda_vec *vecAlquileres,
                         tda_vec *vecClientes,
                         tda_vec *vecPeliculas,
                         long matAudP[][2],
                         long matAudC[][2],
                         int erroresC,
                         int erroresP);


int cargaTXTaMatrizP (const char *nombreArchivo, long matriz[][2]);
int cargaTXTaMatrizC (const char *nombreArchivo, long matriz[][2]);



#endif // ARCHIVOS_H_INCLUDED
