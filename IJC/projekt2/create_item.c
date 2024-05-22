// create_item.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <malloc.h>
#include "htab.h"
#include "htab_inc.h"

htab_item_t *create_item(htab_t *t, htab_key_t key) { //FIXME pozor deep copy of key ??

    htab_item_t* new_item = malloc(sizeof(htab_item));
    if (new_item == NULL){
        return NULL;
    }
    htab_pair_t new_pair;
    new_pair.key = key;
    new_pair.value= 0;
    new_item->data = new_pair;
    new_item->next = NULL;
    t->size++;
    // Poznámka1: Dobře promyslete chování této funkce k parametru key.
    return new_item;
}