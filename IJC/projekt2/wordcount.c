// wordcount.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00

#include "htab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"


const int TABLE_INIT_SIZE = 15000; //aby se nemuselo alokovat moc, ale aby to bylo normáln2 rychle i pro vetsi listy
const int WORD_MAX = 256;

void print_pair(htab_pair_t * mi){
    printf("%s\t%d\n", mi->key, mi->value);
}

int main() {

    htab_t* table = htab_init(TABLE_INIT_SIZE);
    if (table == NULL){
        fprintf(stderr,"malloc htab init is NULL\n");
        exit(42);
    }
    char slovo[WORD_MAX];
    slovo[WORD_MAX-1] = '\0'; //abych to mohl bezpecne vypisovat jako string

    while(read_word( slovo, WORD_MAX, stdin) != EOF){
        //printf(" %s=>", slovo);
        //ptr_pole(table);
        htab_pair_t * pnp = htab_lookup_add(table, slovo); //TODO pozor aby nebyly klice s jinym \0 rozdilne
        if(pnp == NULL){
            fprintf(stderr, "New pair error\n");
            htab_free(table);
            exit(1);
        }else{
            pnp->value++;
        }
        //ptr_pole(table);
        //htab_for_each(table,print_pair);
        //printf("----------------------\n");
    }

    //ptr_pole(table);

    /*htab_lookup_add(table, "ahoj");
    htab_lookup_add(table, "ahoj1");
    htab_lookup_add(table, "ahoj2");
    htab_lookup_add(table, "ahoj3");
    htab_lookup_add(table, "ahoj4");

    htab_erase(table, "ahoj");
    htab_erase(table, "ahoj1");
    htab_erase(table, "ahoj2");
    htab_erase(table, "ahoj3");
    htab_erase(table, "ahoj4");*/
    //ptr_pole(table);
    htab_for_each(table,print_pair);
    //htab_statistics(table);

    //ptr_pole(table);

    htab_free(table);
    exit(0);
}


