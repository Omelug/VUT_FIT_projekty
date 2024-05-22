// htab_for_each.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00



#include "htab.h"
#include "htab_inc.h"

// for_each: projde všechny záznamy a zavolá na ně funkci f
// Pozor: f nesmí měnit klíč .key ani přidávat/rušit položky
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
    for(size_t i =0; i< htab_bucket_count(t);i++){
        htab_item_t * act_ptr = t->ptr_pole[i];
        while(act_ptr != NULL){
            f(&act_ptr->data);
            act_ptr = act_ptr->next;
        }
    }
}