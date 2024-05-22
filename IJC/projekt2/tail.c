// tail.c
// testovano na gcc 13.2,0,  16.4.2024
// IJC-DU2
// Autor: Vít Janeček, VUT FIT
// xjanecv00


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
int const LINE_LIMIT = 2047+1; //kontrolovat error na strerr i s \0 nebo s \n

typedef struct {
   long n;
   long read_ptr;
   long write_ptr;
   char * lines;
} cir_buf;
void cir_buf_r_add(cir_buf* cb){
    cb->read_ptr++;
    if (cb->read_ptr >= cb->n*LINE_LIMIT){
        cb->read_ptr -= cb->n*LINE_LIMIT;
    }
}
void cir_buf_w_add(cir_buf* cb){
    cb->write_ptr++;
    if (cb->write_ptr == cb->n*LINE_LIMIT){
        cb->write_ptr -= cb->n*LINE_LIMIT;
    }
}

cir_buf* cbuf_create(long n){
    cir_buf* buffer = malloc(sizeof(*buffer));
    if(buffer == NULL){
        fprintf(stderr,"malloc buffer is NULL\n");
        exit(EXIT_FAILURE);
    }
    buffer->n = n;
    buffer->read_ptr = 0;
    buffer->write_ptr = buffer->n;
    buffer->lines = calloc(n,LINE_LIMIT*sizeof(char));
    if(buffer->lines == NULL){
        fprintf(stderr,"malloc buffer->lines is NULL\n");
        exit(EXIT_FAILURE);
    }
    //TODO dat malloc a opravit tohleto
    for (int i=0;i< buffer->n;i++){
        buffer->lines[i] = '\0';
    }
    return buffer;
}

void cbuf_put(cir_buf* cb,const char* line){
    for (int i = 0; line[i] != '\n' ; ++i) {
        cb->lines[cb->write_ptr] = line[i];
        cir_buf_w_add(cb);
    }

    cb->lines[cb->write_ptr] = '\n';
    cir_buf_w_add(cb);

    while (1){
        if (cb->lines[cb->read_ptr]  == '\0' || cb->lines[cb->read_ptr]  == '\n')
            break;
        cir_buf_r_add(cb);
    }
    cir_buf_r_add(cb);
}
void cbuf_get(cir_buf* cb){
    //cbuf_print(cb);

    for (int i = 0; i < cb->n; ++i) {
        if(cb->lines[cb->read_ptr]  == '\0'){
            cir_buf_r_add(cb);
            continue;
        }
        while (cb->lines[cb->read_ptr]  != '\n'){

            printf("%c", cb->lines[cb->read_ptr]);
            cir_buf_r_add(cb);
        }
        printf("\n");
        //printf("| \n ^ %d\n",i);
        cir_buf_r_add(cb);
    }
}

void cbuf_free(cir_buf* cb){
    free(cb->lines);
    free(cb);
}

/*void cbuf_print(cir_buf* cb){
    printf("--------------\n");
    for (int i = 0; i < cb->n*LINE_LIMIT; ++i) {
        if(cb->lines[i] == '\0'){
            printf("0");
        }else if (cb->lines[i] == '\n'){
            printf("n");
        }else{
            printf("%c", cb->lines[i]);
        }
    }
    printf("\nR:%d W:%d loop: %d\n", cb->read_ptr, cb->write_ptr,cb->n*LINE_LIMIT);
    printf("--------------\n");
}*/

int main(int argc, char *argv[]){
    long n = 10;
    FILE * in = stdin;
    for (int i = 1; i < argc; ++i) {
        //printf("%d --- %s %d\n", i, argv[i], argc);
        if(!strcmp(argv[i], "-n")){
            //printf("%d %d\n",i+1, i+1<argc);
            if(i+1<argc){
                char *end_check;
                errno = 0;
                i++;
                n = strtol(argv[i], &end_check,0);
                if (n == 0 && errno != 0) {
                    fscanf(stderr,"Invalid number for n (%s)\n", argv[i+1]);
                    exit(EINVAL);
                }
                if (*end_check != '\0') {
                    fprintf(stderr, "No valid number %s \n", argv[i+1]);
                    exit(EINVAL);
                }
            }else{
                fprintf(stderr, "No number for n \n");
                exit(EINVAL);
            }
        }else{
            if(in == stdin){
                in = fopen(argv[i],"r");
                if(in == NULL){
                    fprintf(stderr,"Invalid file\n");
                    exit(EINVAL);
                }
            }else{
                fprintf(stderr,"Only one file can be in arguments");
                exit(EINVAL);
            }
        }

    }
    if(n == 0){
        exit(0); //nic nealokovat, jen pokojeně zemřít
    }
    cir_buf* buffer = cbuf_create(n);
    //printf("HERE  %ld", n);
    //exit(41);

    int c;
    char line[LINE_LIMIT];
    int i = 0;
    bool error = 1;
    bool skip_n = 0;
    int line_finished = -1; //pokud je lajna ukoncena -1, neukoncena, vraci i

    //cbuf_print(buffer);
    while((c=getc(in))!=EOF){
        if(skip_n){
            if(c == '\n')
                skip_n = 0;
            continue;
        }
        if( i == LINE_LIMIT-1){  //pokud je posledni co se vejde do lajny, zmenit na \n
            if(error){
                fprintf(stderr,"Warning: cutted to limit %d\n", LINE_LIMIT);
                error = 0;
            }
            if(c != '\n')
                skip_n = 1;
            c = '\n';
        }

        line[i] = (char)c;
        i++;

        line_finished = i;

        if (c=='\n'){
            //printf("aduawd %s", line);
            //printf("\nR:%d W:%d\n", buffer->read_ptr, buffer->write_ptr);
            cbuf_put(buffer,line);
            i=0;
            line_finished = -1;
        }
    }

    fclose(in);
    //printf("%s", line);
    if(line_finished != -1){
        if( line_finished == LINE_LIMIT-1){  //pokud je posledni co se vejde do lajny, zmenit na \n
            if(error){
                fprintf(stderr,"Warning: cutted to limit %d\n", LINE_LIMIT);
            }
        }
        line[line_finished] = '\n';
        //printf("HERE %s", line);
        cbuf_put(buffer,line);
    }

    //TODO z nejakeho duvodu se prida \n

    cbuf_get(buffer);
    cbuf_free(buffer);
    return 0;
}
