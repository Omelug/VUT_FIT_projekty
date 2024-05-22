// htab_statistics.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <stdio.h>
#include "htab.h"
#include "htab_inc.h"

// výpočet a tisk statistik délky seznamů (min,max,avg) do stderr:

void save_min(htab_pair_t *data, long *min){
    if (*min == -1 || *min>data->value)
        *min = data->value;
}

void save_max(htab_pair_t *data,  long *max){
    if (*max == -1 || *max<data->value)
        *max = data->value;
}

void save_sum(htab_pair_t *data,  long *sum){
    *sum += data->value;
}

void htab_for_each_long(const htab_t * t, void (*f)(htab_pair_t *data, long *p_a), long *a){
    for(size_t i =0; i< htab_bucket_count(t);i++){
        htab_item_t * act_ptr = t->ptr_pole[i];
        while(act_ptr != NULL){
            f(&act_ptr->data, a);
            act_ptr = act_ptr->next;
        }
    }
}
void htab_statistics(const htab_t * t){
    long min = -1;
    long max = -1;

    //htab_for_each_long(t, save_min, &min);
    //htab_for_each_long(t, save_max, &max);
    //htab_for_each_long(t, save_sum, &sum);
    for (size_t i = 0; i < htab_bucket_count(t); ++i) {
        htab_item_t * act_ptr = t->ptr_pole[i];
        int n = 0;
        while(act_ptr != NULL){
            n++;
            act_ptr = act_ptr->next;
        }
        if(max < n || max == -1){max = n;};
        if(min > n || min == -1){min = n;};
    }

    fprintf(stderr, "min: %ld ", min);
    fprintf(stderr,"max: %lu ", max);
    fprintf(stderr,"avg: %f \n", (double )htab_size(t)/ (double )htab_bucket_count(t));
}