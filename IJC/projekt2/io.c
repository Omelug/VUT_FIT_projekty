// wordcount.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "ctype.h"
#include "io.h"


int read_word(char *s, int max, FILE *f){
    int c;
    bool started = false;
    int word_i = 0;
    while((c=getc(f))!=EOF){
        //printf("%c", c);
        if(!started && isspace(c)){ //preskocit separator znaky na zacatku
            continue;
        }else{
            started = true;
        }
        if(word_i == max -1 || isspace(c)){
            if(word_i == max-1){
                fprintf(stderr,"Word has been shorted %s\n", s);
            }
            s[word_i] = '\0';
            while ((c=getc(f))!=EOF){
                if(!isspace(c)){
                    ungetc(c, f);
                    break;
                }
            }
            return word_i;
        }
        s[word_i] = (char)c;
        word_i++;
    }
    if(started){
        if(word_i == max - 1){
            fprintf(stderr,"Word has been shorted %s\n", s);
        }
        s[word_i] = '\0';
        return word_i;
    }

    return EOF;
}
