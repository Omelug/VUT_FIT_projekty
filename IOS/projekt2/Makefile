CC=gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic -ltr -pthread

default: proj2

run: proj2
	./proj2 8 4 10 4 5

test: proj2
	./proj2 8 4 10 4 5

proj2:FORCE
	gcc -std=gnu99 -Wall -Wextra -Werror -pthread proj2.c -o proj2
test: test.c.test
demo: demo.c

run_merlin: proj2 # zkusit
	sshpass -p kupemve8jA scp -r ../projekt2 xjanecv00@merlin.fit.vutbr.cz:~

pack:
	zip proj2.zip *.c *.h Makefile
clean:
	 rm -f *.o *.a *.so

FORCE: ;