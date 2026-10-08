#include "menu.h"
#include "tda.h"
#include "validacionesClientes.h"
#include "validacionesTitulos.h"
#include "macros.h"
#include "funcionesDeLibreria.h"

///Funcion para limpiar pantalla (Es generica, funciona para linux o windows)
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void switchmenu(t_indice *indiC,t_indice* indiP,t_indice* indiAYN,tda_vec*vector_alquileres,tda_vec*vector_validosClientes,tda_vec*vector_validosPeliculas,
                long Matriz_auditoriaP[][2], long Matriz_auditoriaC[][2],Sfech *f, int erroresC, int erroresP )
{
    char opciones[][L_MENU]= {"abcdefghijkl",
                              "Alta de un miembro",
                              "Alta de un titulo",
                              "Baja de un miembro",
                              "Baja de un titulo",
                              "Modificacion de un miembro",
                              "Modificacion de un titulo",
                              "Mostrar informacion de un miembro",
                              "Alquiler de un titulo",
                              "Listado de miembros ordenados por DNI",
                              "Listado miembros por Plan",
                              "Mostrar Incidencias de errores por archivo",
                              "salir"
                             };
    char op;
    Ssocio s;
    Spelicula p;
    printf("Entre al menu\n");




    do
    {
        op=menu(opciones,"Menu Principal");

        switch(op)
        {
        case 'a': /// Alta de un miembro
            limpiarPantalla();
            resuelveA(indiC,indiAYN,vector_validosClientes, &s, f);
            system("pause");
            break;
        case 'b': /// Alta de un titulo
            limpiarPantalla();
            resuelveB(indiP,vector_validosPeliculas,&p);
            system("pause");
            break;
        case 'c': /// Baja de un miembro
            limpiarPantalla();
            resuelveC(indiC,indiAYN,vector_validosClientes);
            system("pause");
            break;
        case 'd': /// Baja de un titulo
            limpiarPantalla();
            resuelveD(indiP, vector_validosPeliculas);
            system("pause");
            break;
        case 'e': /// Modificacion de un miembro
            limpiarPantalla();
            resuelveE(indiC, vector_validosClientes, f);
            system("pause");
            break;
        case 'f': /// Modificacion de un titulo
            limpiarPantalla();
            resuelveF(indiP,vector_validosPeliculas);
            system("pause");
            break;
        case 'g': /// Mostrar informacion de un miembro
            limpiarPantalla();
            resuelveG(indiC, vector_validosClientes);
            system("pause");
            break;

        case 'h': /// Alquiler de un titulo
            limpiarPantalla();
            resuelveH(indiC,indiP,vector_validosClientes,vector_validosPeliculas,vector_alquileres);
            system("pause");
            break;

        case 'i': /// Listado de miembros ordenados por DNI
            limpiarPantalla();
            resuelveI(indiC,vector_validosClientes);
            system("pause");
            break;

        case 'j': /// Listado miembros por Plan
            limpiarPantalla();
            resuelveJ(indiAYN, vector_validosClientes);
            system("pause");
            break;

        case 'k': ///Mostrar Incidencias de errores por archivo
            limpiarPantalla();
            resuelveK(Matriz_auditoriaC,Matriz_auditoriaP,erroresC,erroresP);
            system("pause");
            break;

        case 'l': ///termina el programa
            break;
        }
    }
    while(op!='l');
}



char menu(char m[][L_MENU], const char *tit)
{
    char op;
    op=opcion(m,tit,"Ingrese opcion");
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


void resuelveA(t_indice *indi, t_indice* indiAYP , tda_vec *vectorValidos, Ssocio *s, Sfech *hoy)
{
    long dni;
    const char *cad_error;
    int pos;
    t_reg_indice reg;
    t_reg_indice_ayn reg2;

    if (!indi || !vectorValidos || !s || !hoy || !indi->vindice || !indiAYP)
    {
        printf("Error: parametros invalidos.\n");
        return;
    }



    /// Ingreso de datos sin validaciones inmediatas
    printf("Ingrese DNI para dar de alta a nuevo socio: ");
    scanf("%ld", &dni);
    reg.dni = dni;
    reg.nro_reg = 0;
    s->dni = dni;

    pos = indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_indice);
    if (pos != -1)
    {
        printf("Error: El socio ya esta ingresado en el sistema.\n");
        return;
    }



    printf("Sexo (M/F): ");
    scanf(" %c", &s->sexo);
    s->sexo= mytoupper(s->sexo);

    fflush(stdin);

    printf("Ingrese su CUIL: ");
    scanf("%11s", s->cuil);
    fflush(stdin);

    printf("Ingrese apellidos y nombres: ");
    fgets(s->nyp, sizeof(s->nyp), stdin);
    size_t len = mystrlen(s->nyp);
    if (len > 0 && s->nyp[len - 1] == '\n')
        s->nyp[len - 1] = '\0';

    normalizar(s->nyp);
    fflush(stdin);

    printf("Fecha de nacimiento (dd/mm/aaaa): ");
    fflush(stdin);
    scanf("%d/%d/%d", &s->fechNac.d, &s->fechNac.m, &s->fechNac.a);

    printf("Fecha de afiliacion (dd/mm/aaaa): ");
    fflush(stdin);
    scanf("%d/%d/%d", &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a);

    printf("Categoria: ");
    fflush(stdin);
    scanf("%s", s->categoria);

    printf("Fecha ultima cuota (dd/mm/aaaa): ");
    fflush(stdin);
    scanf("%d/%d/%d", &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a);

    printf("Plan: ");
    fflush(stdin);
    scanf("%s", s->plan);

    int edad = hoy->a - s->fechNac.a;
    if (hoy->m < s->fechNac.m || (hoy->m == s->fechNac.m && hoy->d < s->fechNac.d))
        edad--;

    if (edad < 18)
    {
        printf("El socio es menor de edad.\n");
        printf("Ingrese email del tutor: ");
        scanf("%s", s->emailTutor);
    }
    else
    {
        strcpy(s->emailTutor, "");
    }

    s->estado = 'A';

    /// Validación final en bloque
    cad_error = validarSocioRetornaCadena(s, hoy);
    if (cad_error)
    {
        printf("\nDATOS INVALIDOS: %s\n", cad_error);
        return;
    }

    /// Persistencia e índice
    reg.nro_reg = vectorValidos->ce;
    reg2.nro_reg=vectorValidos->ce;
    strcpy(reg2.nyp, s->nyp);

    tda_vec_insertar(vectorValidos, s);
    indice_insertar(indi, &reg, sizeof(t_reg_indice), cmp_indice);
    indice_insertar(indiAYP,&reg2,sizeof(t_reg_indice_ayn),cmp_indice_AYN);

    printf("Socio registrado exitosamente.\n");
}

void resuelveB (t_indice *indiP,tda_vec * vectorValidos, Spelicula *p)
{
    // int ultimoid;
    int id;
    p->estado = 'A';
    const char*cad_error;
    int pos;
    t_reg_indice_pelicula reg;
    if (!indiP || !vectorValidos || !p || !indiP->vindice)
    {
        printf("Error: parametros invalidos.\n");
        return;
    }

    ///Para controlar lo que seria los id  esto se borra
    t_reg_indice_pelicula*aux=indiP->vindice;

    for(int i = 0; i < indiP->cantidad_elementos_actual; i++, aux++)
    {
        printf("%d\n", aux->id_pelicula);
    }

    id = generarNuevoId(indiP);

    printf("ID asignado automaticamente: %d\n", id);
    fflush(stdin);

    p->idPelicula = id;

    reg.id_pelicula = id;

    reg.nro_reg=0;

    pos = indice_buscar(indiP, &reg, indiP->cantidad_elementos_actual, sizeof(t_reg_indice_pelicula), cmp_indice_p);
    if (pos != -1)
    {
        printf("Error: El id ya esta ingresado en el sistema.\n");
        return;
    }


    printf("Ingrese el nombre del titulo: ");
    fgets(p->titulo, sizeof(p->titulo), stdin);
    size_t len = mystrlen(p->titulo);
    if (len > 0 && p->titulo[len - 1] == '\n')
        p->titulo[len - 1] = '\0';


    eliminarEspaciosExtremos(p->titulo);
    eliminarEspaciosRepetidos(p->titulo);
    normalizar(p->titulo);

    if(!*p->titulo)
    {

        printf("Error: Debe ingresar un titulo.\n");
        return;
    }


    printf("ingrese el genero de la pelicula:");
    scanf("%s", p->genero);

    printf("ingrese el stock correspondiente:");
    scanf("%d", &p->stock);


    /// encontrar el error del socio cargado
    cad_error=validarPeliculaRetornaCadena(p);
    if (cad_error)
    {
        printf("Datos invalidos: %s\n",cad_error );
        return;
    }

        tda_vec_insertar(vectorValidos,p);

    reg.id_pelicula = p->idPelicula;

    reg.nro_reg = vectorValidos->ce-1;
    // Persistencia del socio en el vector TDA

    // Actualización del índice en memoria
    indice_insertar(indiP, &reg, sizeof(t_reg_indice_pelicula), cmp_indice_p);

    printf("PELICULA registrada exitosamente.\n");

}


const char* validarPeliculaRetornaCadena(Spelicula *p)
{

    if(!generoEsvalido(p->genero))
        return ("El genero ingresado no es correcto");

    else if(!cantidadVHSD(&p->stock))
        return ("El valor de stock ingresado no es correcto");

    return 0;
}


void resuelveC (t_indice *indi,t_indice * indiAYP, tda_vec *vectorValidos)
{
    long dni;
    int pos;
    char seguridad;
    t_reg_indice reg;
    t_reg_indice_ayn reg2;

    printf("Ingrese DNI del socio que desea dar de BAJA: ");
    scanf("%ld", &dni);

    reg.dni = dni;
    reg.nro_reg = 0;

    pos = indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_indice);

    if(pos == -1)
    {
        printf("Error: DNI no encontrado.\n");
        return;
    }

    do{
    printf("Esta seguro que desea eliminar a este socio? S(si)/N(no):");
    scanf(" %c", &seguridad);
    while(getchar() != '\n');

    }while(seguridad != 'S' && seguridad != 's' &&
                seguridad != 'N' && seguridad != 'n');

    if(seguridad == 'S' || seguridad == 's'){
        t_reg_indice *encontrado =(t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));

        Ssocio *socio =(Ssocio *)((char *)vectorValidos->vec + encontrado->nro_reg * sizeof(Ssocio));

        socio->estado = 'B';

        reg2.nro_reg = encontrado->nro_reg;
        strcpy(reg2.nyp, socio->nyp);

        indice_eliminar(indi, &reg,sizeof(t_reg_indice), cmp_indice);

        indice_eliminar(indiAYP,&reg2,sizeof(t_reg_indice_ayn),cmp_indice_AYN);

        printf("\nSocio eliminado del sistema.\n");
    }
    else
        printf("\nEl socio NO fue eliminado\n");
}

void resuelveD (t_indice *indi, tda_vec *vectorValidos)
{
    int id;
    int pos;
    char seguridad;
    t_reg_indice_pelicula reg;

    printf("Ingrese ID de la pelicula que desea dar de BAJA: ");
    scanf("%d", &id);

    reg.id_pelicula = id;
    reg.nro_reg = 0;

    pos = indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice_pelicula), cmp_indice_p);

    if(pos == -1)
    {
        printf("Error: id no encontrado.\n");
        return;
    }

    do{
    printf("Esta seguro que desea eliminar a esta pelicula? S(si)/N(no):");
    scanf(" %c", &seguridad);
    while(getchar() != '\n');

    }while(seguridad != 'S' && seguridad != 's' &&
                seguridad != 'N' && seguridad != 'n');

    if(seguridad == 'S' || seguridad == 's'){
    t_reg_indice_pelicula *encontrado =(t_reg_indice_pelicula *)((char *)indi->vindice + pos * sizeof(t_reg_indice_pelicula));

    Spelicula *p =(Spelicula *)((char *)vectorValidos->vec + encontrado->nro_reg * sizeof(Spelicula));

    p->estado = 'B';

    indice_eliminar(indi, &reg,sizeof(t_reg_indice_pelicula), cmp_indice_p);

    printf("Pelicula eliminada del sistema.\n");
    }
    else
        printf("\nLa pelicula NO fue eliminada\n");
}

/// Modificacion de un miembro ///
void resuelveE(t_indice *indi, tda_vec *vectorValidos, Sfech *hoy)
{
    long dni;
    int pos,op, cod = 0;
    t_reg_indice reg;
    t_reg_indice *encontrado;
    Ssocio *s;

    printf("Ingrese DNI del socio a modificar: ");
    scanf("%ld",&dni);

    reg.dni=dni;
    reg.nro_reg=0;

    pos=indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_indice);

    if(pos==-1)
    {
        printf("Error en socio: DNI no encontrado. \n");
        return;
    }

    encontrado = (t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));
    s = (Ssocio *)((char *)vectorValidos->vec + encontrado->nro_reg * sizeof(Ssocio));

    printf("\nDatos actuales del miembro con dni %ld y cuil %s:\n\n",s->dni,s->cuil);
    printf("1. Apellidos y nombres: %s\n", s->nyp);
    printf("2. Sexo: %c \n",s->sexo);
    printf("3. Categoria: %s\n",s->categoria);
    printf("4. Fecha ultima cuota: %d/%d/%d\n",s->fechCuotaPaga.d,s->fechCuotaPaga.m,s->fechCuotaPaga.a);
    printf("5. Fecha de afiliacion: %d/%d/%d\n",s->fechAfilia.d,s->fechAfilia.m,s->fechAfilia.a);
    printf("6. Fecha de nacimiento: %d/%d/%d\n",s->fechNac.d,s->fechNac.m,s->fechNac.a);
    printf("7. Plan: %s\n",s->plan);
    if (*s->emailTutor == '\n' || *s->emailTutor == '\0')
        printf("8. Email Tutor: Sin email asignado.\n");
    else
        printf("8. Email Tutor: %s\n",s->emailTutor);

    printf("9. presione 9 para dejar de modificar.\n");


    while(cod==0){
        cod=1;
        printf("\n Ingrese numero de campo que desea modificar (1-9): ");
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
                printf("\nError: Sexo invalido.\n");
                cod = 0;
            }
            break;
        case 3:
            fflush(stdin);
            printf("Nueva categoria: ");
            scanf("%s", s->categoria);
            if (!categoriaEsvalida(s->categoria))
            {
                printf("\nError: Categoria invalida.\n");
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
                    printf("\nError: Formato de email invalido.\n");
                    cod = 0;
                }
            }
            break;
        case 4:
            printf("Nueva fecha ultima cuota (dd/mm/aaaa): ");
            if (scanf("%d/%d/%d", &s->fechCuotaPaga.d, &s->fechCuotaPaga.m, &s->fechCuotaPaga.a) != 3)
            {
                printf("\nError: Fecha de ultima cuota invalida.\n");
                cod = 0;
            }
            break;
        case 5:
            printf("Nueva fecha de afiliacion (dd/mm/aaaa):");
            if (scanf("%d/%d/%d", &s->fechAfilia.d, &s->fechAfilia.m, &s->fechAfilia.a) != 3)
            {
                printf("\nError: Fecha de afiliación invalida.\n");
                cod = 0;
            }
            break;
        case 6:
            printf("Nueva fecha de nacimiento (dd/mm/aaaa):");
            if (scanf("%d/%d/%d", &s->fechNac.d, &s->fechNac.m, &s->fechNac.a) != 3)
            {
                printf("\nError: Fecha de nacimiento invalida.\n");
                cod = 0;
            }
            break;
        case 7:
            fflush(stdin);
            printf("Nuevo plan: ");
            scanf("%s", s->plan);
            if (!planEsvalido(s->plan))
            {
                printf("\nError: Plan invalido.\n");
                cod = 0;
            }
            break;
        case 8:
            fflush(stdin);
            printf("\nNuevo email de tutor: ");
            scanf("%s", s->emailTutor);
            if (!emailTutorEsvalido(s->emailTutor) )
            {
                printf("\nError: Formato de email invalido.\n");
                cod = 0;
            }
            break;
        case 9:
            cod=1;
            break;
        default:
            printf("\n---Opcion invalida---.\n");
            cod = 0;

        }
    }

    if (cod)
        printf("Modificaciones realizadas con exito.\n");
    else
        printf("Error en la actualizacion.\n");
}


void resuelveF (t_indice *indiP, tda_vec *vectorValidos)
{
    int id, op, cod = 0;
    int pos;
    t_reg_indice_pelicula reg;
    t_reg_indice_pelicula *encontrado;
    Spelicula *p;

    printf("Ingrese ID de la pelicula a modificar: ");
    scanf("%d", &id);

    reg.id_pelicula = id;
    reg.nro_reg = 0;

    pos = indice_buscar(indiP,
                        &reg,
                        indiP->cantidad_elementos_actual,
                        sizeof(t_reg_indice_pelicula),
                        cmp_indice_p);

    if(pos == -1)
    {
        printf("Error: ID no encontrado.\n");
        return;
    }

    encontrado = (t_reg_indice_pelicula *)
                 ((char *)indiP->vindice +
                  pos * sizeof(t_reg_indice_pelicula));

    p = (Spelicula *)
        ((char *)vectorValidos->vec +
         encontrado->nro_reg * sizeof(Spelicula));

    printf("\nDatos actuales de la pelicula:\n");
    printf("1. Titulo: %s\n", p->titulo);
    printf("2. Genero: %s\n", p->genero);
    printf("3. Stock: %d\n", p->stock);
    printf("4. presione 4 para dejar de modificar.\n");

    while(cod==0){

        cod=1;
        printf("\nIngrese campo a modificar (1-4): ");
        scanf("%d", &op);

        switch(op)
        {
        case 1:
            printf("Nuevo titulo: ");
            getchar();
            fgets(p->titulo, sizeof(p->titulo), stdin);

            {
                size_t len = mystrlen(p->titulo);
                if(len > 0 && p->titulo[len - 1] == '\n')
                    p->titulo[len - 1] = '\0';
            }

            eliminarEspaciosExtremos(p->titulo);
            eliminarEspaciosRepetidos(p->titulo);
            normalizar(p->titulo);
            break;


        case 2:
            printf("Nuevo genero: ");
            scanf("%20s", p->genero);

            if(!generoEsvalido(p->genero))
            {
                printf("Error: Genero invalido.\n");
                cod = 0;
            }
            break;

        case 3:
            printf("Nuevo stock: ");
            scanf("%d", &p->stock);

            if(p->stock < 0)
            {
                printf("Error: Stock invalido.\n");
                cod = 0;
            }
            break;
        case 4:
            cod=-1;
            break;
        default:
            printf("\n---Opcion invalida---.\n");
            cod = 0;
        }
    }

    if(cod)
        printf("Pelicula modificada exitosamente.\n");
    else
        printf("Error en la actualizacion.\n");
}

void resuelveG (t_indice *indi, tda_vec *vectorValidos)
{
    long dni;
    int pos;
    t_reg_indice reg;
    Ssocio *s;

    printf("Ingrese DNI para mostrar informacion: ");
    scanf("%ld",&dni);

    reg.dni=dni;
    reg.nro_reg=0;

    pos=indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_indice);

    if(pos==-1)
        printf("Error en socio: DNI no encontrado. \n");
    else
    {
        t_reg_indice *encontrado = (t_reg_indice *)((char *)indi->vindice + pos * sizeof(t_reg_indice));


        s = (Ssocio *)((char *)vectorValidos->vec + encontrado->nro_reg * sizeof(Ssocio));


        printf("\nInformacion del socio:\n\n");
        printf("DNI: %ld\n", s->dni);
        printf("CUIL: %s\n", s->cuil);
        printf("Apellidos y nombres: %s\n", s->nyp);
        printf("Fecha de nacimiento: %d/%d/%d\n",s->fechNac.d,s->fechNac.m,s->fechNac.a);
        printf("Sexo: %c\n", s->sexo);
        printf("Fecha de afiliacion: %d/%d/%d\n",s->fechAfilia.d,s->fechAfilia.m,s->fechAfilia.a);
        printf("Categoria: %s\n", s->categoria);
        printf("Fecha ultima cuota: %d/%d/%d\n",s->fechCuotaPaga.d,s->fechCuotaPaga.m,s->fechCuotaPaga.a);
        printf("Estado: %c\n",s->estado);

        printf("Plan: %s\n", s->plan);
        if (*s->emailTutor == '\n' || *s->emailTutor == '\0')
            printf("Email Tutor: Sin email asignado.\n\n");
        else
            printf("Email Tutor: %s\n\n",s->emailTutor);
    }
}

void resuelveI(t_indice *indi, tda_vec *vectorValidos)
{
    printf("\nListado de socios ordenado por DNI:\n");
    printf("%-10s %-15s %-60s %-5s %-12s %-14s %-12s %-12s %-8s %-10s %-25s\n",
           "DNI", "CUIL", "Apellidos y nombres", "Sexo", "F.Nac.",
           "F.Afiliacion", "Categoria", "Ult.Cuota", "Estado",
           "Plan", "Email Tutor");

    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    t_reg_indice *ini = (t_reg_indice *)indi->vindice;
    t_reg_indice *fin = ini + indi->cantidad_elementos_actual;

    while (ini < fin)
    {
        Ssocio *socio = (Ssocio *)((char *)vectorValidos->vec + ini->nro_reg * vectorValidos->tam_elem);

        if (socio->estado == 'A')
        {
            printf("%-10ld %-15s %-60s %-5c %02d/%02d/%04d   %02d/%02d/%04d     %-12s %02d/%02d/%04d   %-8c %-10s %-25s\n\n",
                   socio->dni,
                   socio->cuil,
                   socio->nyp,
                   socio->sexo,
                   socio->fechNac.d, socio->fechNac.m, socio->fechNac.a,
                   socio->fechAfilia.d, socio->fechAfilia.m, socio->fechAfilia.a,
                   socio->categoria,
                   socio->fechCuotaPaga.d, socio->fechCuotaPaga.m, socio->fechCuotaPaga.a,
                   socio->estado,
                   socio->plan,
                   socio->emailTutor);
        }

        ini++;
    }
}


void resuelveJ (t_indice *indiAYN, tda_vec *vectorValidos)
{
    t_reg_indice_ayn *ind = (t_reg_indice_ayn *)indiAYN->vindice;
    t_reg_indice_ayn *fin = ind + indiAYN->cantidad_elementos_actual;


    printf("\n%-30s %-12s %-12s %-12s %-12s\n\n----------------------------------------------------------------------------------\n",
           "Apellido y Nombre",
           "BASIC",
           "PREMIUM",
           "VIP",
           "FAMILY");

    while(ind < fin)
    {
        Ssocio *s = (Ssocio *)((char *)vectorValidos->vec +
                               ind->nro_reg * vectorValidos->tam_elem);


        printf("%-30s ", s->nyp);

        if(strcmp(s->plan, "BASIC") == 0)
            printf("%-12ld %-12s %-12s %-12s",
                   s->dni, "0", "0", "0");

        else if(strcmp(s->plan, "PREMIUM") == 0)
            printf("%-12s %-12ld %-12s %-12s",
                   "0", s->dni, "0", "0");

        else if(strcmp(s->plan, "VIP") == 0)
            printf("%-12s %-12s %-12ld %-12s",
                   "0", "0", s->dni, "0");

        else if(strcmp(s->plan, "FAMILY") == 0)
            printf("%-12s %-12s %-12s %-12ld",
                   "0", "0", "0", s->dni);

        printf("\n");


        ind++;
    }
}

void imprimirVector(const int *vec,const char *vecN[], int tam, const char *titulo)
{
    int i;
    printf("%s\n", titulo);

    for(i = 0; i < tam; i++)
    {
        printf("%s -> %d\n", vecN[i], vec[i]);
    }
    printf("\n");
}

void resuelveK (long matC [][2], long matP [][2], int erroresC, int erroresP)
{
    int i,x,pos;

    // Vector de nombres de errores (índice = código de error)
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



    int vectorErroresC[16]= {0};
    int vectorErroresP[3]= {0};

    for (i=0; i<erroresC; i++)
    {
        pos=(matC[i][0])-1;
        vectorErroresC[pos]++;
    }

    for (x=0; x<erroresP; x++)
    {
        pos=(matP[x][0])-1;
        vectorErroresP[pos]++;
    }


    imprimirVector(vectorErroresC, vectorErroresC_Nombres, 16, "Errores Clientes:");
    imprimirVector(vectorErroresP, vectorErroresP_Nombres, 3, "Errores Peliculas:");

}

int contarAlquileresActivos(tda_vec *vecAlquileres, long dni)
{
    int cont = 0;

    SAlquiler *act = (SAlquiler*)vecAlquileres->vec; /// Puntero al inicio
    SAlquiler *fin = act + vecAlquileres->ce; /// Puntero al inicio

    while(act < fin)
    {
        if(act->dni == dni &&
                act->activo == 'S')
        {
            cont++;
        }

        act++;
    }

    return cont;
}

SAlquiler* buscarAlquiler(tda_vec *vecAlquileres,
                          long dni,
                          int idPelicula)
{
    SAlquiler *encontrado = NULL;

    SAlquiler *act = (SAlquiler*)vecAlquileres->vec;
    SAlquiler *fin = act + vecAlquileres->ce;

    while(act < fin && encontrado == NULL)
    {
        if(act->dni == dni &&
                act->idPelicula == idPelicula)
        {
            encontrado = act;
        }
        else
        {
            act++;
        }
    }

    return encontrado;
}

void resuelveH(t_indice *indiC,
               t_indice *indiP,
               tda_vec *vecSocios,
               tda_vec *vecPeliculas,
               tda_vec *vecAlquileres)
{
    long dni;
    int idPelicula;

    int posSocio;
    int posPelicula;

    t_reg_indice claveSocio;
    t_reg_indice_pelicula clavePeli;

    t_reg_indice *regSocio;
    t_reg_indice_pelicula *regPeli;

    Ssocio *socio;
    Spelicula *pelicula;

    SAlquiler *alquiler;

    printf("\n=== ALQUILER DE TITULO ===\n");

    printf("\nIngrese DNI del socio: ");
    scanf("%ld", &dni);

    claveSocio.dni = dni;
    //    pos = indice_buscar(indi, &reg, indi->cantidad_elementos_actual, sizeof(t_reg_indice), cmp_indice);

    printf("\nDNI ingresado: %ld\n", dni);

//    t_reg_indice *aux = (t_reg_indice*)indiC->vindice;
//
//    printf("\nContenido del indice:\n");
//    for(int i = 0; i < indiC->cantidad_elementos_actual; i++)
//    {
//        printf("[%d] DNI=%ld REG=%d\n",
//               i,
//               aux[i].dni,
//               aux[i].nro_reg);
//    }
    posSocio = indice_buscar(
                   indiC,
                   &claveSocio,
                   indiC->cantidad_elementos_actual,
                   sizeof(t_reg_indice),
                   cmp_indice);



    if(posSocio == -1)
    {
        printf("\nSocio inexistente.\n");
        return;
    }

    regSocio = (t_reg_indice*)indiC->vindice + posSocio;

    socio = (Ssocio*)vecSocios->vec + regSocio->nro_reg;

    if(socio->estado!='A')
    {
        printf("\nEl socio no esta activo.\n");
        return;
    }

    printf("\nIngrese ID de pelicula: ");
    scanf("%d", &idPelicula);

    clavePeli.id_pelicula = idPelicula;

    posPelicula = indice_buscar(
                      indiP,
                      &clavePeli,
                      indiP->cantidad_elementos_actual,
                      sizeof(t_reg_indice_pelicula),
                      cmp_indice_p);

    if(posPelicula == NO_EXISTE)
    {
        printf("\nPelicula inexistente.\n");
        return;
    }

    regPeli = (t_reg_indice_pelicula*)indiP->vindice + posPelicula;

    pelicula = (Spelicula*)vecPeliculas->vec + regPeli->nro_reg;

    if(pelicula->estado != 'A')
    {
        printf("\nLa pelicula no esta activa.\n");
        return;
    }

    if(pelicula->stock <= 0)
    {
        printf("\nNo hay stock disponible.\n");
        return;
    }

    if(my_strcmpi(socio->plan, "BASIC") == 0)
    {
        if(contarAlquileresActivos(vecAlquileres,
                                   socio->dni) >= 2)
        {
            printf("\nEl socio BASIC ya posee 2 peliculas alquiladas.\n");
            return;
        }
    }

    alquiler = buscarAlquiler(vecAlquileres,
                              socio->dni,
                              pelicula->idPelicula);

    if(alquiler)
    {
        alquiler->cantAlquileres++;
        alquiler->activo = 'S';
    }
    else
    {
        SAlquiler nuevo;

        nuevo.dni = socio->dni;
        nuevo.idPelicula = pelicula->idPelicula;
        nuevo.cantAlquileres = 1;
        nuevo.activo = 'S';

        tda_vec_insertar(vecAlquileres, &nuevo);
    }

    pelicula->stock--;

    printf("\nAlquiler realizado correctamente.\n");
    printf("\nSocio: %s", socio->nyp);
    printf("\nPelicula: %s", pelicula->titulo);
    printf("\nStock restante: %d\n", pelicula->stock);
}
