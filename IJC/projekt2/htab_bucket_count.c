// htab_bucket_count.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include "htab.h"
#include "htab_inc.h"

size_t htab_bucket_count(const htab_t * t){
    return t->arr_size;
}