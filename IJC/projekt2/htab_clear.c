// htab_clear.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00

#include <stdlib.h>
#include "htab.h"
#include "htab_inc.h"

// ruší všechny záznamy
void htab_clear(htab_t * t){
    //htab_for_each(t, (void (*)(htab_pair_t *)) free); //TODO chek if working
    //for all call htab_erase
    for (size_t i = 0; i < htab_bucket_count(t); ++i) {
        htab_item_t * act_ptr = t->ptr_pole[i];
        while(act_ptr != NULL){
            htab_item_t * temp = act_ptr;
            free((char*)act_ptr->data.key);
            act_ptr = act_ptr->next;
            free(temp);
        }
    }
    free(t->ptr_pole);
    t->size = 0;
    t->arr_size = 0;
}