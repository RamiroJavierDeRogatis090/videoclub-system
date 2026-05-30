#include "menu.h"

void resuelveA(t_indice *indi, FILE *archbin, Ssocio *s, Sfech *hoy,char *cad_error)
{
    long dni;
    int pos;
    t_reg_indice reg;

    if (!indi || !archbin || !s || !hoy || !indi->vindice)
    {
        printf("Error: parametros invalidos.\n");
        return;
    }

    printf("Ingrese DNI para dar de alta a nuevo socio: ");
    if (scanf("%ld", &dni) != 1 || !dniEsvalido(dni))
    {
        printf("Error: DNI invalido.\n");
        while (getchar() != '\n');
        return;
    }

    reg.dni = dni;
    reg.nro_reg = 0;

    pos = indice_buscar(indi, &reg, indi->cant, sizeof(t_reg_indice), cmp_indice);

    if (pos != -1)
    {
        printf("Error: El socio ya esta ingresado en el sistema.\n");
        return;
    }

    printf("Ingrese apellidos y nombres: ");
    getchar();
    fgets(s->nyp, sizeof(s->nyp), stdin);
    size_t len = mystrlen(s->nyp);
    if (len > 0 && s->nyp[len - 1] == '\n')
        s->nyp[len - 1] = '\0';
    eliminarEspaciosExtremos(s->nyp);
    eliminarEspaciosRepetidos(s->nyp);
    normalizar(s->nyp);

    printf("Fecha de nacimiento (dd/mm/aaaa): ");
    if (scanf("%d/%d/%d", &s->fechNac.d, &s->fechNac.m, &s->fechNac.a) != 3)
    {
        printf("Error: Fecha de nacimiento invalida.\n");
        return;
    }

    printf("Sexo (M/F): ");
    scanf(" %c", &s->sexo);
    if (!sexoEsvalido(s->sexo))
    {
        printf("Error: Sexo invalido.\n");
        return;
    }

    printf("Fecha de afiliacion (dd/mm/aaaa): ");
    if (scanf("%d/%d/%d", &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a) != 3)
    {
        printf("Error: Fecha de afiliación invalida.\n");
        return;
    }

    printf("Categoria: ");
    scanf("%s", s->categoria);
    if (!categoriaEsvalida(s->categoria))
    {
        printf("Error: Categoria invalida.\n");
        return;
    }

    printf("Fecha ultima cuota (dd/mm/aaaa): ");
    if (scanf("%d/%d/%d", &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a) != 3)
    {
        printf("Error: Fecha de ultima cuota invalida.\n");
        return;
    }

    printf("Plan: ");
    scanf("%s", s->plan);
    if (!planEsvalido(s->plan))
    {
        printf("Error: Plan invalido.\n");
        return;
    }

    int edad = hoy->a - s->fechNac.a;
    if (hoy->m < s->fechNac.m || (hoy->m == s->fechNac.m && hoy->d < s->fechNac.d))
        edad--;

    if (edad < 18)
    {
        printf("El socio es menor de edad.\n");
        printf("Ingrese email del tutor: ");
        scanf("%s", s->emailTutor);
        if (!emailTutorEsvalido(s->emailTutor))
        {
            printf("Error: Formato de email invalido.\n");
            return;
        }
    }
    else
    {
        strcpy(s->emailTutor, "");
    }

    s->dni = dni;
    s->estado = 'A';
    if (!estadoEsvalido(s->estado))
    {
        printf("Error: Estado invalido.\n");
        return;
    }

    if (!validarSocio(s, hoy,cad_error))
    {
        printf("Datos invalidos: %s\n",cad_error );
        return;
    }

    // Persistencia del socio
    fseek(archbin, 0, SEEK_END);
    reg.nro_reg = ftell(archbin) / sizeof(Ssocio);
    fwrite(s, sizeof(Ssocio), 1, archbin);
    fflush(archbin); // Asegura escritura inmediata

    // Actualización del índice en memoria
    indice_insertar(indi, &reg, sizeof(t_reg_indice), cmp_indice);

    printf("Socio registrado exitosamente.\n");
}

void resuelveB(t_indice *indi, FILE*archbin, Ssocio* s)
{
    long dni;
    int pos;
    t_reg_indice reg;

    printf("Ingrese DNI del socio que desea dar de BAJA: ");
    scanf("%ld",&dni);

    reg.dni = dni;
    reg.nro_reg= 0;

    pos=indice_buscar(indi, &reg, indi->cant, sizeof(t_reg_indice), cmp_indice);

    if(pos==-1)
        printf("Error en socio: DNI no encontrado. \n");
    else
    {
        t_reg_indice *encontrado = (t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));
        fseek(archbin, encontrado->nro_reg * sizeof(Ssocio), SEEK_SET);
        if (fread(s, sizeof(Ssocio), 1, archbin) != 1)
        {
            printf("Error al leer el registro del archivo.\n");
            return;
        }
        s->estado = 'B';
        fseek(archbin, -((long)sizeof(Ssocio)), SEEK_CUR);
        fwrite(s, sizeof(Ssocio), 1, archbin);
        indice_eliminar(indi, &reg, sizeof(t_reg_indice), cmp_indice);
        printf("Socio eliminado del sistema.\n");
    }
}

void resuelveC(t_indice *indi, FILE*archbin,Ssocio *s, Sfech *hoy)
{
    long dni;
    int pos,op, cod = 1;
    t_reg_indice reg;
    Ssocio *st=s;

    printf("Ingrese DNI del socio a modificar: ");
    scanf("%ld",&dni);

    reg.dni=dni;
    reg.nro_reg=0;

    pos=indice_buscar(indi, &reg, indi->cant, sizeof(t_reg_indice), cmp_indice);

    if(pos==-1)
    {
        printf("Error en socio: DNI no encontrado. \n");
        return;
    }

    t_reg_indice *encontrado = (t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));
    fseek(archbin, encontrado->nro_reg * sizeof(Ssocio), SEEK_SET);
    fread(s,sizeof(Ssocio),1,archbin);

    printf("Datos actuales:\n");
    printf("1. Apellidos y nombres: %s\n", st->nyp);
    printf("2. Sexo: %c \n",st->sexo);
    printf("3. Categoria: %s\n",st->categoria);
    printf("4. Fecha ultima cuota: %d/%d/%d\n",st->fechCuotaPaga.d,st->fechCuotaPaga.m,st->fechCuotaPaga.a);
    printf("5. Fecha de afiliacion: %d/%d/%d\n",st->fechAfilia.d,st->fechAfilia.m,st->fechAfilia.a);
    printf("6. Fecha de nacimiento: %d/%d/%d\n",st->fechNac.d,st->fechNac.m,st->fechNac.a);
    printf("7. Plan: %s\n",st->plan);
    printf("8. Email Tutor: %s\n",st->emailTutor);

    printf("\n Ingrese numero de campo que desea modificar (1-8): ");
    scanf("%d",&op);

    switch(op)
    {
    case 1:
        printf("Nuevo apellido y nombre: ");
        getchar();
        fgets(s->nyp, sizeof(s->nyp), stdin);
        size_t len = mystrlen(s->nyp);
        if (len > 0 && s->nyp[len - 1] == '\n')
            s->nyp[len - 1] = '\0';
        eliminarEspaciosExtremos(s->nyp);
        eliminarEspaciosRepetidos(s->nyp);
        normalizar(s->nyp);
        break;
    case 2:
        fflush(stdin);
        printf("Actualizacion de sexo: ");
        scanf(" %c", &s->sexo);
        if (!sexoEsvalido(s->sexo))
        {
            printf("Error: Sexo invalido.\n");
            cod = 0;
        }
        break;
    case 3:
        fflush(stdin);
        printf("Nueva categoria: ");
        scanf("%s", s->categoria);
        if (!categoriaEsvalida(s->categoria))
        {
            printf("Error: Categoria invalida.\n");
            cod = 0;
        }
        pasarMayusculas(s->categoria);
        if(strcmp(s->categoria, "MENOR") == 0)
        {
            fflush(stdin);
            printf("Ingrese email de tutor al ser menor de edad: ");
            scanf("%s", s->emailTutor);
            if (!emailTutorEsvalido(s->emailTutor))
            {
                printf("Error: Formato de email invalido.\n");
                cod = 0;
            }
        }
        break;
    case 4:
        printf("Nueva fecha ultima cuota (dd/mm/aaaa): ");
        if (scanf("%d/%d/%d", &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a) != 3)
        {
            printf("Error: Fecha de ultima cuota invalida.\n");
            cod = 0;
        }
        break;
    case 5:
        printf("Nueva fecha de afiliacion (dd/mm/aaaa):");
        if (scanf("%d/%d/%d", &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a) != 3)
        {
            printf("Error: Fecha de afiliación invalida.\n");
            cod = 0;
        }
        break;
    case 6:
        printf("Nueva fecha de nacimiento (dd/mm/aaaa):");
        if (scanf("%d/%d/%d", &s->fechNac.d, &s->fechNac.m, &s->fechNac.a) != 3)
        {
            printf("Error: Fecha de nacimiento invalida.\n");
            cod = 0;
        }
        break;
    case 7:
        fflush(stdin);
        printf("Nuevo plan: ");
        scanf("%s", s->plan);
        if (!planEsvalido(s->plan))
        {
            printf("Error: Plan invalido.\n");
            cod = 0;
        }
        break;
    case 8:
        fflush(stdin);
        printf("Nuevo email de tutor: ");
        scanf("%s", s->emailTutor);
        if (!emailTutorEsvalido(s->emailTutor))
        {
            printf("Error: Formato de email invalido.\n");
            cod = 0;
        }
        break;
    }

    if(cod == 1) //uso para validar que lo ingresado este bien
    {
        fseek(archbin, encontrado->nro_reg * sizeof(Ssocio), SEEK_SET);
        fwrite(s, sizeof(Ssocio), 1, archbin);
        printf("Socio modificado exitosamente\n");
    }
    else
        printf("Error en actualizacion, datos invalidos");
}

void resuelveD(t_indice *indi, FILE*archbin, Ssocio*s)
{
    long dni;
    int pos;
    t_reg_indice reg;
    printf("Ingrese DNI para mostrar informacion: ");
    scanf("%ld",&dni);

    reg.dni=dni;
    reg.nro_reg=0;

    pos=indice_buscar(indi, &reg, indi->cant, sizeof(t_reg_indice), cmp_indice);

    if(pos==-1)
        printf("Error en socio: DNI no encontrado. \n");
    else
    {
        t_reg_indice *encontrado = (t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));

        fseek(archbin, encontrado->nro_reg * sizeof(Ssocio), SEEK_SET);
        if (fread(s, sizeof(Ssocio), 1, archbin) != 1)
        {
            printf("Error al leer el registro del archivo.\n");
            return;
        }
        printf("\nInformacion del socio\n");
        printf("DNI: %ld\n", s->dni);
        printf("Apellidos y nombres: %s\n", s->nyp);
        printf("Fecha de nacimiento: %d/%d/%d\n",s->fechNac.d,s->fechNac.m,s->fechNac.a);
        printf("Sexo: %c\n", s->sexo);
        printf("Fecha de afiliacion: %d/%d/%d\n",s->fechAfilia.d,s->fechAfilia.m,s->fechAfilia.a);
        printf("Categoria: %s\n", s->categoria);
        printf("Fecha ultima cuota: %d/%d/%d\n",s->fechCuotaPaga.d,s->fechCuotaPaga.m,s->fechCuotaPaga.a);
        printf("Plan: %s\n", s->plan);
        printf("Email Tutor: %s\n", s->emailTutor);
    }
}

void resuelveE(t_indice *indi, FILE*archbin, Ssocio *socio)
{

    printf("\nListado de socios ordenado por DNI:\n");
    printf("%-10s %-60s %-5s %-12s %-14s %-12s %-12s %-8s %-10s %-25s\n",
           "DNI", "Apellidos y nombres", "Sexo", "F.Nac.",
           "F.Afiliacion", "Categoria", "Ult.Cuota", "Estado",
           "Plan", "Email Tutor");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    t_reg_indice *ini = (t_reg_indice *)indi->vindice;
    t_reg_indice *fin = ini + indi->cant;
    while(ini<fin)
    {
        fseek(archbin,ini->nro_reg*sizeof(Ssocio),SEEK_SET);
        fread(socio, sizeof(Ssocio), 1,archbin);
        if(socio->estado == 'A')
        {
            printf("%-10ld %-60s %-5c %02d/%02d/%04d   %02d/%02d/%04d     %-12s %02d/%02d/%04d   %-8c %-10s %-25s\n",
                   socio->dni,socio->nyp,socio->sexo,
                   socio->fechNac.d,socio->fechNac.m,socio->fechNac.a,
                   socio->fechAfilia.d,socio->fechAfilia.m,socio->fechAfilia.a,
                   socio->categoria,
                   socio->fechCuotaPaga.d,socio->fechCuotaPaga.m,socio->fechCuotaPaga.a,
                   socio->estado,
                   socio->plan,
                   socio->emailTutor);
        }
        ini++;
    }
}

void resuelveF(t_indice *indi, FILE *archbin, Ssocio *socio)
{
    const char *planes[] = {"BASIC", "PREMIUM", "VIP", "FAMILY"};
    const int cant_planes = sizeof(planes) / sizeof(planes[0]);

    t_reg_indice *v = (t_reg_indice *)indi->vindice;

    printf("\nListado de miembros activos agrupados por Plan y ordenados por DNI:\n");
    printf("%-10s %-60s %-5s %-12s %-14s %-12s %-12s %-8s %-10s %-25s\n",
           "DNI", "Apellidos y nombres", "Sexo", "F.Nac.",
           "F.Afiliacion", "Categoria", "Ult.Cuota", "Estado",
           "Plan", "Email Tutor");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int p = 0; p < cant_planes; p++)
    {
        for (int i = 0; i < indi->cant; i++)
        {
            fseek(archbin, v[i].nro_reg * sizeof(Ssocio), SEEK_SET);
            fread(socio, sizeof(Ssocio), 1, archbin);

            if (socio->estado == 'A' && strcmp(socio->plan, planes[p]) == 0)
            {
                printf("%-10ld %-60s %-5c %02d/%02d/%04d   %02d/%02d/%04d     %-12s %02d/%02d/%04d   %-8c %-10s %-25s\n",
                       socio->dni, socio->nyp, socio->sexo,
                       socio->fechNac.d, socio->fechNac.m, socio->fechNac.a,
                       socio->fechAfilia.d, socio->fechAfilia.m, socio->fechAfilia.a,
                       socio->categoria,
                       socio->fechCuotaPaga.d, socio->fechCuotaPaga.m, socio->fechCuotaPaga.a,
                       socio->estado,
                       socio->plan,
                       socio->emailTutor);
            }
        }
    }
}

////////////////////////  MANIPULACION DE MENU  ////////////////////////
char menu(char m[][L_MENU], const char *tit)
{
    char op;
    op= opcion(m,tit,"Ingrese opcion");
    while(!mystrchr(m[0],op))
        op=opcion(m,tit,"Opcion erronea. Ingrese nuevamente");

    return op;
}

char opcion(char m[][L_MENU], const char *tit, const char *msj)
{
    char op;
    int i;
    system("CLS");
    printf("\n\n %s \n",tit);
    for(i=1; i<=mystrlen(m[0]); i++)
        printf("\n %c - %s",m[0][i-1],m[i]);

    printf("\n\n %s: ",msj);
    fflush(stdin);
    scanf("%c",&op);
    op=mytolower(op);
    return op;
}

void switchmenu(t_indice *indi, Sfech *f, const char *nombin)
{
    char opciones[][L_MENU]= {"abcdefg",
                              "Alta",
                              "Baja",
                              "Modificacion",
                              "Mostrar informacion de un socio",
                              "Listado de miembros ordenados por DNI",
                              "Listado de todos los miembros agrupados por plan",
                              "Salir"
                             };
    char op;

    FILE *archBin = fopen(nombin,"r+b");
    if(!archBin)
        exit(-1);
    Ssocio s;
    char cadenaError [50];

    do
    {
        op=menu(opciones,"Menu Principal");

        switch(op)
        {
        case 'a':
            resuelveA(indi, archBin, &s, f, cadenaError);
            system("pause");
            break;
        case 'b': //Si existe, actualizar ARCHBIN con B y eliminar registro del indice
            resuelveB(indi, archBin, &s);
            system("pause");
            break;
        case 'c': //Si existe, actualizar info. Si ingreso datos erroneos, ignorar todo
            resuelveC(indi, archBin, &s, f);
            system("pause");
            break;
        case 'd': /*si existe, mostrar toda la linea de info*/
            resuelveD(indi, archBin, &s);
            system("pause");
            break;
        case 'e': /*mostrar todos los ACTIVOS(alta) ordenados por DNI*/
            resuelveE(indi, archBin, &s);
            system("pause");
            break;
        case 'f': /*mostrar los miembros activos agrupados por Plan y a su vez ordenados por DNI*/
            resuelveF(indi, archBin, &s);
            system("pause");
            break;
        }
    }
    while(op!='g');

    fclose(archBin);
}
