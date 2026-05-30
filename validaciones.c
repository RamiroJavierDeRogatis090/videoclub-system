#include "validaciones.h"

/////////////////// VALIDACIONES DE FECHAS ///////////////////
void fechaActual(Sfech* fecha)
{
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    fecha->d = tm->tm_mday;
    fecha->m = tm->tm_mon + 1;
    fecha->a = tm->tm_year + 1900;
}

int esBisiesto(Sfech *f)
{
    return (f->a % 4 == 0 && (f->a % 100 != 0 || f->a % 400 == 0));

}

int fechaMenor(Sfech*f1, Sfech*f2)
{
    if (f1->a < f2->a) return 1;
    if (f1->a > f2->a) return 0;
    if (f1->m < f2->m) return 1;
    if (f1->m > f2->m) return 0;
    return f1->d <= f2->d;
}

int validarFecha(Sfech *f)
{

    int es_valida = 1;  // bandeta que uso para devolver si la fecha es valida o no.

    // Validar año
    if (f->a < MIN_YEAR || f->a > MAX_YEAR)
    {
        es_valida = 0;
    }

    // Validar mes
    if (f->m < 1 || f->m > 12)
    {
        es_valida = 0;
    }

    // Vector con los días por mes
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int *ptrDias = diasPorMes;

    // Ajustar febrero si es bisiesto
    if (esBisiesto(f))   //aca si anio me retorna verdadero entra
    {
        *(ptrDias + 1) = 29;
    }

    // Validar día
    if (f->d < 1 || f->d > *(ptrDias + f->m - 1))
    {
        es_valida = 0;
    }

    return es_valida;

}

int validarFechaMenorDiez(Sfech *fechNac, Sfech *fechProc)
{

    //El usuario tiene que ser mayor a 10 años para estar en la base de datos
    if(fechProc->a - fechNac->a >10)
    {
        return 1; //es mayor
    }
    else
    {
        if((fechProc->a - fechNac->a) == 10)
        {
            if(fechProc->m == fechNac->m)
            {
                if(fechProc->d >= fechNac->d)
                {
                    return 1; //cumple antes de la fecha de proceso, tiene 10
                }
                else
                {
                    return 0; //cumple despues de fecha de proceso, tiene 9
                }
            }
            else
            {
                if(fechProc->m > fechNac->m)
                    return 0; //cumple en mes antes de fecha de proceso, tiene 9
                else
                    return 1; //cumple en mes despues de fecha de proceso, tiene 10
            }
        }
        else
            return 0;
    }

}

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

    while(*s2!='\0' && *s1!='\0')
    {
        *s1=*s2;
        s1++;
        s2++;
    }
    *s1='\0';
    return aux;
}


////////////////  VALIDACIONES DE LOS CAMPOS  //////////////////
int dniEsvalido(const long dni)
{
    if(dni<=1000000 || dni>=100000000)
        return 0;
    else
        return 1;
}

int nypEsvalido(const char *s)
{
    if(strlen(s) >= 60 || strlen(s) <= 0)
        return 0;
    else
        return 1;
}

int sexoEsvalido(const char s)
{
    if(s=='F'||s=='M'||s=='f'||s=='m')
        return 1;
    else
        return 0;
}

int estadoEsvalido(const char est)
{
    if(est=='A'||est=='B'||est=='a'||est=='b')
        return 1;
    else
        return 0;
}

int categoriaEsvalida(char* cat)
{
    pasarMayusculas(cat);
    if(strcmp(cat,"MENOR")== 0 || strcmp(cat,"ADULTO")== 0)
        return 1;
    else
        return 0;
}

int planEsvalido(char* plan)
{
    limpiarCadena(plan);
    pasarMayusculas(plan);
    if(strcmp(plan,"BASIC")== 0 || strcmp(plan,"PREMIUM")== 0 || strcmp(plan,"VIP")== 0 || strcmp(plan,"FAMILY")== 0)
        return 1;
    else
        return 0;
}

int emailTutorEsvalido(const char* email)
{
    const char *p = email;
    const char *arroba = NULL;
    int punto_despues = 0;

    if(strlen(p) >= 30)
        return 0;

    // Verifica que no empiece con '@' o '.'
    if (*p == '@' || *p == '.') return 0;

    while (*p)
    {
        if (*p == '@')
        {
            if (arroba) return 0; // Más de un '@'
            arroba = p;
        }
        else if (*p == '.' && arroba && p > arroba + 1)
            punto_despues = 1;
        p++;
    }

    // Verifica que haya un '@', que no esté al final, y que haya un '.' después
    if (!arroba || *(p - 1) == '@' || *(p - 1) == '.') return 0;
    if (!punto_despues) return 0;

    return 1;
}

int calcular_edad(Sfech *fNac)
{
    time_t t = time(NULL);
    struct tm *hoy = localtime(&t);

    int *diaNac = &fNac->d;
    int *mesNac = &fNac->m;
    int *anioNac = &fNac->a;

    int edad = hoy->tm_year + 1900 - *anioNac;

    if ((hoy->tm_mon + 1 < *mesNac) ||
            (hoy->tm_mon + 1 == *mesNac && hoy->tm_mday < *diaNac))
    {
        edad--;
    }

    return edad;
}

////// NORMALIZACION DE CAMPO APELLIDO Y NOMBRE ///////
void eliminarEspaciosRepetidos(char *s)
{
    char *p = s;
    char *q = s;

    while(*p)
    {
        while(*p == ' ' && *(p+1) == ' ')
            p ++;
        *q = *p;
        q ++;
        p ++;
    }

    *q = '\0';
}

void eliminarEspaciosExtremos(char *s)
{
    char *inicio = s;
    char *fin;

    while(*inicio == ' ')
        inicio++;

    char *dest = s;
    while(*inicio)
        *dest++ = *inicio++;
    *dest = '\0';

    fin = s + strlen(s) - 1;
    while(fin >= s && *fin == ' ')
        fin--;

    *(fin + 1) = '\0';
}

void normalizar(char *cad)
{
    char *ini = cad;
    eliminarEspaciosExtremos(cad);
    eliminarEspaciosRepetidos(cad);

    if(*cad >= 'a' && *cad <= 'z')
        *cad = mytoupper(*cad);
    cad ++;

    while(*cad)
    {
        if(*cad >= 'A' && *cad <= 'Z' && *(cad-1) != ' ')
            *cad = mytolower(*cad);
        if(*cad >= 'a' && *cad <= 'z' && *(cad-1) == ' ')
            *cad = mytoupper(*cad);
        cad ++;
    }

    cad = ini;
    if (strchr(cad, ','))
        return;

    while(*cad && *cad != ' ' && *cad != ',')
        cad ++;

    if(*cad==',')
        cad ++;
    else
    {
        if(*cad == ' ')
        {
            char *fin = cad + strlen(cad);
            while(fin >= cad)
            {
                *(fin+1) = *fin;
                fin --;
            }

            *cad = ',';
        }
    }

}

void limpiarCadena(char *cad)
{
    char *p = cad;

    while (*p)
        p++;

    while (p > cad && (p[-1] == '\n' || p[-1] == '\r' || p[-1] == ' ' || p[-1] == '\t'))
        p--;

    *p = '\0';
}

void limpiar_campo(char *cad)
{
    size_t len = strlen(cad);
    while (len > 0 && (cad[len - 1] == '|' || cad[len - 1] == '\n' || cad[len - 1] == ' '))
        cad[--len] = '\0';
}

char* pasarMayusculas(char *cad)
{
    char *ini = cad;

    while(*cad)
    {
        if(*cad >= 'a' && *cad <= 'z')
            *cad = mytoupper(*cad);
        cad ++;
    }
    return ini;
}
