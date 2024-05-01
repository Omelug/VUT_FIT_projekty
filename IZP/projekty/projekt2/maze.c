#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

enum BORDER{LEFT=1,RIGHT=2,CENTER=4};

bool isBlank(char c){return (0 <= c) && (c <= 32);}
bool isNumber(char c) {return ('0' <= c) && (c <= '9');}
int toNumber(char c) {return c - '0';}
bool isValidCell(int c) {return ('0' <= c) && (c <= '7');}

///@return next not blank char in FILE
int notBlank(FILE * file){
    int b;
    while (1){
        b = fgetc(file);
        if(!isBlank((char)b)){
            return b;
        }
    }
}

/* /// @return true if strings are same
bool sameString(const char string[], const char string2[]){ //nahrazeno funkci z string
    if(string== NULL){
        if (string2 == NULL){
            return true;
        }
        return false;
    }
    for(int i= 0;1;i++){
        if (string[i] == '\0' || string2[i] == '\0'){
            if(string[i] == '\0' && string2[i] == '\0'){
                return true;
            }
            return false;
        }
        if (string[i] != string2[i]){
            return false;
        }
    }
}*/
/*///@param count number of input strings
///@return pointer to new connected string
char* stringConnect(int count, ...){ //repair before use, cause mem leaks
    va_list strings;
    va_start(strings, count);
    int len = 0;
    char* help[count];

    for(int i = 0; i < count; i++){
        help[i] = va_arg(strings, char*);
        len += (int)strlen(help[i]);
    }

    char *result = malloc(len*sizeof(char) +1); //\0
    if (result == NULL){
        va_end(strings);
        fprintf(stderr, "Allocation of result string in stringConnect failed");
        return NULL;
    }
    int a=0;
    for (int i = 0; i < count; i++) {
        for (int j = 0;j < (int)strlen(help[i]);j++){
            result[a+j] = help[i][j];
        }
        a += (int)strlen(help[i]);
    }

    va_end(strings);

    return result;
}*/

bool xor(bool a, bool b) {
    return (a+b)%2;
}
///can overflow to negative, skip blank chars
///@return int from file or -1 if is not number
int getPlusInt(FILE *file) {
    int r = 0;
    int c;
    bool throwNewBlank = true;
    while ((c = fgetc(file)) != EOF) {
        if (isBlank((char)c)) {
            if(throwNewBlank){
                continue;
            }
            return r;
        }
        throwNewBlank = false;
        if(r == -1){
            r = 0;
        }
        if(isNumber((char)c)){
            r = r*10 + toNumber((char)c);
        }else{
            return -1;
        }
    }
    return r;
}

int getPlusIntStr(char* string){
    int r = 0;
    for(size_t i = 0;i < strlen(string);i++){
        if(isNumber((char) string[i])){
            r = r*10 + toNumber((char)string[i]);
        }else{
            return -1;
        }
    }
    return r;
}

void print_log(char type, char* format, ...){

    va_list strings; //macros for elipsis
    va_start(strings, format);

    if (type == 'e'){
        fprintf(stderr,"\033[31m");
        vfprintf(stderr, format, strings);
        fprintf(stderr, "\033[0m\n");
        va_end(strings);
        return;
    }
    switch (type) {
        case 'w':
            printf("\033[33m");
            break;
        case 'd':
            printf("\033[32m");
            break;
        default:
            printf("\033[36m");
    }
    vprintf(format, strings);
    va_end(strings);
    printf("\033[0m\n");
}
bool getMapBody(Map* map, FILE* file){
    map->cells = malloc(map->rows*map->cols * sizeof(unsigned char));
    if(map->cells == NULL){
        print_log('e',"Error during map data allocation");
        return false;
    }

    for(int r=0; r<map->rows; r++) {
        for(int c=0; c < map->cols; c++) {
            int nb = notBlank(file);
            if(isValidCell(nb)){
                    map->cells[map->cols*r+c] = nb;
                    continue;
            }
            return false; //includes EOF
        }
    }
    int c = notBlank(file);
    if(c == EOF){
        return true;
    }
    return false;
}
bool isborder(Map *map, int r, int c, int border){
    return ((map->cells[map->cols*r+c]-'0') & border) > 0;
}
bool up(int r, int c){return (c+r)%2;}
bool isOut(Map* map, int r, int c){
    return (c == -1 || r == -1 || c == map->cols || r == map->rows);
}
int cycle(int border,int r, int c, int rule){
    if(xor(up(r,c),rule == 'l')){
        //CENTER -> RIGHT -> LEFT -> CENTER
        switch (border) {
            case CENTER:
                return RIGHT;
            case RIGHT:
                return LEFT;
            case LEFT:
                return CENTER;
            default:
                return -1;
        }
    }else{
        //CENTER -> LEFT -> RIGHT -> CENTER
        switch (border) {
            case CENTER:
                return LEFT;
            case RIGHT:
                return CENTER;
            case LEFT:
                return RIGHT;
            default:
                return -1;
        }
    }
}

int start_border(Map *map, int r, int c, int leftright){

    int border = -1;

    //from CENTER border (up or down)
    if(!isborder(map,r,c, CENTER)){
        if (r == map->rows-1 && up(r,c)){
            border = RIGHT;
        }
        if (r == 0 && !up(r,c)){
            border = LEFT;
        }
    }
    if(!isborder(map,r,c, RIGHT) && c == map->cols-1){ //right
        if(up(r,c)){
            border = LEFT;
        }else{
            border = CENTER;
        }
    }
    if(!isborder(map,r,c, LEFT) && c == 0){//left
        if(up(r,c)){
            border = CENTER;
        }else{
            border = RIGHT;
        }
    }

    if(leftright == 'l'){
        border = cycle(border,r,c, 'r');
    }
    return border;
}
bool downBorderControl(Map* map, int r,int c){
    //printf("center (%d;%d)---(%d;%d) %d=%d\n", r1,c1,r2,c2,isborder(map,r1,c1,CENTER),isborder(map,r2,c2,CENTER));
    return isborder(map,r,c,CENTER) == isborder(map,r+1,c,CENTER);
}
bool rightBorderControl(Map* map, int r,int c){
   //printf("sameSideBorder (%d;%d)---(%d;%d) %d=%d\n", r1,c1,r2,c2,isborder(map,r1,c1,RIGHT),isborder(map,r2,c2,LEFT));
    return (isborder(map,r,c,RIGHT) == isborder(map,r,c+1,LEFT));
}

void printLineOne(Map* map, int r, bool up_line){
    for (int j = 0; j < map->cols; j++) {
        if(up_line == up(r, j)){
            isborder(map,r,j,CENTER) ? printf("*----"):printf("*    ");
        }
    }
    printf("*\n");
}
void printLineTwo(Map* map, int r){
    int up_control = up(r,0);
    for (int j = 0; j < map->cols; j++) {
        if(up_control == up(r,j)){
            if(!up(r,j)){
                isborder(map,r,j,LEFT) ? printf(" \\ "):printf("   ");
                isborder(map,r,j,RIGHT) ? printf(" /"):printf("  ");
            }else{
                isborder(map,r,j,LEFT) ? printf(" / "):printf("   ");
                isborder(map,r,j,RIGHT) ? printf(" \\"):printf("  ");
            }
        }else if(j == map->cols-1){
            if(!up(r,j)){
                isborder(map,r,map->cols-1,RIGHT) ? printf(" /"):printf("  ");
            }else{
                isborder(map,r,map->cols-1,RIGHT) ? printf(" \\"):printf("  ");
            }
        }
    }
    printf("\n");
}
void printLineThree(Map* map, int r){
    printf("  ");
    for (int j = 0; j < map->cols; ++j) {
        if(!up(r,j)){
            isborder(map,r,j,LEFT) ? printf("\\"):printf(" ");
            isborder(map,r,j,RIGHT) ? printf("/"):printf(" ");
        }else{
            isborder(map,r,j,CENTER) ? printf("___"):printf("   ");
        }
        if(j == map->cols-1 && up(r,j)){
            isborder(map,r,j,RIGHT) ? printf("\\"):printf(" ");
        }
    }
    printf("\n");
}
void printLineFour(Map* map, int r){
    printf("  ");
    for (int j = 0; j < map->cols; ++j) {
        if(up(r,j)){
            isborder(map,r,j,LEFT) ? printf("/"):printf(" ");
            isborder(map,r,j,RIGHT) ? printf("\\"):printf(" ");
            printf("   ");
        }
        if(j == map->cols-1 && !up(r,j)){
            isborder(map,r,j,RIGHT) ? printf("/"):printf(" ");
        }
    }
    printf("\n");
}
void printMap(Map *map){
    printf("%d %d\n", map->rows, map->cols);

    if (map->cells == NULL){
        return;
    }

    for (int i = 0; i < map->rows*map->cols; ++i) {
        if (i%map->cols == 0) printf("\n");
        printf("%c ",map->cells[i]);

    }
    printf("\n\n");

    for (int i = 0; i < map->rows; ++i) {

        printLineOne(map,i, false);
        printLineTwo(map,i);
        printLineThree(map,i);
        i++;

        printLineFour(map,i);
        printLineTwo(map,i);
        if(i == map->rows-1){
            printLineOne(map,i, true);
        }

    }
}
void freeMap(Map *map){
    if(map->cells != NULL){
        free(map->cells);
    }
}
bool loadMap(Map *map, char* path){
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        print_log('e',"Error opening file %s", path);
        return false;
    }
    if ((map->rows = getPlusInt(file)) < 0){return false;}
    if ((map->cols = getPlusInt(file)) < 0){return false;}

    if (!getMapBody(map, file)){return false;}

    fclose(file);

    for (int r = 0; r < map->rows; r++) {
        for (int c = 0; c < map->cols; c++){

            //control right border integrity
            if ( c != (map->cols-1) && !rightBorderControl(map,r,c)){
                //printf("right %d (%d;%d) (%c;%c)\n",map->cols, r,c,map->cells[map->cols*r+c],map->cells[map->cols*r+c+1]);
                return false;
            }

            //control down border integrity
            if (r != (map->rows-1) && up(r,c) && !downBorderControl(map,r,c)){
                //printf("center (%d;%d) (%c;%c)\n", r,c,map->cells[map->cols*r+c],map->cells[map->cols*(r+1)+c]);
                return false;
            }
        }
    }

    return true;
}

/// @param rule only 'r'/'l'
/// @return true if path was found
bool findPath(char rule, Map* map, int startR, int startC){

    int posR = startR-1;
    int posC = startC-1;

    if(!(0 <= posR && posR <= map->rows && 0 <= posC && posC <= map->cols)){
        print_log('e', "Cell (%d;%d) is not in the map size ", startR, startC);
        return false;
    }

    int border = start_border(map, posR,posC,rule);
    if(border == -1){
        print_log('e', "There is not valid entrance on (%d,%d)", startR,startC);
        return false;
    }

    printf("%d,%d\n",posR+1, posC+1);

    while(true){
        if(isborder(map,posR, posC,border)){
            border = cycle(border, posR, posC,rule);
            //printf("-->%s",BORDERS[border]);
            continue;
        }

        //warning - do not change order of pos and border
        switch (border) {
            case CENTER:
                if(xor(up(posR,posC), rule == 'l')){
                    border = LEFT;
                }else{
                    border = RIGHT;
                }
                up(posR,posC) ? posR++:posR--;
                break;
            case RIGHT:
                if(xor(up(posR,posC), rule == 'l')){
                    border = RIGHT;
                }else{
                    border = CENTER;
                }
                posC++;
                break;
            case LEFT:
                if(xor(up(posR,posC), rule == 'l')){
                    border = CENTER;
                }else{
                    border = LEFT;
                }
                posC--;
                break;
            default:
                print_log('e', "Invalid border during path finding");
                return false;
        }

        if(isOut(map,posR,posC)){
            //printf("\n%d,%d is out\n",posC, posR);
            if(posC == startC+1 && posR == startR+1){
                return false;
            }
            return true;
        }
        //printf("\n %d,%d %s",posR+1, posC+1,BORDERS[border]);
        printf("%d,%d\n",posR+1, posC+1);

    }
}

typedef struct {
    char* name;
    char* opt_types;
    char* description;
} setting;

void print_setting(const setting* setting){
    printf("\033[0;34m --%s \033[0;36m %s\n",setting->name, setting->description);
}

bool validArgument(const setting* setting, char ** argv,int const *i, const size_t argc){{

        if (!(argv[*i][0] == '-' && argv[*i][1] == '-')){
            return false;
        }

        if (argv[*i][2] != '\0' && strcmp( &argv[*i][2], setting->name) == 0){
            size_t opt_c = strlen(setting->opt_types);
            if (opt_c < (argc-*i)){
                for (size_t j = 1; j <= opt_c; j++){
                    if (setting->opt_types[j] == 'i'){
                        for (size_t k = 0; k < strlen(argv[j+*i]); ++k) {
                            if (!isNumber(argv[j+*i][k])){
                                print_log('e', "%s is not integer",argv[j+*i]);
                                return false;
                            }
                        }
                    }
                    if (setting->opt_types[j] == 'f'){
                        for (size_t k = 0; k < strlen(argv[j+*i]); ++k) {
                            if (argv[j+*i][k] == '.'){
                                print_log('e', "Are you sure that %s is file? Has no '.' in name", argv[j+*i]);
                            }
                        }
                    }
                }
                return true;
            }
            print_log('e', "Argument %s should have %ld options", argv[*i], opt_c);
            print_setting(setting);
        }
        return false;
    }
}
setting* con_setting(char * name, char types[],char*  description){
    setting *new = malloc(sizeof(setting));
    if (new == NULL){
        print_log('e', "Error during settings allocation");
        return NULL;
    }
    new->name = name;
    new->description = description;
    for (size_t i = 0; i < strlen(types);i++){
        if(!(types[i] == 'i' || types[i] == 's'|| types[i] == 'f')){
            print_log('e', "Not valid type %c in argument %s", types[i], name);
            return NULL;
        }
    }
    new->opt_types = types;
    return new;
}

void des_setting(setting* setting){
    free(setting);
}

int main(int argc, char** argv){
    Map map = {0,0,NULL};

    setting* help = con_setting("help","", "show this list");
    setting* test = con_setting("test","f","<file.map>; test diif is maze valid (Valid/Invalid)");
    setting* rpath = con_setting("rpath","iif","<R> <C> <file.map>; show right rule path");
    setting* lpath = con_setting("lpath","iif","<R> <C> <file.map>; show left rule path");

    int exit_code = 0;
    if (help==NULL || test==NULL || rpath==NULL || lpath==NULL){
        exit_code = EXIT_FAILURE;
    }

    for (int i = 0; i < argc;i++) {

        if(validArgument(help, argv, &i, argc) || argc==1) {
            printf("\033[0;34m\n");
            printf("H     H  EEEEEEEEE  L        PPPPP    !!\n");
            printf("H     H  E          L        P    P   !!\n");
            printf("HHHHHHH  EEEEEEEE   L        PpppP    !!\n");
            printf("H     H  E          L        P          \n");
            printf("H     H  EEEEEEEEE  LLLLLLL  P        !!\n");
            printf("\033[0m");

            printf("----------------------------------------------------------------\n");
            print_setting(help);
            print_setting(test);
            print_setting(rpath);
            print_setting(lpath);
            printf("\033[0m"); // \033 is same as escape char like \x1b ascii 27 ESC, m is the escape sequence
            printf("----------------------------------------------------------------\n");
        }else

        if(strcmp("--print", argv[i]) == 0) {

            if(loadMap(&map,argv[i+1])){
                printMap(&map);
                i++;
            }else{
                print_log('e', "Print function cant print invalid map\n");
                exit_code = EXIT_FAILURE;
            }
        }else

        if(validArgument(test, argv, &i, argc)) {

            if(loadMap(&map,argv[i+1])){
                printf("Valid\n");
            }else{
                printf("Invalid\n");
            }
            i += (int)strlen(test->opt_types);
        }else

        if(validArgument(lpath, argv, &i, argc)|| validArgument(rpath, argv, &i, argc)){
            int r,c;
            if((r = getPlusIntStr(argv[i+1])) != -1 && (c = getPlusIntStr(argv[i+2])) != -1){
                if(loadMap(&map,argv[i+3])){
                    findPath(argv[i][2],&map, r, c);
                    i += (int)strlen(lpath->opt_types);
                }else{
                    print_log('e',"Not trying find path in invalid Map");
                    exit_code = EXIT_FAILURE;
                }
            }else{
                print_log('e',"Second and third option should be integers");
                exit_code = EXIT_FAILURE;
            }
        }else{

            if ((strlen(argv[i]) >= 2 )&& (argv[i][0] == '-' && argv[i][1] == '-')){
                print_log('e',"Useless setting %s",argv[i]);
            }else{
                print_log('e',"Useless option %s",argv[i]);
            }

            exit_code = EXIT_FAILURE;
        }
    }

    freeMap(&map);

    des_setting(help);
    des_setting(test);
    des_setting(rpath);
    des_setting(lpath);


    return exit_code;
}