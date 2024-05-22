// htab_lookup_add.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00



#include <stdlib.h>
#include "htab.h"
#include "htab_inc.h"
htab_item_t *create_item(htab_t * t, htab_key_t key);

htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){
    //printf("HERE1 %s\n", key);
    char * deep_key = (char*)calloc(strlen(key)+1,1);
    if (deep_key==NULL){
        return NULL;
    }
    memcpy(deep_key,key,strlen(key));
    //printf("HERE3 %s\n", deep_key);
    deep_key[strlen(key)] = '\0';
    //printf("HERE3 %s\n", deep_key);

    size_t index = (htab_hash_function(deep_key) % htab_bucket_count(t));
    //printf("index=%ld\n", index);
    //printf("ptr_pole[index]=%ld ",index);
    if (t->ptr_pole[index] == NULL){
        t->ptr_pole[index] = create_item(t, deep_key); //return (htab_item_t *)
        if (t->ptr_pole[index] == NULL){
            free(deep_key);
            return NULL;
        }
        //printf("Pridano prvni %s\n", t->ptr_pole[index]->data.key);
        return &t->ptr_pole[index]->data;
    }
    htab_item_t *actual_ptr = t->ptr_pole[index];
    while (true){
        if (!strcmp(actual_ptr->data.key, deep_key)){
            //printf("Nalezeno %s\n", actual_ptr->data.key);
            free(deep_key);
            return &actual_ptr->data;
        }else{
            if (actual_ptr->next == NULL){
                actual_ptr->next = create_item(t,deep_key);
                if (actual_ptr->next == NULL){
                    free(deep_key);
                    return NULL;
                }
                //printf("Pridano %s\n", actual_ptr->next->data.key);
                //actual_ptr->next->data.value++;
                return &actual_ptr->next->data;
            }
            actual_ptr = actual_ptr->next;
        }
    }
}
