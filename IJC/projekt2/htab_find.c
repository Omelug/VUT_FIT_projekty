// htab_find.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include "htab.h"
#include "htab_inc.h"

htab_pair_t * htab_find(const htab_t * t, htab_key_t key){
    size_t index = (htab_hash_function(key) % htab_bucket_count(t));
    htab_item* actual_ptr = t->ptr_pole[index];
    while (actual_ptr !=NULL){
        if (!strcmp(actual_ptr->data.key, key)){
            break;
        }
        actual_ptr = actual_ptr->next;
    }
    if (actual_ptr == NULL){
        return NULL;
    }
    return &actual_ptr->data;
}