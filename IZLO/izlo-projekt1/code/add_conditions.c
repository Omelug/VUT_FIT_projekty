#include <stddef.h>
#include "cnf.h"
#include "stdio.h"

//
// LOGIN: xjanecv00
//

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 1)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
// Pole streets ma velikost num_of_streets a obsahuje vsechny existujuci ulice
//    - pro 0 <= i < num_of_streets predstavuje streets[i] jednu existujici
//      ulici od krizovatky streets[i].crossroad_from ke krizovatce streets[i].crossroad_to
void at_least_one_valid_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets, const Street* streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    assert(streets != NULL);

    for(int i=0;i<num_of_streets;++i) {//prochazeni kroku
        Clause* clause = create_new_clause(formula);
        for (int s = 0; s < num_of_streets; ++s) { //v kazdym kroku zjistim moznosti pruchodu
            add_literal_to_clause(clause, true, i, streets[s].crossroad_from, streets[s].crossroad_to);
        }
    }
}
//test

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 2)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void at_most_one_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    for (unsigned i = 0; i < num_of_streets; ++i) {
        // pro kazdy krok z,k
        for (unsigned z = 0; z < num_of_crossroads; ++z) {
            for (unsigned k = 0; k < num_of_crossroads; ++k) {
                // prvni cesta
                for (unsigned z2 = 0; z2 < num_of_crossroads; ++z2) {
                    for (unsigned k2 = 0; k2 < num_of_crossroads; ++k2) {
                        if ( (( z*num_of_crossroads+k ) < ( z2*num_of_crossroads+k2 ))) {
                            Clause* cl = create_new_clause(formula);
                            add_literal_to_clause(cl, false, i, z, k);
                            add_literal_to_clause(cl, false, i, z2, k2);
                        }
                    }
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 3)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_connected(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);

    for (unsigned i = 0; i < num_of_streets-1; ++i) {
        // pro kazdy krok z,k
        for (unsigned z = 0; z < num_of_crossroads; ++z) {
            for (unsigned k = 0; k < num_of_crossroads; ++k) {
                // prvni cesta
                Clause* cl = create_new_clause(formula);
                add_literal_to_clause(cl, false, i, z, k);
                for (unsigned f = 0; f < num_of_crossroads; ++f) {
                    add_literal_to_clause(cl, true, i+1, k, f);
                }
            }
        }
    }
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 4)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_do_not_repeat(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets) {
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    
    for (unsigned i = 0; i < num_of_streets; ++i) {
        // pro kazdy krok i
        for (unsigned j = 0; j < num_of_streets; ++j) {
            if (i != j) {
                // pro kazdy jiny krok j
                for (unsigned z = 0; z < num_of_crossroads; ++z) {
                    for (unsigned k = 0; k < num_of_crossroads; ++k) {
                        // pro kazdu dvojici krizovatek (z, k)
                        Clause* cl = create_new_clause(formula);
                        add_literal_to_clause(cl, false, i, z, k);
                        add_literal_to_clause(cl, false, j, z, k);
                    }
                }
            }
        }
    }
}
