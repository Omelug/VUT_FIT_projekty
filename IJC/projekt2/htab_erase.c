// htab_erase.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <stdlib.h>
#include "htab.h"
#include "htab_inc.h"

bool htab_erase(htab_t * t, htab_key_t key){
    size_t index = (htab_hash_function(key) % htab_bucket_count(t));

    htab_item* actual_ptr = t->ptr_pole[index];
    if (!strcmp(actual_ptr->data.key, key)){
        t->ptr_pole[index] = t->ptr_pole[index]->next;
        free((char*)actual_ptr->data.key);
        free(actual_ptr);
        return true;
    }
    while (actual_ptr->next !=NULL){
        if (!strcmp(actual_ptr->next->data.key, key)){
            htab_item_t * temp = actual_ptr->next;
            actual_ptr->next = actual_ptr->next->next;
            free((char*)temp->data.key);
            free(temp);
            return true;
        }
        actual_ptr = actual_ptr->next;
    }
    return false;
}