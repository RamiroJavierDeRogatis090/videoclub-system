#include "funcionesDeLibreria.h"
#include "string.h"
#include "indice.h"
#include <ctype.h>
#include "estructura.h"
#include "macros.h"
#include "validacionesTitulos.h"
#include "validacionesClientes.h"

int validarPelicula(Spelicula *p, int *ultimoId)
{

    if(!generoEsvalido(p->genero))
        return ERROR_GENERO;

    if(!cantidadVHSD(&p->stock))
        return ERROR_STOCK;

    if(!estadoEsvalido(p->estado))
        return ERROR_ESTADO;
    if(!esAutoIncremental(p->idPelicula, ultimoId))
    {

        return ERROR_ID_AUTOINCREMENTAL;
    }

    *ultimoId = p->idPelicula;

    return 0;
}

int esAutoIncremental(int idPelicula, int *ultimoId)
{
    return idPelicula == (*ultimoId + 1);
}



int generoEsvalido(char* genero)
{

    if(my_strcmpi(genero,"accion")== 0 || my_strcmpi(genero,"drama")== 0 || my_strcmpi(genero,"Comedia")== 0 || my_strcmpi(genero,"Terror")== 0)
        return 1;
    else
        return 0;
}

int cantidadVHSD (const int *stock)
{
    if(*stock<0)
    {
        return 0;
    }
    return 1;
}

char* normalizar_titulo(char* cad)
{
    char aux[100];

    char* lect = cad;
    char* esc = aux;

    int primeraLetraPalabra;
    int posicionPalabra = 0;

    while(*lect)
    {
        while(*lect &&
                (isspace(*lect) || *lect == ','))
        {
            lect++;
        }

        if(*lect)
        {
            posicionPalabra++;

            if(posicionPalabra == 2)
            {
                *esc = ',';
                esc++;

                *esc = ' ';
                esc++;
            }
            else if(posicionPalabra > 2)
            {
                *esc = ' ';
                esc++;
            }

            primeraLetraPalabra = 1;

            while(*lect &&
                    !isspace(*lect) &&
                    *lect != ',')
            {
                if(primeraLetraPalabra)
                {
                    *esc = toupper(*lect);
                    primeraLetraPalabra = 0;
                }
                else
                {
                    *esc = tolower(*lect);
                }

                esc++;
                lect++;
            }
        }
    }

    *esc = '\0';

    strcpy(cad, aux);

    return cad;
}




int generarNuevoId(t_indice *indiP)
{
    t_reg_indice_pelicula *ult;

    if(indiP->cantidad_elementos_actual == 0)
        return 1;   // primer ID

    ult = (t_reg_indice_pelicula*)indiP->vindice +
          (indiP->cantidad_elementos_actual - 1);

    return ult->id_pelicula + 1;
}




