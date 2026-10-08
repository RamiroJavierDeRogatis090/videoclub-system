#ifndef VALIDACIONESTITULOS_H_INCLUDED
#define VALIDACIONESTITULOS_H_INCLUDED

char * normalizar_titulo (char * cad);
int validarIdUnico(const t_indice* indice, int id);
int cantidadVHSD (const int*stock);
int generoEsvalido(char* genero);
int autoIncremental(int idPelicula, int* ultimoId);
int validarPelicula(Spelicula *p, int *ultimoId);
int esAutoIncremental(int idPelicula, int *ultimoId);
int estadoEsvalido(const char est);
int generarNuevoId(t_indice *indiP);

#endif // VALIDACIONESTITULOS_H_INCLUDED
