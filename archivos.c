#include "archivos.h"

///////////////////    MANIPULACION DE ARCHIVOS  /////////////////
void cargarerror(FILE *binerror, Ssocio *s1, char *cad_error)
{
    limpiar_campo(s1->plan);
    limpiar_campo(s1->emailTutor);
    fprintf(binerror,
            "%ld|%s|%02d/%02d/%04d|%c|%02d/%02d/%04d|%s|%02d/%02d/%04d|%c|%s|",
            s1->dni,
            s1->nyp,
            s1->fechNac.d, s1->fechNac.m, s1->fechNac.a,
            s1->sexo,
            s1->fechAfilia.d, s1->fechAfilia.m, s1->fechAfilia.a,
            s1->categoria,
            s1->fechCuotaPaga.d, s1->fechCuotaPaga.m, s1->fechCuotaPaga.a,
            s1->estado,
            s1->plan);

    if (s1->emailTutor[0] != '\0' && strcmp(s1->emailTutor, " ") != 0)
        fprintf(binerror, "%s", s1->emailTutor);
    fprintf(binerror, "\n");
    fprintf(binerror, "Motivo: %s\n", cad_error);
}

void cargar_estructura(Ssocio *s, const char *c)
{
    int cant = sscanf(c, "%ld|%[^|]|%d/%d/%d|%c|%d/%d/%d|%[^|]|%d/%d/%d|%c|%[^|]|%[^|]",
                      &s->dni, s->nyp,
                      &s->fechNac.d, &s->fechNac.m, &s->fechNac.a,
                      &s->sexo,
                      &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a,
                      s->categoria,
                      &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a,
                      &s->estado, s->plan, s->emailTutor);

    if (cant < 16) // No se cargo el email
    {
        strcpy(s->emailTutor, "\n");
        sscanf(c, "%ld|%[^|]|%d/%d/%d|%c|%d/%d/%d|%[^|]|%d/%d/%d|%c|%[^|]",
               &s->dni, s->nyp,
               &s->fechNac.d, &s->fechNac.m, &s->fechNac.a,
               &s->sexo,
               &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a,
               s->categoria,
               &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a,
               &s->estado, s->plan);
    }
}

int validarSocio(Ssocio *s, Sfech *f, char *cad_error)
{
    if (!dniEsvalido(s->dni))
    {
        strcpy(cad_error, "DNI invalido\n");
        return 0;
    }
    else if (!nypEsvalido(s->nyp))
    {
        strcpy(cad_error, "Nombre y apellido invalido\n");
        return 0;
    }
    else if (!sexoEsvalido(s->sexo))
    {
        strcpy(cad_error, "Sexo invalido\n");
        return 0;
    }
    else if (!estadoEsvalido(s->estado))
    {
        strcpy(cad_error, "Estado invalido\n");
        return 0;
    }
    else if(!categoriaEsvalida(s->categoria))
    {
        strcpy(cad_error, "Categoria invalida\n");
        return 0;
    }
    else if (!validarFecha(&s->fechAfilia))
    {
        strcpy(cad_error, "Fecha de afiliacion invalida\n");
        return 0;
    }
    else if(!validarFecha(&s->fechCuotaPaga))
    {
        strcpy(cad_error, "Fecha de ultima cuota invalida\n");
        return 0;
    }
    else if (!validarFecha(&s->fechNac))
    {
        strcpy(cad_error, "Fecha de nacimiento invalida\n");
        return 0;
    }
    else if (!validarFechaMenorDiez(&s->fechNac, f))
    {
        strcpy(cad_error, "El socio debe tener al menos 10 anios\n");
        return 0;
    }
    else if (!fechaMenor(&s->fechNac, &s->fechAfilia))
    {
        strcpy(cad_error, "La afiliacion no puede ser anterior a la fecha de nacimiento\n");
        return 0;
    }
    else if (!fechaMenor(&s->fechAfilia, f))
    {
        strcpy(cad_error, "Fecha de afiliacion posterior a la fecha de proceso\n");
        return 0;
    }
    else if (!fechaMenor(&s->fechCuotaPaga, f))
    {
        strcpy(cad_error, "Fecha de ultima cuota posterior a la fecha de proceso\n");
        return 0;
    }
    else if (!fechaMenor(&s->fechAfilia, &s->fechCuotaPaga))
    {
        strcpy(cad_error, "La ultima cuota no puede ser anterior a la afiliacion\n");
        return 0;
    }
    else if (!planEsvalido(s->plan))
    {
        strcpy(cad_error, "Plan invalido\n");
        return 0;
    }
    else if (strcmp(s->categoria, "MENOR") == 0)
    {
        if (!emailTutorEsvalido(s->emailTutor))
        {
            strcpy(cad_error, "Email del tutor invalido\n");
            return 0;
        }
    }

    return 1; // Todo válido
}

void archtextbin(const char *ftxt, const char *fbin, const char *error, Sfech *f)
{

    char aux[REGISTRO];
    char cad_error[100];
    Ssocio s1;
    FILE *archtxt;
    FILE *archbin;
    FILE *archerror;

    //acontinuacion se verifica si los archivos se abrieron correctamente.
    archtxt=fopen(ftxt,"r");
    if(!archtxt)
    {
        printf("ERROR AL ABRIR EL ARCHIVO");
        exit(-1);
    }

    archbin=fopen(fbin,"wb");
    if(!archbin)
    {
        printf("ERROR AL ABRIR EL ARCHIVO");
        fclose(archtxt);
        exit(-1);
    }

    archerror=fopen(error,"w");
    if(!archerror)
    {
        printf("ERROR AL ABRIR EL ARCHIVO");
        fclose(archtxt);
        fclose(archbin);
        exit(-1);
    }

    while(fgets(aux, REGISTRO, archtxt))
    {
        cargar_estructura(&s1, aux); // usamos esta funcion para cargar en el struct socios los archivos txt.
        if(validarSocio(&s1, f, cad_error))
        {
            normalizar(s1.nyp); // normaliza nombre y apellido
            fwrite(&s1, sizeof(Ssocio), 1, archbin); //una vez validado los diferentes campos se hace la escritura.
        }
        else
        {
            cargarerror(archerror, &s1, cad_error); //crea y graba los errores en un txt
        }
    }

    fclose(archtxt);
    fclose(archbin);
    fclose(archerror);
}

int elegir_archivo_base(char *archivoBase, Sfech *fechaProceso)
{
    char archivoUltimo[MAX_PATH];
    char archivoNuevo[MAX_PATH];

    int existePrevio = buscar_ultimo_archivo(archivoUltimo);
    if (existePrevio)
    {
        printf("\nSe encontro un archivo previo: %s\n", archivoUltimo);
        printf("Desea trabajar con el archivo recuperado? S(Si) / N(No): ");
        char opcion;
        scanf("%c", &opcion);
        while(opcion != 'S' && opcion != 's' && opcion != 'N' && opcion != 'n')
        {
            fflush(stdin);
            printf("Error. Opcion invalida.\n");
            printf("Desea trabajar con el archivo recuperado? S(Si) / N(No): ");
            scanf("%c", &opcion);
        }
        if (opcion == 'S' || opcion == 's')
        {
            strcpy(archivoBase, archivoUltimo);
            return 1;
        }
        else
        {
            sprintf(archivoNuevo, "miembros-VC-%04d%02d%02d.dat", fechaProceso->a, fechaProceso->m, fechaProceso->d);
            strcpy(archivoBase, archivoNuevo);
            printf("\nSe usara el archivo original y se generara: %s\n", archivoNuevo);
            return 0;
        }
    }
    else
    {
        printf("\nNo se encontro ningun archivo previo. Se usara el original.\n");
        sprintf(archivoNuevo, "miembros-VC-%04d%02d%02d.dat", fechaProceso->a, fechaProceso->m, fechaProceso->d);
        strcpy(archivoBase, archivoNuevo);
        return 0;
    }
}

int buscar_ultimo_archivo(char *nombreUltimo)
{
    DIR *dir = opendir(".");
    if (!dir)
        return 0;

    struct dirent *ent;
    char ultimo[50] = "";
    int fechaMax = 0;

    while ((ent = readdir(dir)) != NULL)
    {
        if (strncmp(ent->d_name, "miembros-VC-", 12) == 0 && strstr(ent->d_name, ".dat"))
        {
            // Extraer los 8 dígitos de la fecha dentro del nombre
            char fechaStr[9];
            strncpy(fechaStr, ent->d_name + 12, 8);
            fechaStr[8] = '\0';

            int fechaNum = atoi(fechaStr);
            if (fechaNum > fechaMax)
            {
                fechaMax = fechaNum;
                strcpy(ultimo, ent->d_name);
            }
        }
    }
    closedir(dir);

    if (fechaMax)
    {
        strcpy(nombreUltimo, ultimo);
        return 1; // Encontró un archivo previo
    }
    return 0; // No encontró ninguno
}
