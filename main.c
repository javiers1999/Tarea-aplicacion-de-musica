/**
  APLICACION DE MUSICA
  NOMBRES: FELIPE TAPIA VERA
           JAVIER RODRIGUEZ SAAVEDRA
  ESTRUCTURA DE DATOS 2S-2019
**/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Map.h"
#include "list.h"

typedef struct {
    char* Nombre;
    char* Artista;
    char* Duracion;
    char* Album;
} Cancion;

typedef struct {
    char* Nombre;
    char* Fecha;
    list* listaCanciones;
} Album;

typedef struct {
    char* nombre;
    list* listaCanciones;
} Artista;

int menuPrincipal();
void crearAlbum(char*, char*, Map*);
void crearArtista(char*, Map*);
void crearCancion(char*, char*, char*, char*, char*, Map*, Map*, Map*, list*);
void importarCancionesCSV(Map* , Map* , Map* , list* );
void exportarCancionesCSV(list*, Map*, Map*);
void agregarAlbum(Map*, Map*, Map*, list*);
void agregarCancion(Map*, Map*, Map*, list*);
void eliminarCancion(Map*, Map*, Map*, list*);
void buscarCancion(Map*);
void buscarCancionArtista(Map*, Map*);
void buscarAlbum(Map*);
/**===================== **/
char* _strdup(const char*);
const char* get_csv_field(char*, int);
long long stringHash(const void*);
int stringEqual(const void*, const void*);
/**=====================**/

int main(void){
    srand(time(NULL));      // SE UTILIZA PARA EVITAR QUE RAND GENERE EL MISMO PATRON DE NUMEROS CADA VEZ QUE SE UTILIZA EL PROGRAMA
    Map* mapCanciones = createMap(stringHash, stringEqual);
    Map* mapAlbumes = createMap(stringHash, stringEqual);
    Map* mapArtistas = createMap(stringHash, stringEqual);
    list* listaCanciones = list_create_empty();

    do {
        switch (menuPrincipal()){
            case 1:
                    importarCancionesCSV(mapCanciones, mapAlbumes, mapArtistas, listaCanciones);
                    break;
          case 2:
                  exportarCancionesCSV(listaCanciones, mapCanciones, mapAlbumes);
                  break;
          case 3:
                  agregarAlbum(mapAlbumes, mapCanciones, mapArtistas, listaCanciones);
                  break;
          case 4:
                  agregarCancion(mapAlbumes, mapCanciones, mapArtistas, listaCanciones);
                  break;
          case 5:
                  eliminarCancion(mapAlbumes, mapCanciones, mapArtistas, listaCanciones);
                  break;
          case 6:
                  buscarCancion(mapCanciones);
                  break;
          case 7:
                  buscarCancionArtista(mapArtistas, mapCanciones);
                  break;
          case 8:
                  buscarAlbum(mapAlbumes);
                  break;
          case 9:
                  printf("Hasta Pronto!\n");
                  return 0;
          default:
                  return 0;
      }
  } while (1);
}

int menuPrincipal(){
    int opcion = 0;
    do {
        printf("Ingrese la opcion deseada:\n");
        printf("1. Importar musica de un archivo\n");
        printf("2. Exportar musica a un archivo\n");
        printf("3. Agregar Album\n");
        printf("4. Agregar Cancion\n");
        printf("5. Eliminar todas las canciones de un artista\n");
        printf("6. Buscar Cancion\n");
        printf("7. Buscar todas las canciones de un artista\n");
        printf("8. Buscar Album\n");
        printf("9. Salir\n");
        scanf("%d", &opcion);
    } while (opcion < 1 || opcion > 9);
    return opcion;
}

void importarCancionesCSV(Map* mapCanciones, Map* mapAlbumes, Map* mapArtistas, list* listaCanciones){
    FILE* cancionesCsv = fopen("canciones.csv", "r");
    if (cancionesCsv == NULL){
        printf("ERROR\n");
        return;
    }
    char linea[1025];
    int cont = 0;
    fgets(linea, 1024, cancionesCsv);
    while (fgets(linea, 1024, cancionesCsv) != NULL){
        char* nombre = _strdup(get_csv_field(linea, 1));
        char* artista = _strdup(get_csv_field(linea, 2));
        char* duracion = _strdup(get_csv_field(linea, 3));
        char* album = _strdup(get_csv_field(linea, 4));
        char* fecha = _strdup(get_csv_field(linea, 5));
        if (searchMap(mapCanciones, nombre) == NULL){
            crearCancion(nombre, artista, duracion, album, fecha, mapCanciones, mapArtistas, mapAlbumes, listaCanciones);
            cont++;
        }
    }
    fclose(cancionesCsv);
    printf("%d canciones importadas exitosamente\n", cont);
    printf("Hay %ld canciones en el sistema\n", mapCount(mapCanciones));
}

void exportarCancionesCSV(list* listaCanciones, Map* mapCanciones, Map* mapAlbumes){
    char filename[1024] = "Exportar";
    char random [7];
    char ext [5] = ".csv";
    sprintf(random, "%d", (rand()%100000));     // SE GENERA NUMERO ALEATORIO PARA EVITAR SOBREESCRIBIR ARCHIVO AL EJECUTAR VARIAS VECES EL PROGRAMA
    strcat(filename, random);
    strcat(filename, ext);
    FILE* cancionesCSV = fopen(filename, "w");
    if (cancionesCSV == NULL){
        printf("Error al crear el archivo\n");
        return;
    }
    char* cancionActual = list_first(listaCanciones);
    do {
        Cancion* cancionActualStruct = searchMap(mapCanciones, cancionActual);
        char* albumCancionActual = cancionActualStruct->Album;
        char* duracionCancionActual = cancionActualStruct->Duracion;
        char* artistaCancionActual = cancionActualStruct->Artista;
        Album* albumCancionActualStruct = searchMap(mapAlbumes, albumCancionActual);
        char* fechaAlbumCancionActual = albumCancionActualStruct->Fecha;
        fprintf(cancionesCSV, "%s,", cancionActual);
        fprintf(cancionesCSV, "%s,", artistaCancionActual);
        fprintf(cancionesCSV, "%s,", duracionCancionActual);
        fprintf(cancionesCSV, "%s,", albumCancionActual);
        fprintf(cancionesCSV, "%s", fechaAlbumCancionActual);
        fprintf(cancionesCSV, "\n");
        cancionActual = list_next(listaCanciones);
    } while (cancionActual != NULL);
    if (fclose(cancionesCSV)!=0){
        printf("Error al escribir el archivo. Datos no guardados correctamente\n");
    }
    return;
}


void agregarAlbum(Map* mapAlbumes, Map* mapCanciones, Map* mapArtistas, list* listaCanciones){ /** opcion 3 **/
    int option;

    char nombreAlbum[1025];
    char fecha[1025];

    char nombreCancion[1025];
    char duracion[1025];
    char artista[1025];

    printf("Escriba nombre del album : ");
    scanf("%s", nombreAlbum);
    printf("\nEscriba fecha de lanzamiento : ");
    scanf("%s", fecha);
    if (searchMap(mapAlbumes, nombreAlbum) == NULL ){
        crearAlbum(nombreAlbum, fecha, mapAlbumes);
    } else {
        printf("\nAlbum existe\n");
    }
    printf("\nAlbum agregado\n");
    printf("Quiere agregar canciones al album? [1]SI  [2]NO  ");
    scanf("%d", &option);
    while ( option == 1){
        printf("\nEscriba nombre cancion: ");
        scanf("%s", nombreCancion);
        printf("\nEscriba duracion: ");
        scanf("%s", duracion);
        printf("\nEscriba artista: ");
        scanf("%s", artista);
        crearCancion(nombreCancion, artista , duracion, nombreAlbum, fecha, mapAlbumes, mapArtistas, mapCanciones, listaCanciones);
        printf("CANCION AGREGADA CON EXITO\nQuiere agregar canciones al album? [1]SI  [2]NO  ");
        scanf("%d", &option);
    }
    return;
}

void agregarCancion(Map* mapAlbumes, Map* mapCanciones, Map* mapArtistas, list* listaCanciones){
    char nombreCancion[1025];
    char duracion[1025];
    char artista[1025];
    char album[1025];
    char fecha[1025];

    printf("Escriba nombre de la cancion : ");
    scanf("%s", nombreCancion);
    printf("\nEscriba el artista de la cancion : ");
    scanf("%s", artista);
    printf("\nEscriba duracion de la cancion : ");
    scanf("%s", duracion);
    printf("\nEscriba el album");
    scanf("%s", album);
    printf("\nIngrese la fecha");
    scanf("%s", fecha);

    if(searchMap(mapAlbumes,album ) != NULL){
        if (searchMap(mapCanciones , nombreCancion) == NULL) {
            crearCancion(nombreCancion,artista,duracion,album, fecha, mapCanciones, mapArtistas, mapAlbumes, listaCanciones);
            printf("\nCancion agregada\n");
        }else{
            printf("Cancion ya existe");
        }
    }
}

void crearCancion(char* nombre, char* artista, char* duracion, char* album, char* fecha, Map* mapCanciones, Map* mapArtistas, Map* mapAlbumes, list* listaCanciones){
    Album* albumAgregar;
    albumAgregar = searchMap(mapAlbumes, album);
    if (albumAgregar == NULL){
      crearAlbum(album, fecha, mapAlbumes);
      albumAgregar = searchMap(mapAlbumes, album);
    }
    Artista* artistaAgregar = searchMap(mapArtistas, artista);
    if (artistaAgregar == NULL){
      crearArtista(artista, mapArtistas);
      artistaAgregar = searchMap(mapArtistas, artista);
    }
    Cancion* nuevo = malloc(sizeof(Cancion));
    nuevo->Nombre = nombre;
    nuevo->Artista = artista;
    nuevo->Duracion = duracion;
    nuevo->Album = album;
    insertMap(mapCanciones, nombre, nuevo);
    list_push_back(albumAgregar->listaCanciones, nombre);
    list_push_back(artistaAgregar->listaCanciones, nombre);
    list_push_back(listaCanciones, nombre);
    return;
}

void eliminarCancion(Map* mapAlbumes, Map* mapCanciones, Map* mapArtistas, list* listaCanciones){
    char buscar[1025];
    getchar();
    printf("Ingrese el nombre de la cancion a eliminar");
    scanf("%[^\n]s", buscar);
    Cancion* encontrada = searchMap(mapCanciones, buscar);
    if (encontrada){
        char* album = _strdup(encontrada->Album);
        char* artista = _strdup(encontrada->Artista);
        Album* albumCancionActualStruct = searchMap(mapAlbumes,album);
        char* cancionActual;
        cancionActual = list_first(albumCancionActualStruct->listaCanciones);
        while (cancionActual != buscar && cancionActual != NULL){
            cancionActual = list_next(albumCancionActualStruct->listaCanciones);
        }
        if (cancionActual == buscar){
            list_pop_current(albumCancionActualStruct->listaCanciones);
            printf("1..");
        }
        Artista* artistaCancionActualStruct = searchMap(mapArtistas, artista);
        cancionActual = list_first(artistaCancionActualStruct->listaCanciones);
        while (cancionActual != buscar && cancionActual != NULL){
            cancionActual = list_next(artistaCancionActualStruct->listaCanciones);
        }
        if (cancionActual == buscar){
            list_pop_current(albumCancionActualStruct->listaCanciones);
            printf("2..");
        }
        cancionActual = list_first(listaCanciones);
        while (cancionActual != buscar && cancionActual != NULL){
            cancionActual = list_next(listaCanciones);
        }
        if (cancionActual == buscar){
            list_pop_current(listaCanciones);
            printf("3..");
        }
        eraseKeyMap(mapCanciones, buscar);
        printf("Se ha eliminado exitosamente");

    } else {
        printf("No se pudo eliminar, no se encontro.\n");
    }
    return;
}

void buscarCancion(Map* mapCanciones){
    char buscar[1025];
    getchar();
    printf("Ingrese el nombre de la cancion a buscar: ");
    scanf("%[^\n]s", buscar);
    Cancion* encontrada = searchMap(mapCanciones, buscar);
    if(encontrada){
        printf("Nombre de la Cancion: %s\n", encontrada->Nombre);
        printf("Nombre del Artista: %s\n", encontrada->Artista);
        printf("Duracion de la Cancion: %s\n", encontrada->Duracion);
        printf("Album: %s\n", encontrada->Album);
    } else {
        printf("No Esta\n");
    }

}

void buscarCancionArtista(Map* mapArtista, Map* mapCanciones){
    char buscar[1025];
    getchar();
    printf("Ingrese el nombre del Artista: ");
    scanf("%[^\n]s", buscar);

    Artista* encontrado = searchMap(mapArtista, buscar);

    if (encontrado){
        char* imprimir;
        imprimir = _strdup(list_first(encontrado->listaCanciones));
        do {
            if (imprimir == NULL) break;
            Cancion* aImprimir = searchMap(mapCanciones, imprimir);
            printf("Nombre de la Cancion: %s\n", aImprimir->Nombre);
            printf("Duracion de la Cancion: %s\n", aImprimir->Duracion);
            printf("Album: %s\n", aImprimir->Album);

            imprimir = list_next(encontrado->listaCanciones);
        } while (imprimir != NULL);
    } else {
        printf("No se encontro al artista ingresado\n");
    }

    return;
}

void buscarAlbum(Map* mapAlbumes){
    char buscar[1025];
    getchar();
    printf("Ingrese el nombre del Album: ");
    scanf("%[^\n]s", buscar);

    Album* encontrado = searchMap(mapAlbumes, buscar);

    if (encontrado){
        char* imprimir;
        imprimir = _strdup(list_first(encontrado->listaCanciones));
        do {
            if (imprimir == NULL) break;
            printf("Nombre Cancion: %s\n", imprimir);
            imprimir = list_next(encontrado->listaCanciones);
        } while (imprimir != NULL);
    } else {
        printf("No se encontro el album ingresado\n");
    }
}

void crearArtista(char* nombre, Map* mapArtista){
    if (searchMap(mapArtista, nombre) == NULL){
        Artista* nuevo = malloc(sizeof(Artista));
        nuevo->nombre = nombre;
        nuevo->listaCanciones = list_create_empty();
        insertMap(mapArtista, nombre, nuevo);
    }
    return;
}

void crearAlbum(char* nombre, char* fecha, Map* mapAlbumes){
    if (searchMap(mapAlbumes, nombre) == NULL){
        Album* nuevo = malloc(sizeof(Album));
        nuevo->Nombre = nombre;
        nuevo->Fecha = fecha;
        nuevo->listaCanciones = list_create_empty();
        insertMap(mapAlbumes, nombre, nuevo);
    }
    return;
}

char * _strdup (const char *s) {
	size_t len = strlen (s) + 1;
    void *new = malloc (len);
	if (new == NULL) return NULL;
	return (char *) memcpy (new, s, len);
}
const char *get_csv_field (char * tmp, int i) {
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) return tok;
    }
    return NULL;
}

long long stringHash (const void * key){
    long long hash = 5381;
    const char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++)  hash = ((hash << 5 ) + hash) + tolower(*ptr);
    return hash;
}

int stringEqual (const void *key1, const void *key2){
    const char *A = key1;
    const char *B = key2;
    return strcmp(A,B) == 0;
}
