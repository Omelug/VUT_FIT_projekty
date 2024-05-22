# MakeFile
#// testovano na gcc 13.2,0,  16.4.2024
#// IJC-DU2
#// Autor: Vít Janeček, VUT FIT
#// xjanecv00

CC=gcc
CFLAGS = -g -std=c11 -pedantic -Wall -Wextra -O2 -fPIC
#CPPFLAGS = -g -pedantic -Wall -Wextra -O2 # pro profiler -pg
CFLAGS += -fsanitize=address
LDFLAGS += -fsanitize=address
MODULES = htab_statistics.c htab_hash_function.c htab_init.c htab_lookup_add.c htab_for_each.c htab_clear.o htab_free.c htab_erase.c create_item.c htab_size.c htab_bucket_count.c
OBJECTS = htab_statistics.o htab_hash_function.o htab_init.o htab_lookup_add.o htab_for_each.o htab_clear.o htab_free.o htab_erase.o create_item.o htab_size.o htab_bucket_count.o

.PHONY: clean

default: all

all: tail wordcount wordcount-dynamic libhtab.a libhtab.so

# tail
run_tail: tail
	./tail -n 6 < wordcount.c
tail: tail.c
	$(CC) $(CFLAGS) tail.c -o tail

#wordcount
run_val_wc: wordcount
	valgrind -s --track-origins=yes --leak-check=full  ./wordcount < test_cislovky
run_wc: wordcount
	./wordcount < test_cislovky

#wordcount
run_val_wc_d: wordcount-dynamic
	LD_LIBRARY_PATH="."  valgrind -s --leak-check=full  ./wordcount-dynamic < test_cislovky
run_wc_d: wordcount-dynamic
	LD_LIBRARY_PATH="." ./wordcount-dynamic < test_cislovky

wordcount: libhtab.a io.o wordcount.o Makefile
	$(CC) $(CFLAGS) -o wordcount -Bstatic wordcount.o io.o libhtab.a
wordcount-dynamic: libhtab.so io.o wordcount.o Makefile
	$(CC) $(CFLAGS) -o $@ wordcount.o io.o -L. -lhtab

libhtab.a: libhtab.o htab.h $(OBJECTS)
	ar -rcs libhtab.a libhtab.o $(OBJECTS)
libhtab.so: libhtab.o  htab.h $(OBJECTS)
	$(CC) $(CFLAGS) -fPIC  -shared libhtab.o -o libhtab.so $(OBJECTS)

io.o: io.c io.h
wordcount.o: wordcount.c
libhtab.o: libhtab.c htab.h htab_inc.h

htab_statistic.o: htab_statistics.c htab.h htab_inc.h
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@
htab_hash_function.o:htab_hash_function.c htab.h htab_inc.h
htab_init.o:htab_init.c htab.h htab_inc.h
htab_lookup_add.o:htab_lookup_add.c htab.h htab_inc.h
htab_for_each.o:htab_for_each.c htab.h htab_inc.h
htab_free.o:htab_free.c htab.h htab_inc.h
htab_erase.o:htab_erase.c htab.h htab_inc.h
create_item.o:create_item.c htab.h htab_inc.h
htab_size.o:htab_size.c htab.h htab_inc.h
htab_bucket_count.o:htab_bucket_count.c htab.h htab_inc.h


pack:
	zip xjanecv00.zip *.c *.h Makefile
clean:
	 rm -f *.o *.a *.so tail wordcount