// libhtab.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00

#include <string.h>
#include <stdbool.h>
#include "htab.h"
#include "htab_inc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void ptr_pole(htab_t * t){
    printf("\n");
    for(size_t i =0; i< htab_bucket_count(t);i++){
        //printf(" %d \n", t->ptr_pole[i]);
        htab_item_t *actual = t->ptr_pole[i];
        while (actual != NULL){
            printf("->%s(%p)", actual->data.key, (void *) actual);
            actual = actual->next;
        }
        printf("->NULL");
        printf("\n");
    }
    printf("\n");
}