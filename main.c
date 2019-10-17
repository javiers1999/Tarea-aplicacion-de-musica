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
void crearCancion(char*, char*, char*, char*, Map*, Map*, Map*, list*);
void importarCancionesCSV(Map* , Map* , Map* , list* );
/**===================== **/
char * _strdup(const char*);
const char *get_csv_field(char*, int);
long long stringHash(const void*);
int stringEqual(const void*, const void*);
/**=====================**/

int main(void){
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
                  break;
          case 3:
                  break;
          case 4:
                  break;
          case 5:
                  break;
          case 6:
                  break;
          case 7:
                  break;
          case 8:
                  break;
          case 9:
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
        crearCancion(nombre, artista, duracion, album, mapCanciones, mapArtistas, mapAlbumes, listaCanciones);
        if (searchMap(mapCanciones, nombre) != NULL){
          cont++;
        }
    }
    fclose(cancionesCsv);
    printf("%d canciones importadas exitosamente\n", cont);
}

void crearCancion(char* nombre, char* artista, char* duracion, char* album, Map* mapCanciones, Map* mapArtistas, Map* mapAlbumes, list* listaCanciones){
    Album* albumAgregar;
    albumAgregar = searchMap(mapAlbumes, album);
    if (albumAgregar == NULL){
      crearAlbum(album, "01-01-1970", mapAlbumes);
      albumAgregar = searchMap(mapAlbumes, album);
    }
    Artista* artistaAgregar = searchMap(mapArtistas, artista);
    if (artistaAgregar == NULL){
      crearArtista(nombre, mapArtistas);
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

void crearAlbum(char* nombre, char* fecha, Map* mapAlbumes){
    if (searchMap(mapAlbumes, nombre) == NULL){
        Album* nuevo = malloc(sizeof(Album));
        nuevo->Nombre = nombre;
        nuevo->Fecha = fecha;
        nuevo->listaCanciones = list_create_empty();
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
