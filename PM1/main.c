#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTOS 744        // 24 eventos al dia * 31 dias
#define MAX_EVENTOS_FORZAND 31 // maximo 31 dias al mes

// -------------------------
// Estructuras
// -------------------------
typedef struct
{
    float max, sumatoria, cantidad, temp;
} Costos;

typedef struct
{
    int hora;
    char evento[81];
    char lugar[81];
} Evento;

// LVO aux
typedef struct NodoEvento
{
    Evento evento;
    struct NodoEvento *sig;
} NodoEvento;

// LSO sin forzar dp. funcional
typedef struct LSOBB
{
    char fecha[11]; // formato: AAAA-MM-DD
    Evento evento;
} LSOBB;

// LSO forzando dp. funcional
typedef struct LSOBB_F
{
    char fecha[11]; // formato: AAAA-MM-DD
    NodoEvento *listaEventos;
} LSOBB_F;

// ABB sin dependencia funcional: un evento por fecha (se puede repetir fecha)
typedef struct NodoABB
{
    char fecha[11]; // formato: AAAA-MM-DD
    Evento evento;
    struct NodoABB *izq, *der;
} NodoABB;

typedef struct ABB
{
    NodoABB *cur;
    NodoABB *raiz;
    NodoABB *ant;
} ABB;

// ABB-F (forzando dependencia funcional): un conjunto de eventos por fecha
typedef struct NodoABB_F
{
    char fecha[11]; // formato: AAAA-MM-DD
    NodoEvento *listaEventos;
    struct NodoABB_F *izq, *der;
} NodoABB_F;

typedef struct ABB_F
{
    NodoABB_F *cur;
    NodoABB_F *raiz;
    NodoABB_F *ant;
} ABB_F;

// Prototipos
void menuPrincipal();
int Lectura_Operaciones();
void comparacionEstructuras();
NodoEvento *crearNodoLVO(Evento e);
void mostrarLSOBB();
void mostrarLSOBB_F();
void mostrarABBPreorden();
void mostrarABB_FPreorden();
int compararEventos();
void mostrarEvento();
NodoEvento *evocacionABB();
NodoEvento *evocacionLSOBB_F();
NodoEvento *evocacionLSOBB();
NodoEvento *evocacionABB_F();
NodoEvento *crearNodoLVO();

// -------------------------

Costos cLSOevoc = {0, 0, 0, 0};
Costos cLSOalta = {0, 0, 0, 0};
Costos cLSObaja = {0, 0, 0, 0};
Costos cLSO_Fevoc = {0, 0, 0, 0};
Costos cLSO_Falta = {0, 0, 0, 0};
Costos cLSO_Fbaja = {0, 0, 0, 0};
Costos cABBevoc = {0, 0, 0, 0};
Costos cABBalta = {0, 0, 0, 0};
Costos cABBbaja = {0, 0, 0, 0};
Costos cABB_Fevoc = {0, 0, 0, 0};
Costos cABB_Falta = {0, 0, 0, 0};
Costos cABB_Fbaja = {0, 0, 0, 0};

int nodosABBF;
int eventosLSOBB_Fglobal;

int main()
{
    menuPrincipal();
    return 0;
}

void menuPrincipal()
{
    int opc, opcion, opcion2, opcion3;

    char fechaev[11];
    NodoEvento *resultado = NULL;

    ABB arbol;
    arbol.raiz = NULL;
    arbol.cur = NULL;
    arbol.ant = NULL;
    int cargadosABB = 0;

    ABB_F arbol_f;
    arbol_f.raiz = NULL;
    arbol_f.cur = NULL;
    arbol_f.ant = NULL;
    int cargadosABB_F = 0;

    LSOBB_F lista_f[MAX_EVENTOS];
    int cargadosLSO_F = 0;

    LSOBB lista[MAX_EVENTOS];
    int cargadosLSO = 0;

    do
    {
        system("cls");
        printf("=== Agenda Mensual ===\n");
        printf("1. Comparacion de Estructuras\n");
        printf("2. Administrar Estructuras\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc)
        {
        case 1:
            comparacionEstructuras(&arbol, &arbol_f, lista, lista_f, &cargadosLSO, &cargadosLSO_F, &cargadosABB, &cargadosABB_F);
            printf("\n\nCargados lso: %d\n", cargadosLSO);
            printf("\nCargados lso_f: %d\n, total eventos: %d\n\n", cargadosLSO_F, eventosLSOBB_Fglobal);
            printf("Cargados abb: %d\n", cargadosABB);
            printf("\nCargados abb_f: %d\n total eventos: %d\n\n", nodosABBF, cargadosABB_F);

            system("pause");
            break;
        case 2:
            do
            {
                printf("\nADMINISTRAR ESTRUCTURA\n");
                printf("1. Mostrar Estructura.\n");
                printf("2. Evocar.\n");
                printf("0. Volver\n");
                printf("Seleccione una opcion: ");
                scanf("%d", &opcion2);

                switch (opcion2)
                {
                case 1:
                    do
                    {
                        printf("\n1. Mostrar LSO sin forzar dependencia.\n");
                        printf("2. Mostrar LSO forzando dependencia.\n");
                        printf("3. Mostrar ABB sin forzar dependencia..\n");
                        printf("4. Mostrar ABB forzando dependencia.\n");
                        printf("0. Volver\n");
                        printf("Seleccione una opcion: ");
                        scanf("%d", &opcion);
                        switch (opcion)
                        {
                        case 1:
                            mostrarLSOBB(lista, cargadosLSO);
                            break;
                        case 2:
                            mostrarLSOBB_F(lista_f, cargadosLSO_F);
                            break;
                        case 3:
                            system("cls");
                            printf("ARBOL BINARIO DE BUSQUEDA SIN FORZAR DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            mostrarABBPreorden(arbol.raiz);
                            break;
                        case 4:
                            system("cls");
                            printf("ARBOL BINARIO DE BUSQUEDA FORZANDO DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            mostrarABB_FPreorden(arbol_f.raiz);
                            break;
                        case 0:
                            printf(">> Volviendo...\n");
                            opcion2 = 0;
                            break;
                        default:
                            printf(">> Opción invalida. Intente de nuevo.\n");
                        }
                    } while (opcion != 0);
                    break;
                case 2:
                    // EVOCAR
                    printf("Ingrese una fecha a evocar: ");
                    scanf(" %[^\n]", fechaev);

                    do
                    {
                        printf("\nEVOCAR ESTRUCTURA\n");
                        printf("1. Evocar LSO sin forzar dependencia.\n");
                        printf("2. Evocar LSO forzando dependencia.\n");
                        printf("3. Evocar ABB sin forzar dependencia..\n");
                        printf("4. Evocar ABB forzando dependencia.\n");
                        printf("0. Volver\n");
                        printf("Seleccione una opcion: ");
                        scanf("%d", &opcion3);

                        switch (opcion3)
                        {
                        case 1:
                            system("cls");
                            printf("LSO SIN FORZAR DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            resultado = evocacionLSOBB(lista, fechaev, cargadosLSO);
                            if (resultado != NULL)
                            {
                                printf("Fecha %s:\n", fechaev);
                                while (resultado != NULL)
                                {
                                    mostrarEvento(resultado->evento);
                                    printf("-------------------------------------------------\n");
                                    resultado = resultado->sig;
                                }
                            }
                            else
                            {
                                printf("No hay eventos asociados a la fecha %s:\n", fechaev);
                            }
                            system("pause");
                            break;
                        case 2:
                            system("cls");
                            printf("LSO FORZANDO DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            resultado = evocacionLSOBB_F(lista_f, fechaev, cargadosLSO_F);
                            if (resultado != NULL)
                            {
                                printf("Fecha %s:\n", fechaev);
                                while (resultado != NULL)
                                {
                                    mostrarEvento(resultado->evento);
                                    printf("-------------------------------------------------\n");
                                    resultado = resultado->sig;
                                }
                            }
                            else
                            {
                                printf("No hay eventos asociados a la fecha %s:\n", fechaev);
                            }
                            system("pause");
                            break;
                        case 3:
                            system("cls");
                            printf("ARBOL BINARIO DE BUSQUEDA SIN FORZAR DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            resultado = evocacionABB(&arbol, fechaev);
                            if (resultado != NULL)
                            {
                                printf("Fecha %s:\n", fechaev);
                                while (resultado != NULL)
                                {
                                    mostrarEvento(resultado->evento);
                                    printf("-------------------------------------------------\n");
                                    resultado = resultado->sig;
                                }
                            }
                            else
                            {
                                printf("No hay eventos asociados a la fecha %s:\n", fechaev);
                            }
                            system("pause");
                            break;
                        case 4:
                            system("cls");
                            printf("ARBOL BINARIO DE BUSQUEDA FORZANDO DEPENDENCIA FUNCIONAL\n");
                            printf("--------------AGENDA DE EVENTOS--------------\n");
                            resultado = evocacionABB_F(&arbol_f, fechaev);
                            if (resultado != NULL)
                            {
                                printf("Fecha %s:\n", fechaev);
                                while (resultado != NULL)
                                {
                                    mostrarEvento(resultado->evento);
                                    printf("-------------------------------------------------\n");
                                    resultado = resultado->sig;
                                }
                            }
                            else
                            {
                                printf("No hay eventos asociados a la fecha %s:\n", fechaev);
                            }
                            system("pause");
                            break;
                        case 0:
                            printf(">> Volviendo...\n");
                            opcion2 = 0;
                            break;
                        default:
                            printf(">> Opción invalida. Intente de nuevo.\n");
                        }
                    } while (opcion3 != 0);
                    if (resultado != NULL)
                    {
                        NodoEvento *cur = resultado;
                        while (cur != NULL)
                        {
                            mostrarEvento(cur->evento);
                            cur = cur->sig;
                        }
                    }
                    else
                        printf(">> No se encontraron eventos con esa fecha.\n");
                    break;
                case 0:
                    printf(">> Volviendo...\n");
                    break;
                default:
                    printf(">> Opción invalida. Intente de nuevo.\n");
                }
            } while (opcion2 != 0);
            break;
        case 0:
            printf(">> Saliendo...\n");
            break;
        default:
            printf(">> Opción invalida. Intente de nuevo.\n");
        }
    } while (opc != 0);
}

// --- Operaciones de LSO sin forzar dependencia ---
int inicioLSOBB(LSOBB lista[], char fechaBuscada[], int total)
{    
    int inf = 0;
    int sup = total - 1;
    int medio;

    int costoAux=0;

    while (inf <= sup)
    {
        medio = (inf + sup + 1) / 2; //  (Sumando +1 para obtener techo, ya que C redondea al piso)

        costoAux++;

        if (strcmp(lista[medio].fecha, fechaBuscada) < 0)
        {
            inf = medio + 1; // Buscamos a la derecha
        }
        else
        {
            sup = medio - 1; // Buscamos a la izquierda
        }
    }

    cLSOevoc.temp = costoAux;

    return inf; // devolvemos el testigo, deberia ser donde comience la primer fecha
}

int hayMasLSOBB(LSOBB lista[], char fechaBuscada[], int pos, int total)
{
    if (pos < total)
    {
        cLSOevoc.temp++;

        if(strcmp(lista[pos].fecha, fechaBuscada) == 0){
          return 1;  
        }     
        
    }
    return 0;
}

// Localizar retorna: 1 si hay un evento en la fecha dada a esa hora, 0 si no existe.
int localizarLSOBB(LSOBB lista[], LSOBB evBuscado, int total, int *pos)
{
    *pos = inicioLSOBB(lista, evBuscado.fecha, total);
    int posAux = *pos;
    while (hayMasLSOBB(lista, evBuscado.fecha, posAux, total))
    {
        if (evBuscado.evento.hora == lista[posAux].evento.hora)
        {
            *pos = posAux;
            return 1; // ya existe un evento en esa hora, se debería dar de BAJA en lista[posAux]
        }
        posAux++; // deme otro
    }

    return 0; // el evento no existe, se insertará en lista[pos] ALTA
}

NodoEvento *evocacionLSOBB(LSOBB lsobb[], char fechaBuscada[], int cargadosLSO)
{
    NodoEvento *listaEventos = NULL, *ultimo = NULL;
    int pos = inicioLSOBB(lsobb, fechaBuscada, cargadosLSO);

    while (pos < cargadosLSO && strcmp(lsobb[pos].fecha, fechaBuscada) == 0)
    {
        NodoEvento *nuevo = crearNodoLVO(lsobb[pos].evento);

        if (listaEventos == NULL)
        {
            listaEventos = nuevo;
        }
        else
        {
            ultimo->sig = nuevo;
        }

        ultimo = nuevo;
        pos++;
    }

    if(listaEventos != NULL){               
        cLSOevoc.cantidad++; //evocar exitoso

        cLSOevoc.sumatoria += cLSOevoc.temp;

        if(cLSOevoc.temp > cLSOevoc.max){
            cLSOevoc.max = cLSOevoc.temp;
        }

    }

    return listaEventos;
}

// Retorna 1 si se insertó, -1 si está llena y 0 si ya estaba
int altaLSOBB(LSOBB lsobb[], LSOBB elem, int *cargadosLSO)
{
    cLSOalta.temp=0;

    if (*cargadosLSO == MAX_EVENTOS - 1)
    {
        return -1; // lista llena
    }

    int pos = 0;
    if ((localizarLSOBB(lsobb, elem, *cargadosLSO, &pos)) == 1)
    {
        return 0; // ya hay un evento a esa hora en la lista
    }
    int i;
    for (i = *cargadosLSO - 1; i >= pos; i--)
    {

        cLSOalta.temp++;

        lsobb[i + 1] = lsobb[i];
    }
    lsobb[pos] = elem;
    *cargadosLSO += 1;

    if(cLSOalta.temp > cLSOalta.max){
        cLSOalta.max = cLSOalta.temp;
    }

    cLSOalta.cantidad++;

    cLSOalta.sumatoria += cLSOalta.temp;

    return 1;
}

// Retorna 1 si se pudo eliminar, 0 si no se encontró el elemento a eliminar.
int eliminarLSOBB(LSOBB lsobb[], LSOBB elem, int *cargadosLSO)
{

    cLSObaja.temp=0;

    int pos = 0;
    if (localizarLSOBB(lsobb, elem, *cargadosLSO, &pos) == 1)
    {
        if (compararEventos(elem.evento, lsobb[pos].evento) == 1)
        {
            int i = pos;
            for (i; i < (*cargadosLSO) - 1; i++)
            {

                cLSObaja.temp++;

                lsobb[i] = lsobb[i + 1];
            }
            *cargadosLSO -= 1;

            if(cLSObaja.temp > cLSObaja.max){
                cLSObaja.max = cLSObaja.temp;
            }
        
            cLSObaja.cantidad++;
        
            cLSObaja.sumatoria += cLSObaja.temp;

            return 1; // se elimino con exito
        }
    }
    return 0; // el elemento no esta en la lista
}

void mostrarLSOBB(LSOBB lista[], int cargados)
{
    int i, mod;
    // PARA QUE VAYA MOSTRANDO DE A 5
    system("cls");
    printf("LISTA SECUENCIAL ORDENADA CON BUSQUEDA BINARIA SIN FORZAR DEPENDENCIA FUNCIONAL\n");
    printf("--------------AGENDA DE EVENTOS--------------\n");
    for (i = 0; i < cargados; i += 5)
    {
        for (mod = i; mod < i + 5 && mod < cargados; mod++)
        {
            printf("Fecha %s:\n", lista[mod].fecha);
            mostrarEvento(lista[mod].evento);
            printf("-------------------------------------------------\n");
        }
        system("pause");
    }
}

// ----
// --- Operaciones LSO FORZANDO dependencia ---
// ----

int inicioLSOBB_F(LSOBB_F lista[], char fechaBuscada[], int total)
{
    int inf = 0;
    int sup = total - 1;
    int medio;

    cLSO_Fevoc.temp=0;

    while (inf <= sup)
    {
        medio = (inf + sup + 1) / 2; // techo segmento más grande a izquierda

        cLSO_Fevoc.temp++;

        if (strcmp(lista[medio].fecha, fechaBuscada) < 0)
        {
            inf = medio + 1; // buscamos a la derecha
        }
        else
        {
            sup = medio - 1; // buscamos a la izquierda
        }
    }

    return inf; // devolvemos el testigo a la izquierda del bloque
}

int hayMasLSOBB_F(NodoEvento *listaEventos)
{
    if (listaEventos != NULL)
    {
        return 1;
    }
    return 0;
}

// Localizar retorna: 1 si hay un evento en la fecha dada a esa hora, 0 si la fecha está y no hay evento a esa hora, -1 si NO esta la fecha.
int localizarLSOBB_F(LSOBB_F lista[], char fechaB[], Evento evBuscado, int total, int *pos, NodoEvento **cur, NodoEvento **ant)
{
    *pos = inicioLSOBB_F(lista, fechaB, total);

    if (*pos >= total)
    {
        return -1; // fuera de rango → fecha nueva
    }

    cLSO_Fevoc.temp++;

    if (strcmp(lista[*pos].fecha, fechaB) != 0)
    {
        return -1; // No existe la fecha en la lista (ALTA de FECHA en lista[pos])
    }

    *cur = lista[*pos].listaEventos;
    *ant = *cur;

    while (hayMasLSOBB_F(*cur))
    {
        if ((*cur)->evento.hora == evBuscado.hora)
        {
            return 1; // ya existe un evento a esa hora
        }
        *ant = *cur;
        *cur = (*cur)->sig;
    }

    return 0; // fecha existe, pero evento nuevo
}

int altaLSOBB_F(LSOBB_F lsobb_F[], char fecha[], Evento ev, int *cargadosLSO_F)
{
    if (*cargadosLSO_F == MAX_EVENTOS_FORZAND)
    {
        return 0; // lista llena
    }

    int pos = 0;
    NodoEvento *cur = NULL;
    NodoEvento *ant = NULL;

    cLSO_Falta.temp=0;

    int resultadoLocalizar = localizarLSOBB_F(lsobb_F, fecha, ev, *cargadosLSO_F, &pos, &cur, &ant);
    if (resultadoLocalizar == 1)
    {
        return 0; // ya hay un evento a esa hora en la lista
    }

    if (resultadoLocalizar == -1)
    { // caso: Dar de alta FECHA y evento
        int i;
        for (i = *cargadosLSO_F - 1; i >= pos; i--)
        {

            cLSO_Falta.temp++;

            lsobb_F[i + 1] = lsobb_F[i];
        }
        strcpy(lsobb_F[pos].fecha, fecha);
        lsobb_F[pos].listaEventos = crearNodoLVO(ev);
        *cargadosLSO_F += 1; // celda nueva
    }
    else // caso: Dar de alta un evento en una fecha ya existente
    {
        NodoEvento *nuevo = crearNodoLVO(ev);
        nuevo->sig = lsobb_F[pos].listaEventos;
        lsobb_F[pos].listaEventos = nuevo;
    }
    eventosLSOBB_Fglobal++;

    if(cLSO_Falta.temp > cLSO_Falta.max){
        cLSO_Falta.max=cLSO_Falta.temp;
    }

    cLSO_Falta.cantidad++;
    cLSO_Falta.sumatoria += cLSO_Falta.temp;

    return 1;
}

// Retorna 1 si se pudo eliminar, 0 si no se encontró el elemento a eliminar.
int eliminarLSOBB_F(LSOBB_F lsobb_F[], char fecha[], Evento ev, int *cargadosLSO_F)
{
    int pos = 0;
    NodoEvento *cur = NULL;
    NodoEvento *ant = NULL;

    cLSO_Fbaja.temp=0;

    if (localizarLSOBB_F(lsobb_F, fecha, ev, *cargadosLSO_F, &pos, &cur, &ant) == 1)
    {
        if (compararEventos(ev, cur->evento) == 1)
        {
            if (lsobb_F[pos].listaEventos->sig != NULL) // hay más de un evento en la lista
            {
                if (cur == ant)
                { // el evento esta en la cabecera y HAY mas eventos.
                    lsobb_F[pos].listaEventos = lsobb_F[pos].listaEventos->sig;
                    free((void *)cur);
                }
                else
                {
                    ant->sig = cur->sig;
                    free((void *)cur);
                }
            }
            else // solo hay un evento en la lista → eliminar fecha
            {
                // Liberar toda la sublista de eventos
                NodoEvento *aux = lsobb_F[pos].listaEventos;
                while (aux != NULL)
                {
                    NodoEvento *temp = aux;
                    aux = aux->sig;
                    free(temp);
                }
                int resultadoCURSIG = -1;
                if (cur->sig == NULL)
                {
                    resultadoCURSIG = 0;
                }
                else
                {
                    resultadoCURSIG = 1;
                }
                // Corrimiento de celdas
                for (int i = pos; i < (*cargadosLSO_F) - 1; i++)
                {

                    cLSO_Fbaja.temp++;

                    lsobb_F[i] = lsobb_F[i + 1];
                }

                // Limpiar la última celda que quedó duplicada
                lsobb_F[*cargadosLSO_F - 1].listaEventos = NULL;
                lsobb_F[*cargadosLSO_F - 1].fecha[0] = '\0';

                (*cargadosLSO_F)--;
            }

            eventosLSOBB_Fglobal--;

            if(cLSO_Fbaja.temp > cLSO_Fbaja.max){
                cLSO_Fbaja.max=cLSO_Fbaja.temp;
            }

            cLSO_Fbaja.cantidad++;
            cLSO_Fbaja.sumatoria += cLSO_Fbaja.temp;

            return 1; // se elimino con exito
        }
    }
    return 0; // el elemento no esta en la lista
}

NodoEvento *evocacionLSOBB_F(LSOBB_F lsobb_F[], char fechaBuscada[], int cargadosLSO_F)
{
    int pos = inicioLSOBB_F(lsobb_F, fechaBuscada, cargadosLSO_F);      

    if (strcmp(lsobb_F[pos].fecha, fechaBuscada) == 0)
    {
        cLSO_Fevoc.temp++; 

        if(cLSO_Fevoc.temp>cLSO_Fevoc.max){
            cLSO_Fevoc.max = cLSO_Fevoc.temp;
        }
    
        cLSO_Fevoc.sumatoria += cLSO_Fevoc.temp;
        cLSO_Fevoc.cantidad++;

        return lsobb_F[pos].listaEventos;
    }
    
    return NULL;
}

void mostrarLSOBB_F(LSOBB_F lista[], int cargados)
{
    int i;
    // MUESTRA TODOS LOS EVENTOS DE UNA FECHA
    system("cls");
    printf("LISTA SECUENCIAL ORDENADA CON BUSQUEDA BINARIA FORZANDO DEPENDENCIA FUNCIONAL\n");
    printf("--------------AGENDA DE EVENTOS--------------\n");
    for (i = 0; i < cargados; i++)
    {
        printf("Fecha %s:\n", lista[i].fecha);
        NodoEvento *cur = lista[i].listaEventos;
        while (cur != NULL)
        {
            mostrarEvento(cur->evento);
            printf("-------------------------------------------------\n");
            cur = cur->sig;
        }
        system("pause");
    }
}

//--------------------------
// --- Operaciones de ABB
//--------------------------

// RETORNA NULL SI NO SE PUDO
NodoABB *crearnodoABB(char fecha[], Evento evento)
{
    NodoABB *nodo_nuevo = (NodoABB *)malloc(sizeof(NodoABB));
    if (nodo_nuevo == NULL)
    {
        return nodo_nuevo; // Error: no se pudo reservar memoria
    }

    strcpy(nodo_nuevo->fecha, fecha);
    nodo_nuevo->evento = evento;

    nodo_nuevo->izq = NULL;
    nodo_nuevo->der = NULL;
    return nodo_nuevo;
}

int inicioABB(ABB *arbol, char fecha[]) {
    arbol->cur = arbol->raiz;
    arbol->ant = NULL;

    cABBevoc.temp=0;

    while (arbol->cur != NULL) {
        int cmp = strcmp(fecha, arbol->cur->fecha);

        cABBevoc.temp++;

        if (cmp == 0) {
            // Encontramos la primera coincidencia
            return 1;
        }
        arbol->ant = arbol->cur;
        if (cmp < 0) {
            arbol->cur = arbol->cur->izq;  // menor → izquierda
        } else {
            arbol->cur = arbol->cur->der;  // mayor → derecha
        }
    }
    return 0;  // no está cargada
}

int hayMasABB(ABB *arbol, char fechaBuscada[])
{
    if (arbol->cur != NULL)
    {
        cABBevoc.temp++;
        if(strcmp(arbol->cur->fecha, fechaBuscada) == 0){
            return 1;   //el nodo actual si es la fecha buscada
        }
        return 0;   //hay mas nodos, el actual NO tiene la fecha, pero podria haber mas
    }
    return -1;  //llegamos a un null (no hay mas)
}

// retorna 0 si no hay fecha o si no hay evento a esa hora, 1 si ya hay evento a esa hora.
int localizarABB(ABB *arbol, char fecha[], Evento ev)
{
    if (inicioABB(arbol, fecha) == 0)
    {
        return 0; // no esta cargada la fecha
    }

    int resHayMas = hayMasABB(arbol,fecha);
    while (resHayMas != -1)
    {
        if(resHayMas == 1){    
            if (arbol->cur->evento.hora == ev.hora){
                return 1; // Ya existe evento a esa hora
            }
        }
        arbol->ant = arbol->cur;

        cABBevoc.temp++;

        if (strcmp((arbol->cur)->fecha, fecha) >= 0)
        {
           arbol->cur = (arbol->cur)->izq; // menor o igual → izquierda
        }
        else
        {
           arbol->cur = (arbol->cur)->der; // mayor → derecha
        }

        resHayMas = hayMasABB(arbol,fecha);
    }
    return 0;
}   

int altaABB(ABB *arbol, char fecha[], Evento evento, int *cantCargadosABB) {
    NodoABB *nodo = crearnodoABB(fecha, evento);
    if (nodo == NULL) {return 0;}

    if (arbol->raiz == NULL) {
        arbol->raiz = nodo;
        (*cantCargadosABB)++;

        cABBalta.cantidad++;
        cABBalta.temp = 0.5;

        if(cABBalta.temp > cABBalta.max){
            cABBalta.max = cABBalta.temp; 
        }

        cABBalta.sumatoria += cABBalta.temp;

        return 1;
    }

    if (localizarABB(arbol, fecha, evento) == 1) {
        free(nodo);  // liberamos porque no lo vamos a insertar
        return 0;    // evento ya existe
    }

    // Insertar en el lugar correcto
    if (strcmp(arbol->ant->fecha, fecha) >= 0) {
        arbol->ant->izq = nodo;  // menor o igual → izquierda
    } else {
        arbol->ant->der = nodo;  // mayor → derecha
    }

    (*cantCargadosABB)++;


    cABBalta.cantidad++;
    cABBalta.temp = 0.5;

    if(cABBalta.temp > cABBalta.max){
        cABBalta.max = cABBalta.temp; 
    }

    cABBalta.sumatoria += cABBalta.temp;


    return 1;
}

int eliminarABB(ABB *arbol, char fecha[], Evento evento, int *cantCargadosABB)
{
    int res = localizarABB(arbol, fecha, evento);
    if (res == 0)
    {
        return 0; // No encontrado
    }

    if (compararEventos(evento, arbol->cur->evento) != 1)
    {
        return 0; // El evento no coincide exactamente
    }

    NodoABB *nodoAEliminar = arbol->cur;

    // Caso 1: sin hijos
    if (nodoAEliminar->izq == NULL && nodoAEliminar->der == NULL)
    {
        if (nodoAEliminar == arbol->raiz)
        {
            arbol->raiz = NULL;
        }
        else if (arbol->ant->der == nodoAEliminar)
        {
            arbol->ant->der = NULL;
        }
        else
        {
            arbol->ant->izq = NULL;
        }
        free(nodoAEliminar);

        
        cABBbaja.temp = 0.5;        

        
    }
    // Caso 2: solo hijo derecho
    else if (nodoAEliminar->izq == NULL)
    {
        if (nodoAEliminar == arbol->raiz)
        {
            arbol->raiz = nodoAEliminar->der;
        }
        else if (arbol->ant->der == nodoAEliminar)
        {
            arbol->ant->der = nodoAEliminar->der;
        }
        else
        {
            arbol->ant->izq = nodoAEliminar->der;
        }
        free(nodoAEliminar);

        cABBbaja.temp = 0.5;

    }
    // Caso 3: solo hijo izquierdo
    else if (nodoAEliminar->der == NULL)
    {
        if (nodoAEliminar == arbol->raiz)
        {
            arbol->raiz = nodoAEliminar->izq;
        }
        else if (arbol->ant->der == nodoAEliminar)
        {
            arbol->ant->der = nodoAEliminar->izq;
        }
        else
        {
            arbol->ant->izq = nodoAEliminar->izq;
        }
        free(nodoAEliminar);

        cABBbaja.temp = 0.5;

    }
    // Caso 4: dos hijos
    else
    {
        NodoABB *actual = nodoAEliminar->izq;
        NodoABB *padre = nodoAEliminar;
        while (actual->der != NULL)
        {
            padre = actual;
            actual = actual->der;
        }

        // Copiar datos
        strcpy(nodoAEliminar->fecha, actual->fecha);
        nodoAEliminar->evento = actual->evento;

        // Reconectar
        if (padre->der == actual)
        {
            padre->der = actual->izq;
        }
        else
        {
            padre->izq = actual->izq;
        }
        free(actual);

        cABBbaja.temp = 1.5; //1 copiar datos + 0,5 actualizar puntero.

    }

    (*cantCargadosABB) = *cantCargadosABB - 1;

    cABBbaja.cantidad++;

    if(cABBbaja.temp > cABBbaja.max){
        cABBbaja.max = cABBbaja.temp; 
    }

    cABBbaja.sumatoria += cABBbaja.temp;

    return 1;
}

// al recibir chequear si es NULL
NodoEvento *evocacionABB(ABB *arbol, char fechaBuscar[])
{
    NodoEvento *listaEventos = NULL;
    NodoEvento *ultimo = NULL;

    if (inicioABB(arbol, fechaBuscar) == 0)
    {
        return NULL; // no se encontró la fecha
    }

    int resHayMas=hayMasABB(arbol, fechaBuscar);
    while (resHayMas != -1)
    {
        if(resHayMas==1){
            NodoEvento *nuevo = crearNodoLVO(arbol->cur->evento);

            if (listaEventos == NULL)
            {
                listaEventos = nuevo;
            }
            else
            {
                ultimo->sig = nuevo;
            }
            ultimo = nuevo;
        }

        cABBevoc.temp++;

        arbol->ant = arbol->cur;
        if (strcmp((arbol->cur)->fecha, fechaBuscar) >= 0)
        {
           arbol->cur = (arbol->cur)->izq; // menor o igual → izquierda
        }
        else
        {
           arbol->cur = (arbol->cur)->der; // mayor → derecha
        }
        resHayMas=hayMasABB(arbol, fechaBuscar);
    }

    if(listaEventos != NULL){
        if(cABBevoc.temp > cABBevoc.max){
            cABBevoc.max = cABBevoc.temp;
        }

        cABBevoc.cantidad++;
        cABBevoc.sumatoria += cABBevoc.temp;
    }

    return listaEventos;
}

// PASAR LA RAIZ AL LLAMAR LA FUNCION
void mostrarABBPreorden(NodoABB *nodo)
{
    if (nodo != NULL)
    {
        printf("Fecha nodo: %s\n", nodo->fecha);
        mostrarEvento(nodo->evento);

        if (nodo->izq != NULL)
        {
            printf("  Hijo izquierdo: %s\n", nodo->izq->fecha);
        }
        if (nodo->der != NULL)
        {
            printf("  Hijo derecho: %s\n", nodo->der->fecha);
        }
        printf("-------------------------------------------------\n");

        // Recorremos hijo izquierdo
        mostrarABBPreorden(nodo->izq);

        // Recorremos hijo derecho
        mostrarABBPreorden(nodo->der);
    }
}

//--------------------------------
// --- ABB FORZANDO DEPENDENCIA FUNCIONAL
//--------------------------------

// CREA UN NODO CON UNA FECHA E INICIALIZA LA LISTA DE EVENTOS DE ESA FECHA
// RETORNA NULL SI NO SE PUDO Y EL Nodoabb_f si tuvo exito.
NodoABB_F *crearnodoABB_F(char fecha[], Evento evento)
{
    NodoABB_F *nodo_nuevo = (NodoABB_F *)malloc(sizeof(NodoABB_F));
    if (nodo_nuevo == NULL)
    {
        return nodo_nuevo; // Error: no se pudo reservar memoria
    }
    strcpy(nodo_nuevo->fecha, fecha);
    NodoEvento *nodo_evento = crearNodoLVO(evento);
    nodo_nuevo->listaEventos = nodo_evento;
    nodo_nuevo->izq = NULL;
    nodo_nuevo->der = NULL;
    return nodo_nuevo;
}

int inicioABB_F(ABB_F *arbol, char fecha[])
{

    cABB_Fevoc.temp=0;

    (arbol->cur) = (arbol->raiz);
    arbol->ant = arbol->cur;
    while (arbol->cur != NULL)
    {

        cABB_Fevoc.temp++;

        if (strcmp((arbol->cur)->fecha, fecha) != 0)
        {
            arbol->ant = arbol->cur; // actualizo el cursor del anterior

            cABB_Fevoc.temp++;

            if (strcmp((arbol->cur)->fecha, fecha) > 0)
            {
                arbol->cur = (arbol->cur)->izq; // menor → izquierda
            }
            else
            {
                arbol->cur = (arbol->cur)->der; // mayor → derecha
            }
        }
        else
        {
            break;
        }
    }
    if (arbol->cur == NULL)
    {
        return 0; // la fecha no está cargada
    }
    return 1; // se encontro la primer fecha
}

int hayMasABB_F(NodoEvento *listaEventos)
{
    if (listaEventos != NULL)
    {
        return 1;
    }
    return 0;
}

// Localizar retorna: 1 si hay un evento en la fecha dada a esa hora, 0 si la fecha está y no hay evento a esa hora, -1 si NO esta la fecha.
int localizarABB_F(ABB_F *arbol, char fechaB[], Evento evBuscado, int total, NodoEvento **cur, NodoEvento **ant)
{
    if (inicioABB_F(arbol, fechaB) == 0)
    {
        return -1; // no esta cargada la fecha
    }

    *cur = arbol->cur->listaEventos; // cur apunta a la lvo de la fecha buscada
    *ant = *cur;

    while (hayMasLSOBB_F(*cur))
    {
        if ((*cur)->evento.hora == evBuscado.hora)
        {
            return 1; // ya existe un evento a esa hora, estará apuntado por cur para darlo de baja eventualmente
        }
        *ant = *cur;
        *cur = (*cur)->sig; // deme otro
    }

    return 0; // No existe evento a esa hora
}

NodoEvento *evocacionABB_F(ABB_F arbol[], char fecha[])
{
    int i = inicioABB_F(arbol, fecha);
    if (i == 1)
    {

        if(cABB_Fevoc.temp > cABB_Fevoc.max){
            cABB_Fevoc.max=cABB_Fevoc.temp;
        }
        cABB_Fevoc.cantidad++;
        cABB_Fevoc.sumatoria += cABB_Fevoc.temp;


        return arbol->cur->listaEventos;
    }
    else
        return NULL;
}

// ALTA
int altaABB_F(ABB_F *arbol, char fecha[], Evento evento, int *total)
{
    NodoEvento *cur = NULL;
    NodoEvento *ant = NULL;
    int pos = -1;

    int res = localizarABB_F(arbol, fecha, evento, *total, &cur, &ant);

    if (res == -1)
    { // Fecha no encontrada → agregar nodo nuevo al ABB_F
        NodoABB_F *nuevo = crearnodoABB_F(fecha, evento);
        if (nuevo == NULL)
        {
            return 0; // Error al crear nodo
        }
        if (arbol->raiz == NULL)
        {
            arbol->raiz = nuevo;
        }
        else
        {
            NodoABB_F *actual = arbol->raiz;
            NodoABB_F *padre = NULL;
            while (actual != NULL)
            {
                padre = actual;
                if (strcmp(fecha, actual->fecha) < 0)
                {
                    actual = actual->izq;
                }
                else
                {
                    actual = actual->der;
                }
            }
            if (strcmp(fecha, padre->fecha) < 0)
            {
                padre->izq = nuevo;
            }
            else
            {
                padre->der = nuevo;
            }
        }
        (*total)++;
        nodosABBF++;

        cABB_Falta.temp = 0.5;
        if(cABB_Falta.temp > cABB_Falta.max){
            cABB_Falta.max = cABB_Falta.temp;
        }
        cABB_Falta.cantidad++;
        cABB_Falta.sumatoria += cABB_Falta.temp;

        return 1;
    }
    else if (res == 0)
    { // Fecha encontrada, pero no hay evento a esa hora → agregarlo a la lista
        NodoEvento *nuevoEvento = crearNodoLVO(evento);
        if (nuevoEvento == NULL)
        {
            return 0; // Error al crear evento
        }
        // Insertar al final de la lista
        ant->sig = nuevoEvento;
        (*total)++;
        return 1;
    }
    // Si res == 1 → ya existe evento a esa hora → no agregar nada
    return 0;
}

// BAJA
int eliminarABB_F(ABB_F *arbol, char fecha[], Evento evento, int *total)
{
    NodoEvento *cur = NULL;
    NodoEvento *ant = NULL;
    int pos = -1;

    int res = localizarABB_F(arbol, fecha, evento, *total, &cur, &ant);
    if (res != 1)
    {
        return 0; // No encontrado o no hay evento a esa hora
    }

    // Buscar evento exacto en la lista
    NodoEvento *actual = arbol->cur->listaEventos;
    NodoEvento *anterior = NULL;
    while (actual != NULL)
    {
        if (compararEventos(actual->evento, evento) == 1)
        {
            // Encontrado → eliminarlo
            if (anterior == NULL)
            { // Primer nodo
                arbol->cur->listaEventos = actual->sig;
            }
            else
            {
                anterior->sig = actual->sig;
            }
            free(actual);
            (*total)--;

            // Si la lista quedó vacía → eliminar nodo del ABB_F
            if (arbol->cur->listaEventos == NULL)
            {
                NodoABB_F *nodoAEliminar = arbol->cur;

                // Caso: sin hijos
                if (nodoAEliminar->izq == NULL && nodoAEliminar->der == NULL)
                {
                    if (arbol->cur == arbol->raiz)
                    {
                        arbol->raiz = NULL;
                    }
                    else if (arbol->ant->izq == arbol->cur)
                    {
                        arbol->ant->izq = NULL;
                    }
                    else
                    {
                        arbol->ant->der = NULL;
                    }
                    free(nodoAEliminar);

                    cABB_Fbaja.temp=0.5;

                }
                // Caso: solo hijo izquierdo
                else if (nodoAEliminar->der == NULL)
                {
                    if (arbol->cur == arbol->raiz)
                    {
                        arbol->raiz = nodoAEliminar->izq;
                    }
                    else if (arbol->ant->izq == arbol->cur)
                    {
                        arbol->ant->izq = nodoAEliminar->izq;
                    }
                    else
                    {
                        arbol->ant->der = nodoAEliminar->izq;
                    }
                    free(nodoAEliminar);

                    cABB_Fbaja.temp = 0.5;

                }
                // Caso: solo hijo derecho
                else if (nodoAEliminar->izq == NULL)
                {
                    if (arbol->cur == arbol->raiz)
                    {
                        arbol->raiz = nodoAEliminar->der;
                    }
                    else if (arbol->ant->izq == arbol->cur)
                    {
                        arbol->ant->izq = nodoAEliminar->der;
                    }
                    else
                    {
                        arbol->ant->der = nodoAEliminar->der;
                    }
                    free(nodoAEliminar);

                    cABB_Fbaja.temp=0.5;

                }
                // Caso: dos hijos
                else
                {
                    NodoABB_F *mayorIzq = nodoAEliminar->izq;
                    NodoABB_F *padreMayorIzq = nodoAEliminar;
                    while (mayorIzq->der != NULL)
                    {
                        padreMayorIzq = mayorIzq;
                        mayorIzq = mayorIzq->der;
                    }
                    // Reemplazar datos
                    strcpy(nodoAEliminar->fecha, mayorIzq->fecha);
                    nodoAEliminar->listaEventos = mayorIzq->listaEventos;

                    // Ajustar enlaces
                    if (padreMayorIzq->der == mayorIzq)
                    {
                        padreMayorIzq->der = mayorIzq->izq;
                    }
                    else
                    {
                        padreMayorIzq->izq = mayorIzq->izq;
                    }
                    free(mayorIzq);

                    cABB_Fbaja.temp = 1.5;  //1 de copiar datos + 0,5 actualizar puntero.

                }
                nodosABBF--;
            }

            if(cABB_Fbaja.temp > cABB_Fbaja.max){
                cABB_Fbaja.max = cABB_Fbaja.temp;
            }
            cABB_Fbaja.cantidad++;
            cABB_Fbaja.sumatoria += cABB_Fbaja.temp;

            return 1;
        }
        anterior = actual;
        actual = actual->sig;
    }

    return 0; // Evento no encontrado exactamente
}

// Llamar con la raiz
void mostrarABB_FPreorden(NodoABB_F *nodo)
{
    if (nodo != NULL)
    {
        printf("Fecha nodo: %s\n", nodo->fecha);

        // Mostrar todos los eventos de la lista de esta fecha
        NodoEvento *cur = nodo->listaEventos;
        while (cur != NULL)
        {
            mostrarEvento(cur->evento);
            cur = cur->sig;
        }

        // Mostrar info de hijos si existen
        if (nodo->izq != NULL)
        {
            printf("  Hijo izquierdo: %s\n", nodo->izq->fecha);
        }
        if (nodo->der != NULL)
        {
            printf("  Hijo derecho: %s\n", nodo->der->fecha);
        }
        printf("-------------------------------------------------\n");

        // Recorrer hijo izquierdo
        mostrarABB_FPreorden(nodo->izq);

        // Recorrer hijo derecho
        mostrarABB_FPreorden(nodo->der);
    }
}

// todos los eventos en listaEventos tienen la misma fecha, que se puede obtener de:
// ------------- >    nodo.fecha
// y se puede acceder a un evento en particular asi
// ------------->    nodo->listaEventos->evento (->sig , .lugar , .hora , .evento)

//------------------------------

void limpiarSublistaEventos(NodoEvento *cabeza)
{
    NodoEvento *aux;
    while (cabeza != NULL)
    {
        aux = cabeza;
        cabeza = cabeza->sig;
        free(aux);
    }
}
void limpiarABB(NodoABB *nodo)
{
    if (nodo == NULL)
        return;

    limpiarABB(nodo->izq);
    limpiarABB(nodo->der);
    free(nodo);
}

void limpiarABB_F(NodoABB_F *nodo)
{
    if (nodo == NULL)
        return;
    if (nodo->izq != NULL)
    {
        limpiarABB_F(nodo->izq);
    }
    if (nodo->der != NULL)
    {
        limpiarABB_F(nodo->der);
    }
    limpiarSublistaEventos(nodo->listaEventos);
    free(nodo);
}

int Lectura_Operaciones(ABB *arbol, ABB_F *arbol_f, LSOBB lista[], LSOBB_F lista_f[], int *totalLSOBB, int *totalLSOBB_F, int *totalABB, int *totalABB_F)
{
    FILE *fp;
    int codigoOperador;
    Evento aux;
    char fechaAux[11];

    nodosABBF = 0;
    eventosLSOBB_Fglobal = 0;

    if ((fp = fopen("Operaciones.txt", "r")) == NULL)
    {
        printf("Error al abrir el archivo de operaciones.\n");
        system("pause");
        return 0;
    }

    while (!feof(fp))
    {
        fscanf(fp, "%d", &codigoOperador);
        fscanf(fp, " %[^\n]", fechaAux); // fecha: formato AAAA-MM-DD

        if (codigoOperador == 1 || codigoOperador == 2)
        {
            fscanf(fp, " %[^\n]", aux.evento);
            fscanf(fp, "%d", &aux.hora);
            fscanf(fp, " %[^\n]", aux.lugar);

            LSOBB evAux;
            evAux.evento = aux;
            strcpy(evAux.fecha, fechaAux);

            if (codigoOperador == 1)
            { // ALTA
                altaLSOBB(lista, evAux, totalLSOBB);

                altaLSOBB_F(lista_f, fechaAux, aux, totalLSOBB_F);

                altaABB(arbol, fechaAux, aux, totalABB);

                altaABB_F(arbol_f, fechaAux, aux, totalABB_F);
            }
            else
            { // BAJA
                eliminarLSOBB(lista, evAux, totalLSOBB);

                eliminarLSOBB_F(lista_f, fechaAux, aux, totalLSOBB_F);

                eliminarABB(arbol, fechaAux, aux, totalABB);

                eliminarABB_F(arbol_f, fechaAux, aux, totalABB_F);
            }
        }
        else if (codigoOperador == 3)
        {
            // Evocar en estructuras
            evocacionLSOBB(lista, fechaAux, *totalLSOBB);

            evocacionLSOBB_F(lista_f, fechaAux, *totalLSOBB_F);

            evocacionABB(arbol, fechaAux);

            evocacionABB_F(arbol_f, fechaAux);
        }
        else
        {
            printf(">> Código no reconocido: %d\n", codigoOperador);
        }
    }

    fclose(fp);
    return 1;
}

void comparacionEstructuras(ABB *arbol, ABB_F *arbol_f, LSOBB lista[], LSOBB_F lista_f[], int *totalLSOBB, int *totalLSOBB_F, int *totalABB, int *totalABB_F)
{
    int i = 0;
    *totalLSOBB = 0;
    for (i = 0; i < *totalLSOBB_F; i++)
    {
        limpiarSublistaEventos(lista_f[i].listaEventos);
        lista_f[i].listaEventos = NULL;
    }
    *totalLSOBB_F = 0;

    // limpiarABB(arbol->raiz);
    limpiarABB_F(arbol_f->raiz);
    arbol->raiz = NULL;
    arbol_f->raiz = NULL;
    *totalABB = 0;
    *totalABB_F = 0;

    Lectura_Operaciones(arbol, arbol_f, lista, lista_f, totalLSOBB, totalLSOBB_F, totalABB, totalABB_F);
    cuadroComp();
}

//--------------------------------------

NodoEvento *crearNodoLVO(Evento e)
{
    NodoEvento *nuevoNodo = (NodoEvento *)malloc(sizeof(NodoEvento));
    if (!nuevoNodo)
    {
        printf("Error al asignar memoria para el nuevo nodo.\n");
        exit(1);
    }
    nuevoNodo->evento = e;
    nuevoNodo->sig = NULL;
    return nuevoNodo;
}

// RETORNA 1 SI SON IGUALES 0 SI NO
int compararEventos(Evento evento1, Evento evento2)
{
    if ((strcasecmp(evento1.evento, evento2.evento)) == 0 && evento1.hora == evento2.hora && (strcasecmp(evento1.lugar, evento2.lugar)) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void mostrarEvento(Evento e)
{
    printf("Evento: %s\n", e.evento);
    printf("Hora: %d\n", e.hora);
    printf("Lugar: %s\n", e.lugar);
}

void cuadroComp()
{

    system("cls");
    system("color 03");
    printf("##======================================================================================================##\n");
    printf("||                                    COMPARACION DE ESTRUCTURAS                                        ||\n");
    printf("##======================================================================================================##\n");
    printf("||                      ||  COSTOS LSOBB   ||   COSTOS LSOBB_F   ||   COSTOS ABB    ||   COSTOS ABB_F   ||\n");
    printf("##======================================================================================================##\n");

    printf("|| MAX. EVOC.           ||     %.3f     ||     %.3f     ||      %.3f     ||      %.3f    ||\n",
        cLSOevoc.max, cLSO_Fevoc.max, cABBevoc.max, cABB_Fevoc.max);

    printf("|| MED. EVOC.           ||     %.3f     ||     %.3f     ||      %.3f     ||      %.3f    ||\n",
        (cLSOevoc.cantidad     != 0) ? cLSOevoc.sumatoria     / cLSOevoc.cantidad     : 0.0,
        (cLSO_Fevoc.cantidad   != 0) ? cLSO_Fevoc.sumatoria   / cLSO_Fevoc.cantidad   : 0.0,
        (cABBevoc.cantidad     != 0) ? cABBevoc.sumatoria     / cABBevoc.cantidad     : 0.0,
        (cABB_Fevoc.cantidad   != 0) ? cABB_Fevoc.sumatoria   / cABB_Fevoc.cantidad  : 0.0);

    printf("|| MAX. ALTA            ||     %.3f     ||     %.3f     ||      %.3f     ||     %.3f     ||\n",
        cLSOalta.max, cLSO_Falta.max, cABBalta.max, cABB_Falta.max);

    printf("|| MED. ALTA            ||     %.3f     ||     %.3f     ||      %.3f     ||      %.3f     ||\n",
        (cLSOevoc.cantidad     != 0) ? cLSOalta.sumatoria     / cLSOalta.cantidad     : 0.0,
        (cLSO_Fevoc.cantidad   != 0) ? cLSO_Falta.sumatoria   / cLSO_Falta.cantidad   : 0.0,
        (cABBevoc.cantidad     != 0) ? cABBalta.sumatoria     / cABBalta.cantidad     : 0.0,
        (cABB_Fevoc.cantidad   != 0) ? cABB_Falta.sumatoria   / cABB_Falta.cantidad   : 0.0);

    printf("|| MAX. BAJA            ||     %.3f     ||     %.3f     ||      %.3f     ||      %.3f     ||\n",
        cLSObaja.max, cLSO_Fbaja.max, cABBbaja.max, cABB_Fbaja.max);

    printf("|| MED. BAJA            ||     %.3f     ||     %.3f     ||      %.3f     ||      %.3f     ||\n",
        (cLSOevoc.cantidad     != 0) ? cLSObaja.sumatoria     / cLSObaja.cantidad     : 0.0,
        (cLSO_Fevoc.cantidad   != 0) ? cLSO_Fbaja.sumatoria   / cLSO_Fbaja.cantidad   : 0.0,
        (cABBevoc.cantidad     != 0) ? cABBbaja.sumatoria     / cABBbaja.cantidad     : 0.0,
        (cABB_Fevoc.cantidad   != 0) ? cABB_Fbaja.sumatoria   / cABB_Fbaja.cantidad   : 0.0);

    printf("##======================================================================================================##\n");
    system("pause");
}