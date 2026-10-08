#ifndef FUNCIONESDELIBRERIA_H_INCLUDED
#define FUNCIONESDELIBRERIA_H_INCLUDED
#include "estructura.h"
#include "macros.h"

///FUNCIONES DE BIBLIOTECA STRING.H ///
int my_strcmpi(const char *comparacion, const char *original);
char* mystrchr(char*s1,int letra);
char mytoupper(char c);
char mytolower(char c);
size_t mystrlen(const char *s);
char* mystrcpy(char*s1,const char*s2);

#endif // FUNCIONESDELIBRERIA_H_INCLUDED
