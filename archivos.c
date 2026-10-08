#include "archivos.h"
#include "tda.h"
#include "estructura.h"
#include "macros.h"
#include "printeo.h"
#include "macros.h"
#include "validacionesClientes.h"
#include "validacionesTitulos.h"
#include "indice.h"


/// Proceso de recuperacion de archivos

int elegir_archivo_base(char *archivoBase,Sfech *fechaProceso,const char *prefijo)
{
    char archivoUltimo[MAX_PATH];
    char archivoNuevo[MAX_PATH];

    int existePrevio = buscar_ultimo_archivo(archivoUltimo, prefijo);

    if(existePrevio)
    {
        printf("\nSe encontro un archivo previo: %s\n", archivoUltimo);
        printf("Desea trabajar con el archivo recuperado? S(Si) / N(No): ");

        char opcion;

        scanf(" %c", &opcion);

        while(opcion != 'S' && opcion != 's' &&
                opcion != 'N' && opcion != 'n')
        {
            printf("\nError. Opcion invalida.\n");
            printf("Desea trabajar con el archivo recuperado? S(Si) / N(No): ");
            while(getchar() != '\n');
            scanf(" %c", &opcion);
        }

        if(opcion == 'S' || opcion == 's')
        {
            printf("\nSe usara el archivo original.\n");
            strcpy(archivoBase, archivoUltimo);
            return 1;
        }
    }

    printf("\nSe genera un archivo nuevo.\n");
    sprintf(archivoNuevo,
            "%s%04d%02d%02d.dat",
            prefijo,
            fechaProceso->a,
            fechaProceso->m,
            fechaProceso->d);

    strcpy(archivoBase, archivoNuevo);

    printf("Se generara: %s\n", archivoNuevo);

    return 0;
}


int buscar_ultimo_archivo(char *nombreUltimo,const char *prefijo)
{
    DIR *dir = opendir(".");

    if(!dir)
        return 0;

    struct dirent *ent;

    char ultimo[MAX_PATH] = "";

    int fechaMax = 0;

    while((ent = readdir(dir)) != NULL)
    {
        if(strncmp(ent->d_name,prefijo,strlen(prefijo)) == 0 && strstr(ent->d_name, ".dat"))
        {
            char fechaStr[9];

            strncpy(fechaStr,ent->d_name + strlen(prefijo),8);

            fechaStr[8] = '\0';

            int fechaNum = atoi(fechaStr);

            if(fechaNum > fechaMax)
            {
                fechaMax = fechaNum;

                strcpy(ultimo, ent->d_name);
            }
        }
    }

    closedir(dir);

    if(fechaMax)
    {
        strcpy(nombreUltimo, ultimo);
        return 1;
    }

    return 0;
}




///Csv y las fechas
void auditoriaClientes (const char *ftxt,tda_vec * vec,Sfech *f,long matriz [][2], int *cantErrores, int maxErrores)
{
//    int cantErrores = 0; // ?
    char aux[REGISTRO]; //150
    Ssocio s1;
    FILE *archtxt;
    int tipo;

    //acontinuacion se verifica si los archivos se abrieron correctamente.
    archtxt=fopen(ftxt,"r");
    if(!archtxt)
    {
        printf("ERROR AL ABRIR EL ARCHIVO");
        exit(-1);
    }

    while(fgets(aux, REGISTRO, archtxt))
    {
        cargar_estructura(&s1,aux); // usamos esta funcion para cargar en el struct socios los archivos txt.

        tipo=validarSocio(&s1,f);
        if(tipo==0)
        {
            normalizar(s1.nyp); // normaliza nombre y apellido
            tda_vec_insertar(vec,&s1);
        }

        else
        {
            matriz[*cantErrores][0] = tipo;
            matriz[*cantErrores][1] = s1.dni;
            (*cantErrores)++;
        }
    }

    fclose(archtxt);
}

/// Valida socios ///
int validarSocio(Ssocio* miembro, Sfech *f)
{
    if (!dniEsvalido(miembro->dni))
        return ERR_DNI;

    if (!validarCuil(miembro->dni, miembro->sexo, miembro->cuil))
        return ERR_CUIL;

    if (!nypEsvalido(miembro->nyp))
        return ERR_NYP;

    if (!sexoEsvalido(miembro->sexo))
        return ERR_SEXO;

    if (!estadoEsvalido(miembro->estado))
        return ERR_ESTADO;

    if (!categoriaEsvalida(miembro->categoria))
        return ERR_CATEGORIA;

    if (!validarFecha(&miembro->fechAfilia))
        return ERR_FEC_AFIL;

    if (!validarFecha(&miembro->fechCuotaPaga))
        return ERR_FEC_CUOTA;

    if (!validarFecha(&miembro->fechNac))
        return ERR_FEC_NAC;

    if (!validarFechaMenorDiez(&miembro->fechNac, f))
        return ERR_EDAD;

    if (!fechaMenor(&miembro->fechNac, &miembro->fechAfilia))
        return ERR_AFIL_NAC;

    if (!fechaMenor(&miembro->fechAfilia, f))
        return ERR_AFIL_PROCESO;

    if (!fechaMenor(&miembro->fechCuotaPaga, f))
        return ERR_CUOTA_PROCESO;

    if (!fechaMenor(&miembro->fechAfilia, &miembro->fechCuotaPaga))
        return ERR_CUOTA_AFIL;

    if (!planEsvalido(miembro->plan))
        return ERR_PLAN;

    if (strcmp(miembro->categoria, "MENOR") == 0)
    {
        if (!emailTutorEsvalido(miembro->emailTutor))
            return ERR_EMAIL_TUTOR;
    }

    return SIN_ERROR;
}


/// Carga la estructura ///
void cargar_estructura(Ssocio *s,  char *cad)
{
    int cant = sscanf(cad, "%ld;%[^;];%[^;];%d/%d/%d; %c ;%d/%d/%d;%[^;];%d/%d/%d; %c ;%[^;];%[^;]",
                      &s->dni,
                      s->cuil,
                      s->nyp,
                      &s->fechNac.d, &s->fechNac.m, &s->fechNac.a,
                      &s->sexo,
                      &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a,
                      s->categoria,
                      &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a,
                      &s->estado, s->plan, s->emailTutor);

    if (cant < 17) // No se cargo el email
    {
        strcpy(s->emailTutor, "");

    }
}


/// Contar registros en un archivo ///

int contarLineas(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int contar = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f))
    {
        contar++;
    }
    fclose(f);
    return contar;
}


/// Auditoria de peliculas ///

void auditoriaPeliculas(const char *ftxt,tda_vec*vec,long matriz [][2], int *cantErrores, int maxErrores)
{
    //int cantErrores = 0;
    int tipo;
    int ultimoId = 0;

    char aux[REGISTRO];
    Spelicula p1;

    FILE *archtxt;



    archtxt = fopen(ftxt, "rt");

    if(!archtxt)
    {
        printf("ERROR AL ABRIR EL ARCHIVO\n");
        exit(-1);
    }

    while(fgets(aux, REGISTRO, archtxt))
    {
        cargar_estructuraP(&p1, aux);

        tipo = validarPelicula(&p1, &ultimoId);

        if(tipo == 0)
        {
            tda_vec_insertar(vec, &p1);
        }
        else
        {
            matriz[*cantErrores][0] = tipo;
            matriz[*cantErrores][1] = p1.idPelicula;
            (*cantErrores)++;
        }
    }


    fclose(archtxt);
}


void cargar_estructuraP(Spelicula *s, const char *c)
{
    sscanf(c, "%d;%[^;];%[^;];%d;%c",
           &s->idPelicula,
           s->titulo,
           s->genero,
           &s->stock,
           &s->estado);

}


/// persistenciaEnDisco(nombinC,nombinP,nombinA,errtxtC,errtxtP,&vector_validosClientes,&vector_validosPeliculas,Matriz_auditoriaP,Matriz_auditoriaC)
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
                         int erroresP)

{
    FILE *fpCli, *fpPel, *fpAlq,*fpErrtxtC, *fpErrtxtP ;

    int i;
    const char* vectorErroresC_Nombres[16] =
    {
        "DNI",              // ERR_DNI = 1
        "CUIL",             // ERR_CUIL = 2
        "Nombre y Apellido",// ERR_NYP = 3
        "Sexo",             // ERR_SEXO = 4
        "Estado",           // ERR_ESTADO = 5
        "Categoria",        // ERR_CATEGORIA = 6
        "Fecha Afiliacion", // ERR_FEC_AFIL = 7
        "Fecha Cuota",      // ERR_FEC_CUOTA = 8
        "Fecha Nacimiento", // ERR_FEC_NAC = 9
        "Edad",             // ERR_EDAD = 10
        "Afiliacion vs Nac.",// ERR_AFIL_NAC = 11
        "Afiliacion vs Proceso",// ERR_AFIL_PROCESO = 12
        "Cuota vs Proceso", // ERR_CUOTA_PROCESO = 13
        "Cuota vs Afiliacion",// ERR_CUOTA_AFIL = 14
        "Plan",             // ERR_PLAN = 15
        "Email Tutor"       // ERR_EMAIL_TUTOR = 16
    };


    const char* vectorErroresP_Nombres[3] =
    {
        "ID Autoincremental", // ERROR_ID_AUTOINCREMENTAL = 1
        "Genero",             // ERROR_GENERO = 2
        "Stock"               // ERROR_STOCK = 3
    };



    if (!matAudC || !matAudP)
    {
        puts("las matrices vienen mal cargadas");
        return;
    }


    if(!vecClientes || !vecPeliculas)
        return;

    fpCli = fopen(nomBinC, "wb");
    if(!fpCli)
        return;

    fpPel = fopen(nomBinP, "wb");
    if(!fpPel)
    {
        fclose(fpCli);
        return;
    }

    fpErrtxtC = fopen(errtxtC, "wt");
    if(!fpErrtxtC)
    {
        fclose(fpCli);
        fclose(fpPel);
        return;
    }

    fpErrtxtP = fopen(errtxtP, "wt");
    if(!fpErrtxtP)
    {
        fclose(fpCli);
        fclose(fpPel);
        fclose(fpErrtxtC);
        return;
    }

    fpAlq=fopen(nomBinA,"wb");
    if(!fpAlq)
    {
        fclose(fpCli);
        fclose(fpPel);
        fclose(fpErrtxtC);
        fclose(fpErrtxtP);
        return;
    }


    //  Persistencia binaria
    fwrite(vecClientes->vec,sizeof(Ssocio),vecClientes->ce,fpCli);
    fwrite(vecPeliculas->vec,sizeof(Spelicula),vecPeliculas->ce,fpPel);
    fwrite(vecAlquileres->vec,sizeof(SAlquiler),vecAlquileres->ce,fpAlq);

    for(i = 0; i < erroresC; i++)
    {
        fprintf(fpErrtxtC, "%s;%ld\n",
                vectorErroresC_Nombres[matAudC[i][0]-1],
                matAudC[i][1]);
    }

    printf("ErroresP: %d\n", erroresP);


    for(i = 0; i < erroresP; i++)
    {
        fprintf(fpErrtxtP, "%s;%ld\n",
                vectorErroresP_Nombres[matAudP[i][0]-1],
                matAudP[i][1]);
    }




    fclose(fpCli);
    fclose(fpPel);
    fclose(fpErrtxtC);
    fclose(fpErrtxtP);
    fclose(fpAlq);
}






int cargaBinarioEnMemoriaC(const char *nomBinC, tda_vec *vecClientes)
{
    FILE *fp;
    Ssocio socio;

    fp = fopen(nomBinC, "r+b");
    if(fp == NULL)
        return 0;

    while(fread(&socio, sizeof(Ssocio), 1, fp) == 1)
    {
        puts("hola paola");
        if(!tda_vec_insertar(vecClientes, &socio))
        {
            fclose(fp);

            return 0;
        }
    }

    fclose(fp);
    return 1;
}




int cargaBinarioEnMemoriaA(const char *nomBinA, tda_vec *vecClientes)
{
    FILE *fp;
    SAlquiler socio;

    fp = fopen(nomBinA, "r+b");
    if(fp == NULL)
        return 0;

    while(fread(&socio, sizeof(SAlquiler), 1, fp) == 1)
    {
        if(!tda_vec_insertar(vecClientes, &socio))
        {
            fclose(fp);

            return 0;
        }
    }

    fclose(fp);
    return 1;
}


int cargaBinarioEnMemoriaP (const char *nomBinP, tda_vec *vecPeliculas)
{
    FILE *fp;
    Spelicula p;

    fp = fopen(nomBinP, "r+b");
    if(fp == NULL)
        return 0;

    while(fread(&p, sizeof(Spelicula), 1, fp) == 1)
    {
        if(!tda_vec_insertar(vecPeliculas, &p))
        {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

int obtenerCodigoError(const char *error)
{
    const char* vectorErroresC_Nombres[16] =
    {
        "DNI",              // ERR_DNI = 1
        "CUIL",             // ERR_CUIL = 2
        "Nombre y Apellido",// ERR_NYP = 3
        "Sexo",             // ERR_SEXO = 4
        "Estado",           // ERR_ESTADO = 5
        "Categoria",        // ERR_CATEGORIA = 6
        "Fecha Afiliacion", // ERR_FEC_AFIL = 7
        "Fecha Cuota",      // ERR_FEC_CUOTA = 8
        "Fecha Nacimiento", // ERR_FEC_NAC = 9
        "Edad",             // ERR_EDAD = 10
        "Afiliacion vs Nac.",// ERR_AFIL_NAC = 11
        "Afiliacion vs Proceso",// ERR_AFIL_PROCESO = 12
        "Cuota vs Proceso", // ERR_CUOTA_PROCESO = 13
        "Cuota vs Afiliacion",// ERR_CUOTA_AFIL = 14
        "Plan",             // ERR_PLAN = 15
        "Email Tutor"       // ERR_EMAIL_TUTOR = 16
    };

    for(int i = 0; i < 15; i++)
    {
        if(strcmp(*(vectorErroresC_Nombres+i), error) == 0)
            return i + 1;
    }

    return -1;
}


int cargaTXTaMatrizC(const char *nombreArchivo, long matriz[][2])
{
    FILE *fp;
    char error[50];
    long dni;
    int cantRegistros = 0;

    fp = fopen(nombreArchivo, "rt");
    if(!fp)
    {
        puts("no se pudo abrir el archivo");
        return -1;
    }

    while(fscanf(fp, " %49[^;];%ld", error, &dni) == 2)
    {
            matriz[cantRegistros][0] = obtenerCodigoError(error);
            matriz[cantRegistros][1] = dni;
            cantRegistros++;
    }

    fclose(fp);
    return cantRegistros;
}


int obtenerCodigoErrorP(const char *error)
{

    const char* vectorErroresP_Nombres[3] =
    {
        "ID Autoincremental", // ERROR_ID_AUTOINCREMENTAL = 1
        "Genero",             // ERROR_GENERO = 2
        "Stock"               // ERROR_STOCK = 3
    };

    for(int i = 0; i < 3; i++)
    {
        if(strcmp(*(vectorErroresP_Nombres+i), error) == 0)
            return i + 1;
    }

    return -1;
}



int cargaTXTaMatrizP(const char *nombreArchivo, long matriz[][2])
{
    FILE *fp;
    char error[50];
    long id;
    int cantRegistros = 0;

    fp = fopen(nombreArchivo, "rt");
    if(!fp)
    {
        puts("no se pudo abrir el archivo");
        return -1;
    }

    while(fscanf(fp, " %49[^;];%ld", error, &id) == 2)
    {
        matriz[cantRegistros][0] = obtenerCodigoErrorP(error);
        matriz[cantRegistros][1] = id;

        cantRegistros++;
    }

    fclose(fp);
    return cantRegistros;
}



