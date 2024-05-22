// htab_size.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include "htab.h"
#include "htab_inc.h"

size_t htab_size(const htab_t * t){
    return t->size;
}