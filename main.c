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
  list* listaAlbumes,
  list* listaCanciones
}

int menuPrincipal();

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
