//TODO halvička

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

//#define DEBUG
#define SMF_CNT 9
#define ARG_CNT 5

/*
    $ ./proj2 L Z K TL TB
• L: počet lyžařů, L<20000
• Z: počet nástupních zastávek, 0<Z<=10
• K: kapacita skibusu, 10<=K<=100
• TL: Maximální čas v mikrosekundách, který lyžař čeká, než přijde na zastávku, 0<=TL<=10000
• TB: Maximální doba jízdy autobusu mezi dvěma zastávkami. 0<=TB<=1000

*/

sem_t **smf;
enum semafor {zapis, failed, waiting, bus_wait, in_bus , zas_w, bus_zastavka, umirajici, kapacita};


int* L_counter;
int* zastavky;
int* actual;
int* print_A;
int* args;

FILE * file;
enum argument{L, Z, K, TL,TB};



//NOTE: after arguments alloc
void  get_arg(char* argv[],int argument, const long min_inc, const long max_inc){
    char * last_vld;
    long result = strtol (argv[argument+1], &last_vld, 10);
    if(*last_vld != '\0' || result < min_inc || result > max_inc){
        printf("Invalid %s",argv[argument+1]);
        munmap(args, sizeof(int)*ARG_CNT);
        exit(1);
    }
    args[argument] = result;
}


int get_zas_count(){ //nutne zamknout zas_w
    int sum = 0;
    for (int i= 1; i <= args[Z];i++){
        sum += zastavky[i];
    }
    return sum;
}


#ifdef DEBUG
void vypis_semafors(){

    int r;
    sem_wait(smf[kapacita]);
    sem_wait(smf[umiraci]);
    sem_getvalue(smf[waiting], &r);
    fprintf(file, "W: %d | ", r);
    sem_getvalue(smf[failed], &r);
    fprintf(file, "F: %d |", r);
    sem_getvalue(smf[bus_wait], &r);
    fprintf(file, "BW: %d |", r);
    sem_getvalue(smf[in_bus], &r);
    fprintf(file, "IN: %d |", r);
    sem_getvalue(smf[bus_zastavka], &r);
    fprintf(file, "BZ: %d |", r);
    fprintf(file," zas:");
    for (int i = 0; i <= Z; ++i) {
        fprintf(file,"%d ", zastavky[i]);
    }
    fprintf(file, "| %d ", *L_counter);
    fprintf(file, "| %d ", *L_counter- get_zas_count());
    fprintf(file,"\n");
    sem_post(smf[umiraci]);
    sem_post(smf[kapacita]);
}
#endif

void vypis(const char * format, ...) {
    va_list va_args;
    va_start(va_args, format);
    sem_wait(smf[zapis]);

    (*print_A)++;
    fprintf(file, "%d:", *print_A);
    vfprintf(file, format, va_args);
#ifdef DEBUG
    vypis_semafors();
#endif
    fflush(file); //ulozit buffer
    sem_post(smf[zapis]);
    va_end(va_args);
}


void vypis_L(int idL,const char * format, ...) {
    va_list va_args;
    va_start(va_args, format);
    sem_wait(smf[zapis]);
    (*print_A)++;
#ifdef DEBUG
    for (int i = 0; i <idL; i++) {
        fprintf(file, "\t\t\t");
    }
#endif
    fprintf(file, "%d: L %d:", *print_A, idL);
    vfprintf(file, format, va_args);

#ifdef DEBUG
    for (int i = 0; i <idL; i++) {
            fprintf(file, "\t\t\t");
    }
    vypis_semafors();
#endif
    fflush(file);
    sem_post(smf[zapis]);
    va_end(va_args);
}


void prisel_na_zastavku(int idZ)
{
    sem_wait(smf[zas_w]);
    zastavky[idZ]++;
    sem_post(smf[zas_w]);

}
void umrit(){
    sem_wait(smf[umirajici]);
    (*L_counter)--;
    sem_post(smf[umirajici]);
}
void prijit_na_zastavku(int idL, int idZ){
    sem_wait(smf[zas_w]);
    zastavky[idZ] += 1;
    vypis_L(idL," arrived to %i\n", idZ);
    sem_post(smf[zas_w]);
}
int max(int a, int b){
    return a>b?a:b;
}
void cleanup(){
    fclose(file);
    munmap(L_counter,sizeof(int));
    munmap(zastavky,sizeof(int)*args[Z]+1);
    for (int i=0;i< SMF_CNT;i++){
        sem_destroy(smf[i]);
    }
    munmap(smf,sizeof(sem_t)*SMF_CNT);
    munmap(args,sizeof(int)*ARG_CNT);
}

int main(int argc, char *argv[]) {

    //printf("\033[32;1;1m"); // zapnuti MATRIXU
    //file = stdout;

    file = fopen("proj2.out","w+");
    if(file == NULL){
        fprintf(stderr, "Chyba pri otevirani souboru\n");
        exit(1);
    }

    if(argc != 6){
        fprintf(stderr,"Invalid number of arguments\n");
        cleanup();
        exit(1);
    }

    //argumenty,neocekavany vstup -> chyba error 1
    args = mmap(NULL, sizeof(int)*ARG_CNT, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    if(args == MAP_FAILED) {
        perror("Error args allocation");
        cleanup();
        exit(1);
    }

    get_arg(argv,L,1,20000-1);
    get_arg(argv,Z,1,10);
    get_arg(argv,K,10,100);
    get_arg(argv,TL,0,10000);
    get_arg(argv,TB,0,1000);

    //semafory
    smf = mmap(NULL, sizeof(sem_t*)*SMF_CNT, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    if(smf == MAP_FAILED) {
        perror("Error semafor allocation");
        cleanup();
        exit(1);
    }
    for(int i=0; i < SMF_CNT; i++) {
        smf[i]=mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
        if(smf[i] == MAP_FAILED) {
            printf("Error semafor %d allocation", i);
            cleanup();
            exit(1);
        }
    }

    L_counter = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    zastavky = mmap(NULL, sizeof(int)*Z+1, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    actual = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    print_A = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, 0, 0);
    if(L_counter == MAP_FAILED || zastavky == MAP_FAILED || actual == MAP_FAILED || print_A == MAP_FAILED) {
        perror("Error semafor allocation");
        cleanup();
        exit(1);
    }

    *print_A = 0;
    *L_counter = args[L];
    zastavky[0] = args[K];
    for (int i = 1; i < args[Z];i++){
        zastavky[i] = 0;
    }

    sem_init(smf[failed],1,0);
    sem_init(smf[waiting],1,0);
    sem_init(smf[bus_wait],1,0);
    sem_init(smf[bus_zastavka],1,0);
    sem_init(smf[in_bus],1,0);

    sem_init(smf[zas_w],1,1);
    sem_init(smf[zapis],1,1);
    sem_init(smf[umirajici],1,1);
    sem_init(smf[kapacita],1,1);


    int pid=fork();
    if (pid==-1) {
        fprintf(stderr,"Error: %s", strerror(errno));
        cleanup();
        kill(0, SIGKILL); //zabit vsechny ostatni
        exit(1);
    }
    if (pid==0) { //SKIBUS
        vypis(" BUS: started\n");
        do {
            for (int idZ = 1; idZ <= args[Z]; ++idZ) {
                usleep(rand() % (args[TB] + 1)); //jede na zastavku
                sem_wait(smf[zas_w]); //tady se nebude nic menit, dokud nevypadne za zastavky
                vypis(" BUS: arrived to %i\n", idZ);
                sem_wait(smf[kapacita]);
                    if (zastavky[0] != 0 && zastavky[idZ] != 0){
                        int F = get_zas_count() - zastavky[idZ];
                        int old_kapacita = zastavky[0];
                        sem_post(smf[kapacita]);
#ifdef DEBUG
                        vypis(" BUS: ceka tu %i/%i \n", zastavky[idZ],zastavky[idZ]+F); //ceka na nastup
#endif
                        *actual = idZ;
                        // proubodi vsechny cekajici na vsech zastavkach
                        sem_post(smf[waiting]);
                        for (int i = 0; i < zastavky[idZ]+F; ++i) {
                            sem_post(smf[waiting]);
                        }
                        sem_wait(smf[waiting]);

                        // ceka na vsechny jestli FAILNOU nebo nastoupí
                        for (int i=0;i < zastavky[idZ]+F;i++){
                            sem_wait(smf[bus_zastavka]);
                        }

                        //pusti FAILED
                        for (int i=0;i < F + max(zastavky[idZ]-old_kapacita,0);i++){
                            sem_post(smf[failed]);
                        }

                        //cekani na FAILED
                        for (int i=0;i < F + max(zastavky[idZ]-old_kapacita,0);i++){
                         sem_wait(smf[bus_zastavka]);
                        }

                        zastavky[idZ] = max(zastavky[idZ]-old_kapacita,0); //zastavka nabrana
                    }else{
                        //TODO
                        sem_post(smf[kapacita]);
                    }
                vypis(" BUS: leaving %i\n", idZ);
                sem_post(smf[zas_w]);
            }

            usleep(rand() % (args[TB] + 1)); //jede na final zastavku

            sem_wait(smf[zas_w]);
                vypis(" BUS: arrived to final\n");
                if (zastavky[0] != args[K]){ //aby necekal, pokud neni kdo by vystupoval
                    sem_post(smf[in_bus]); // tukne do in_bus -> retez -> pak to zase uzamkne
                    sem_wait(smf[bus_wait]); //ceka na lyzare, at vystoupi
                    sem_wait(smf[in_bus]);
                }
            sem_post(smf[zas_w]);
            vypis(" BUS: leaving final \n");
        }while (*L_counter != 0); //zkontrolovat zda jsou vsechni lyzari mrtvi
        vypis(" BUS: finish\n");
        exit(0);
    } else {
        int idL = 1;
        while (idL <= args[L]){
            int pid_l = fork();
            if (pid_l == 0) { //lyžař
                srand(getpid());
                int idZ = rand()%(args[Z])+1; // prirazeni 0-10 zastavek
                vypis_L(idL," started\n");
                usleep(rand()%(args[TL]+1)); // snídaně 0-TL mikrosekund

                prijit_na_zastavku(idL,idZ); //ceka na o, az bus nebude na zastavce

                //cekani na autobus
                while(1){
                    sem_wait(smf[waiting]);

                    sem_wait(smf[kapacita]);
                    if(*actual == idZ && zastavky[0] > 0){
                        zastavky[0] -= 1;
                        sem_post(smf[kapacita]);
                        vypis_L(idL," boarding\n");
                        sem_post(smf[bus_zastavka]);
                        break;
                    }
                    sem_post(smf[kapacita]);
#ifdef DEBUG
                    vypis_L(idL," failed\n");
#endif
                    sem_post(smf[bus_zastavka]); //uvolnuje autobus
                    sem_wait(smf[failed]); //cekani na odjezd
                    sem_post(smf[bus_zastavka]);
                }

                sem_wait(smf[in_bus]); //ceka nez dojede autobus

                    vypis_L(idL," going to ski\n");
                    umrit();

                    zastavky[0]++;

                    if (zastavky[0] == args[K]){ //if bus empty, //TODO co se tu kurva děje ?
                        sem_post(smf[bus_wait]);
                    }
                sem_post(smf[in_bus]); // spusti dalsi
                exit(0);
            }else if(pid_l == -1){
                fprintf(stderr,"Error: fork ");
                cleanup();
                kill(0, SIGKILL); //zabit vsechny ostatni
                exit(1);
            }else{ //PARENT
                idL++;
            }
        }
        while (wait(NULL) > 0); //wait for skibus and L
        cleanup();
        return 0;
    }
}