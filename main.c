#include "menu.h"
#include "indice.h"
#include "estructura.h"
#include "archivos.h"

int main()
{
    Sfech fechaProceso;
    Ssocio buffer;
    t_indice indi;
    char nombin[MAX_PATH], errtxt[MAX_PATH];

    ///RECUPERACION DE ARCHIVOS
    fechaActual(&fechaProceso);
    int archivo_elegido = elegir_archivo_base(nombin, &fechaProceso);

    ///CONVIERTO DE TXT A BIN SI ES NECESARIO
    if(archivo_elegido == 0)
    {
        sprintf(errtxt, "error-VC-%04d%02d%02d.txt", fechaProceso.a, fechaProceso.m, fechaProceso.d);
        archtextbin(NOMTXT, nombin, errtxt, &fechaProceso);
    }

    ///GENERO Y CARGO INDICE
    indice_crear(&indi, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    indice_cargar(nombin, &indi, &buffer, sizeof(t_reg_indice), cmp_indice);

    ///CARGA EL MENU
    switchmenu(&indi, &fechaProceso, nombin);

    ///LIBERA MEMORIA DEL INDICE
    indice_vaciar(&indi);
}
