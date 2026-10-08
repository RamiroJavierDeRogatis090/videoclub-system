#include "printeo.h"
#include "macros.h"
#include "validacionesClientes.h"
#include "validacionesTitulos.h"
#include "tda.h"
#include "estructura.h"

/// muestra matriz de errores
void mostrarAuditoria(long auditoria[][2], int cantErrores)
{
    int i;

    printf("\n================ AUDITORIA ================\n");
    printf("%-20s %-12s\n", "ERROR", "DNI");
    printf("------------------------------------------\n");

    for(i = 0; i < cantErrores; i++)
    {
        printf("%-20s %ld\n",
               descripcionError(auditoria[i][0]),
               auditoria[i][1]);
    }
}


const char* descripcionError(int cod)
{
    switch(cod)
    {
    case ERR_DNI:
        return "DNI";
    case ERR_CUIL:
        return "CUIL";
    case ERR_NYP:
        return "NOMBRE";
    case ERR_SEXO:
        return "SEXO";
    case ERR_ESTADO:
        return "ESTADO";
    case ERR_CATEGORIA:
        return "CATEGORIA";
    case ERR_FEC_AFIL:
        return "FEC_AFIL";
    case ERR_FEC_CUOTA:
        return "FEC_CUOTA";
    case ERR_FEC_NAC:
        return "FEC_NAC";
    case ERR_EDAD:
        return "EDAD";
    case ERR_AFIL_NAC:
        return "AFIL_NAC";
    case ERR_AFIL_PROCESO:
        return "AFIL_PROC";
    case ERR_CUOTA_PROCESO:
        return "CUOTA_PROC";
    case ERR_CUOTA_AFIL:
        return "CUOTA_AFIL";
    case ERR_PLAN:
        return "PLAN";
    case ERR_EMAIL_TUTOR:
        return "EMAIL_TUTOR";
    default:
        return "DESCONOCIDO";
    }
}


/// Muestra el TDA VEC
void mostrarValidos(const tda_vec* validos)
{
    int i;
    Ssocio* socio;

    printf("\n=========== SOCIOS VALIDOS ===========\n\n");

    for(i = 0; i < validos->ce; i++)
    {
        socio = (Ssocio*)((char*)validos->vec + i * validos->tam_elem);

        printf("DNI: %ld\n", socio->dni);
        printf("Nombre y Apellido: %s\n", socio->nyp);

        printf("Fecha Nacimiento: %02d/%02d/%04d\n",
               socio->fechNac.d,
               socio->fechNac.m,
               socio->fechNac.a);

        printf("CUIL: %s\n", socio->cuil);

        printf("Sexo: %c\n", socio->sexo);

        printf("Fecha Afiliacion: %02d/%02d/%04d\n",
               socio->fechAfilia.d,
               socio->fechAfilia.m,
               socio->fechAfilia.a);

        printf("Categoria: %s\n", socio->categoria);

        printf("Fecha Ultima Cuota: %02d/%02d/%04d\n",
               socio->fechCuotaPaga.d,
               socio->fechCuotaPaga.m,
               socio->fechCuotaPaga.a);

        printf("Estado: %c\n", socio->estado);

        printf("Plan: %s\n", socio->plan);

        printf("Email Tutor: %s\n", socio->emailTutor);

        printf("--------------------------------------------\n");
    }
}



/// Mostrar peliculas ///


const char* descripcionErrorP(int tipo)
{
    switch(tipo)
    {
    case 1:
        return "ERROR ID AUTOINCREMENTAL";
    case 2:
        return "ERROR GENERO";
    case 3:
        return "ERROR STOCK";
    case 4:
        return "ERROR ESTADO";
    default:
        return "ERROR DESCONOCIDO";
    }
}


void mostrarAuditoriaP(long auditoria[][2], int cantErrores)
{
    int i;

    printf("\n===== AUDITORIA DE ERRORES =====\n");

    for(i = 0; i < cantErrores; i++)
    {
        printf("Error: %s | ID Pelicula: %ld\n",
               descripcionErrorP(auditoria[i][0]),
               auditoria[i][1]);
    }
}

void tda_vec_obtener(tda_vec *v, int pos, void *dest)
{
    if(pos < 0 || pos >= v->ce)
        return;

    memcpy(dest,
           (char*)v->vec + pos * v->tam_elem,
           v->tam_elem);
}
void mostrarValidosPelicula(tda_vec *v)
{
    int i;
    int cant = v->ce;   /// si tu TDA lo permite

    Spelicula p;

    printf("\n===== PELICULAS VALIDAS =====\n");

    for(i = 0; i < cant; i++)
    {
        tda_vec_obtener(v, i, &p);

        printf("ID: %d\n", p.idPelicula);
        printf("Titulo: %s\n", p.titulo);
        printf("Genero: %s\n", p.genero);
        printf("Stock: %d\n", p.stock);
        printf("Estado: %c\n", p.estado);
        printf("----------------------------\n");
    }
}








/// Mostrar Matriz
void mostrarMatrizAuditoria(const char *titulo, long mat[][2], int filas)
{
    int i;

    printf("\n==============================\n");
    printf(" %s\n", titulo);
    printf("==============================\n");
    printf("%-10s %-10s\n", "ERROR", "id");

    for (i = 0; i < filas; i++)
    {
        printf("%-10ld %-10ld\n", mat[i][0], mat[i][1]);
    }
}




