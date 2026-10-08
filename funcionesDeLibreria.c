#include "funcionesDeLibreria.h"
#include "macros.h"

///////////////// BIBLIOTECA STRING.H ///////////////////////
int my_strcmpi(const char *comparacion, const char *original)   //retornara 1 si no son iguales y retornara 0 si son iguales
{
    int i = 0;

    if(mystrlen(comparacion) != mystrlen(original))
        return 1;

    while(comparacion[i]!='\0' && original[i]!='\0')
    {
        char c1 = original[i];
        char c2 = comparacion[i];

        if (c1 >= 'A' && c1 <= 'Z')
            c1 = c1 + 32;
        if (c2 >= 'A' && c2 <= 'Z')
            c2 = c2 + 32;

        if (c1 != c2)
            return 1;
        i ++;
    }

    return 0;
}

char* mystrchr(char*s1,int letra) //devuelve direccion de memoria del caraacter que quiero
{
    while(*s1!='\0')
    {
        if(*s1==letra)
            return s1;
        s1++;
    }

    return NULL;
}

char mytoupper(char c)  //vuelve minus en mayus
{
    if(c<='z' && c>='a')
        c = c - 32;

    return c;

}

char mytolower(char c)  //vuelve mayus en minus
{
    if(c<='Z' && c>='A')
        c = c +32;

    return c;
}

size_t mystrlen(const char *s)
{
    size_t cont = 0;

    while(*s != '\0')
    {
        cont++;
        s++;
    }

    return cont;
}

char* mystrcpy(char*s1,const char*s2)  //copia cadena 2 en cadena 1
{
    char *aux=s1;  //guardo puntero en primera pos de s1

    while(*s2!='\0')
    {
        *s1=*s2;
        s1++;
        s2++;
    }
    *s1='\0';
    return aux;
}


