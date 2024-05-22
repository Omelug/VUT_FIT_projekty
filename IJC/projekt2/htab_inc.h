// htab_inc.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#ifndef PROJEKT2_HTAB_INC_H
#define PROJEKT2_HTAB_INC_H
#include "htab.h"
    struct htab_item;
    typedef struct htab_item htab_item_t;

    typedef struct htab_item{
        htab_pair_t data;
        htab_item_t *next;
    } htab_item;

    typedef struct htab{
        size_t size; //pocet [htab_pair_t,next]
        size_t arr_size; // pocet ptr nasleduiciho pole
        htab_item_t **ptr_pole;
    } htab;
    //void htab_for_each_long(const htab_t * t, void (*f)(htab_pair_t *data, long *p_a), long *a);
#endif