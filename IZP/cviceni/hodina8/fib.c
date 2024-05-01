/**
 * Kostra programu pro vypocet Fibonacciho cisel
 * 2020, Ales Smrcka
 */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ull;

/**
 * Fib(n) =
 *  0                   n=0
 *  1                   n=1
 *  Fib(n-1)+Fib(n-2)   n>1
 *
 * 1. Naprogramujte funkci rekurzivne.
 * 2. Zjistete, kolikrat byla funkce volana.
 * 3. Akcelerujte vypocet pomoci statickeho nebo globalniho pole vysledku.
 */

ull fib(ull n)
{
    //printf("%llu\n", n);
    if(n ==0)
        return 0;
    else if(n ==1)
        return 1;
    else
        return fib(n-1)+ fib(n-2);
}

ull pole[1000] = {0};

ull fib2(ull n)
{
    if(n ==0)
        return 0;
    else if(n ==1)
        return 1;
    else{
        ull one = pole[n-1]?fib2(n-1):pole[n-1];
        ull two = pole[n-2]?fib2(n-2):pole[n-2];

        pole[n] = one+two;

        return pole[n];
    }
}

int main(int argc, char **argv)
{
    ull n;
    if (argc < 2)
        return 1;
    n = strtoul(argv[1], NULL, 10);

    printf("fib2(%llu) = %llu\n", n, fib2(n));
    printf("fib(%llu) = %llu\n", n, fib(n));
    //printf("nf = %llu\n", nf);
    return 0;
}
