// htab_init.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <malloc.h>
#include "htab.h"
#include "htab_inc.h"

htab_t *htab_init(const size_t n){
    htab_t* new_htab = malloc(sizeof(htab_t));
    if (new_htab == NULL){
        return NULL;
    }
    new_htab->size = 0;
    new_htab->arr_size = n;
    new_htab->ptr_pole = calloc(n,sizeof (htab_item *));
    if (new_htab->ptr_pole == NULL){
        free(new_htab);
        return NULL;
    }
    return new_htab;
}