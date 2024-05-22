// htab_hash_function.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <stdint.h>
#include "htab.h"


#ifdef MY_HASH_FUN_TEST

#else
size_t htab_hash_function(htab_key_t str){
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}
#endif