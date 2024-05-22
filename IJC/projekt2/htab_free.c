// htab_free.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <stdlib.h>
#include "htab.h"
#include "htab_inc.h"

void htab_free(htab_t * t){
    htab_clear(t);
    free(t);
}