#include "macros.h"
#include "estructura.h"
#include "menu.h"
#include "indice.h"
#include "archivos.h"
#include "validacionesClientes.h"
#include "tda.h"
#include "printeo.h"
#include "indice.h"

int main()
{
    ///Nombres de archivos para persistencia en disco
    char nombinC[MAX_PATH], errtxtC[MAX_PATH]; //Esto se genera al final
    char nombinP[MAX_PATH], errtxtP[MAX_PATH];
    char nombinA[MAX_PATH];

    ///Declaracion de vectores TDA
    tda_vec vector_validosClientes,vector_validosPeliculas,vectorAlquileres;

    ///Fecha
    Sfech fechaProceso;
    fechaActual(&fechaProceso);

    /// Buffers y variables a estructura
    Ssocio bufferCliente;
    t_indice indiceClientes;

    Ssocio bufferClienteAYN;
    t_indice indiceClientesAYN;

    Spelicula bufferPeliculas;
    t_indice indicePeliculas;

    ///Contador para matriz
    int contarErroresP=0,contarErroresC=0;

    /// Creacion Matriz pelicula;
    int maxErroresP = contarLineas(NOMTXTP);
    long Matriz_auditoriaP[maxErroresP][2];

    ///Creacion de matriz para clientes
    int maxErroresC = contarLineas(NOMTXTC);
    long Matriz_auditoriaC[maxErroresC][2];

    ///Creacion de vectores TDA
    tda_vec_CREAR(&vector_validosClientes, sizeof(Ssocio));
    tda_vec_CREAR(&vector_validosPeliculas, sizeof(Spelicula));
    tda_vec_CREAR(&vectorAlquileres,sizeof(SAlquiler));

    ///Recuperacion de archivos (iteracion)
    int archivo_elegidoC=elegir_archivo_base(nombinC,&fechaProceso,MIEMBROSVC);
    fflush(stdin);
    int archivo_elegidoP=elegir_archivo_base(nombinP,&fechaProceso,MIEMBROSVP);

    puts("\n");
    system("pause");

    ///Asigno nombres a los archivos que van a persistir en memoria
    snprintf(nombinA,sizeof(nombinA),"alquileres-%04d%02d%02d.bin",fechaProceso.a,fechaProceso.m,fechaProceso.d);
    snprintf(errtxtC,sizeof(errtxtC),"error-VC-%04d%02d%02d.csv",fechaProceso.a,fechaProceso.m,fechaProceso.d);
    snprintf(errtxtP,sizeof(errtxtP),"error-VP-%04d%02d%02d.csv",fechaProceso.a,fechaProceso.m,fechaProceso.d);


    /// logica Clientes (si entra por 0 no selecciono la recuperacion de archivos)
    if(archivo_elegidoC == 0)
    {
        auditoriaClientes(NOMTXTC,&vector_validosClientes,&fechaProceso,Matriz_auditoriaC,&contarErroresC,maxErroresC);
    }
    else
    {
        cargaBinarioEnMemoriaC(nombinC,&vector_validosClientes);
        contarErroresC=cargaTXTaMatrizC(errtxtC,Matriz_auditoriaC);
    }

    /// logica Peliculas (si entra por 0 no selecciono la recuperacion de archivos)
    if(archivo_elegidoP == 0)
    {
        auditoriaPeliculas(NOMTXTP,&vector_validosPeliculas,Matriz_auditoriaP,&contarErroresP,maxErroresP);
    }
    else
    {
        cargaBinarioEnMemoriaP (nombinP,&vector_validosPeliculas);
        contarErroresP=cargaTXTaMatrizP(errtxtP,Matriz_auditoriaP);
    }

    /// Recupera siempre el archivo de los alquileres.
    cargaBinarioEnMemoriaA(nombinA,&vectorAlquileres);


    ///GENERO Y CARGO INDICE de los clientes
    indice_crear(&indiceClientes, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice));
    indice_cargar(&vector_validosClientes, &indiceClientes, &bufferCliente, sizeof(t_reg_indice), cmp_indice);

    /// Genera y cargo indice de los clientes por AYN
    indice_crear(&indiceClientesAYN, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice_ayn));
    indice_cargar_AYN(&vector_validosClientes, &indiceClientesAYN, &bufferClienteAYN, sizeof(t_reg_indice_ayn), cmp_indice_AYN);

    /// GENERO Y CARGO INDICE de las peliculas
    indice_crear(&indicePeliculas, CANTIDAD_ELEMENTOS, sizeof(t_reg_indice_pelicula));
    indice_cargar_peliculas(&vector_validosPeliculas, &indicePeliculas, &bufferPeliculas, sizeof(t_reg_indice_pelicula), cmp_indice_p);

    /// Creacion del menu
    switchmenu(&indiceClientes,&indicePeliculas,&indiceClientesAYN,&vectorAlquileres,&vector_validosClientes,&vector_validosPeliculas,Matriz_auditoriaP,Matriz_auditoriaC,&fechaProceso,contarErroresC,contarErroresP);

    /// Persistencia en disco
    persistenciaEnDisco(nombinC, nombinP,
                        errtxtC, errtxtP,nombinA,&vectorAlquileres,
                        &vector_validosClientes, &vector_validosPeliculas,
                        Matriz_auditoriaP, Matriz_auditoriaC,
                        contarErroresC, contarErroresP);

    /// Liberacion de indices
    indice_destruir(&indiceClientes);
    indice_destruir(&indiceClientesAYN);
    indice_destruir(&indicePeliculas);

    /// Liberacion de vectores
    tda_vec_DESTRUIR(&vector_validosClientes);
    tda_vec_DESTRUIR(&vector_validosPeliculas);
    tda_vec_DESTRUIR(&vectorAlquileres);


    return 0;
}
