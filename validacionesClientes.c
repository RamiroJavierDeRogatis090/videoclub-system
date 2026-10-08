#include "estructura.h"
#include "funcionesDeLibreria.h"
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "validacionesClientes.h"

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
///////////// VALIDACION DE QUE SEA MAYOR A 18/////////////

int validarFechaMayor18(Sfech *fechNac, Sfech *fechProc)
{
    if(fechProc->a - fechNac->a >18)
    {
        return 1; //es mayor
    }
    else
    {
        if((fechProc->a - fechNac->a) == 18)
        {
            if(fechProc->m == fechNac->m)
            {
                if(fechProc->d >= fechNac->d)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                if(fechProc->m > fechNac->m)
                    return 0;
                else
                    return 1;
            }
        }
        else
            return 0;
    }

}

///////////// VALIDACION DE CAMPOS /////////////////


int validarCuil(long int dni, char tipo, char* cuil)
{
    int xy, z = 0, resto, suma = 0;

    int pesos[10] = {5,4,3,2,7,6,5,4,3,2};

    int digitos[10];

    int *pPesos = pesos;
    int *pDig = digitos;

    char cuitResultante[20];

    long dniOriginal = dni;

    /// Tipo
    if(tipo == 'M' || tipo == 'm')
        xy = 20;
    else if(tipo == 'F' || tipo == 'f')
        xy = 27;
    else if(tipo == 'O' || tipo == 'o')
        xy = 30;
    else
        return 0;

    /// XY
    *pDig = xy / 10;
    pDig++;

    *pDig = xy % 10;
    pDig++;

    /// DNI
    for(int i = 0; i < 8; i++)
    {
        *(pDig + (7 - i)) = dni % 10;
        dni /= 10;
    }

    /// Multiplicación
    pDig = digitos;

    for(int i = 0; i < 10; i++)
    {
        suma += (*(pDig + i)) * (*(pPesos + i));
    }

    resto = suma % 11;

    /// Dígito verificador
    if(resto == 0)
    {
        z = 0;
    }
    else if(resto == 1)
    {
        if(xy == 20)
        {
            xy = 23;
            z = 9;
        }
        else if(xy == 27)
        {
            xy = 23;
            z = 4;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        z = 11 - resto;
    }

    sprintf(cuitResultante,
            "%02d%08ld%d",
            xy,
            dniOriginal,
            z);


    cuil[strcspn(cuil, "\r\n")] = '\0';
    return strcmp(cuitResultante, cuil) == 0;
}

////// Normalizar /////////////////

char *normalizar(char *cad)
{
    char *lect = cad;
    char *esc = cad;
    int primeraLetraPalabra;
    int posicionPalabra = 0;

    while(*lect)
    {
        // Saltamos espacios y comas
        while(*lect && (isspace((unsigned char)*lect) || *lect == ','))
            lect++;

        if(*lect)
        {
            posicionPalabra++;

            if(posicionPalabra == 2)
            {

                if (esc >= lect) {
                    char *fin = lect;
                    while (*fin) fin++;
                    while (fin >= lect) {
                        *(fin + 1) = *fin;
                        fin--;
                    }
                    lect++;
                }
                *esc++ = ',';

                if (esc >= lect) {
                    char *fin = lect;
                    while (*fin) fin++;
                    while (fin >= lect) {
                        *(fin + 1) = *fin;
                        fin--;
                    }
                    lect++;
                }
                *esc++ = ' ';
            }
            else if(posicionPalabra > 2)
            {
                if (esc >= lect) {
                    char *fin = lect;
                    while (*fin) fin++;
                    while (fin >= lect) {
                        *(fin + 1) = *fin;
                        fin--;
                    }
                    lect++;
                }
                *esc++ = ' ';
            }

            primeraLetraPalabra = 1;

            while(*lect && !isspace((unsigned char)*lect) && *lect != ',')
            {
                *esc++ = primeraLetraPalabra ?
                         toupper((unsigned char)*lect) :
                         tolower((unsigned char)*lect);

                primeraLetraPalabra = 0;
                lect++;
            }
        }
    }

    *esc = '\0';
    return cad;
}

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


/// Valida socios y retorna el error  ///
const char* validarSocioRetornaCadena(Ssocio* miembro, Sfech *f)
{
    if (!dniEsvalido(miembro->dni))
    {
        return ("\nDNI invalido");
    }
    else if (!validarCuil(miembro->dni, miembro->sexo, miembro->cuil))
    {
        return "\nCUIL invalido";
    }

    else if (!nypEsvalido(miembro->nyp))
    {
        return ("\nNombre y apellido invalido");
    }
    else if (!sexoEsvalido(miembro->sexo))
    {
        return("\nSexo invalido");

    }
    else if (!estadoEsvalido(miembro->estado))
    {
        return("\nEstado invalido");

    }
    else if(!categoriaEsvalida(miembro->categoria))
    {
        return("\nCategoria invalida");

    }
    else if (!validarFecha(&miembro->fechAfilia))
    {
        return("\nFecha de afiliacion invalida");
    }
    else if(!validarFecha(&miembro->fechCuotaPaga))
    {
        return("\nFecha de ultima cuota invalida");
    }
    else if (!validarFecha(&miembro->fechNac))
    {
        return("\nFecha de nacimiento invalida");

    }
    else if (!validarFechaMenorDiez(&miembro->fechNac, f))
    {
        return("\nEl socio debe tener al menos 10 anios");

    }
    else if (!fechaMenor(&miembro->fechNac, &miembro->fechAfilia))
    {
        return( "\nLa afiliacion no puede ser anterior a la fecha de nacimiento");

    }
    else if (!fechaMenor(&miembro->fechAfilia, f))
    {
        return( "\nFecha de afiliacion posterior a la fecha de proceso");
    }
    else if (!fechaMenor(&miembro->fechCuotaPaga, f))
    {
        return("\nFecha de ultima cuota posterior a la fecha de proceso");

    }
    else if (!fechaMenor(&miembro->fechAfilia, &miembro->fechCuotaPaga))
    {
        return("\nLa ultima cuota no puede ser anterior a la afiliacion");

    }
    else if (!planEsvalido(miembro->plan))
    {
        return("\nPlan invalido");

    }
    else if (strcmp(miembro->categoria, "MENOR") == 0)
    {
        if (!emailTutorEsvalido(miembro->emailTutor))
        {
            return( "\nEmail del tutor invalido\n");

        }
    }
    else if ( !validarFechaMayor18(&miembro->fechNac, f) && stricmp(miembro->categoria, "adulto")==0){
        return( "\nEl socio es menor y usted indico categoria adulto\n");
    }
    else if (validarFechaMayor18(&miembro->fechNac, f) && stricmp(miembro->categoria, "menor")==0){

        return( "\nEl socio es mayor y usted indico categoria menor\n");
    }

    return NULL; // Todo vlido
}

