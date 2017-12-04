#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Typ pre ukazatel na subor
 */
typedef FILE *tFile;

/**
 * Chybove kody.
 */
typedef enum Error
{
    EOK,        // 0 - vsetko ok
    ELEX,       // 1 - chyby v ramci lexikalnej analyzy
    ESYN,       // 2 - chyby v ramci syntaktickej analyzy
    ESEM,       // 3 - chyby v ramci semantickej analyzy
    ETYP,       // 4 - sémantická chyba typové kompatibility
    EOST,       // 6 - ostatní sémantické chyby.
    EAL,        // 99
} tError;

/**
 * Globalne premenne
 */
tFile soubor;
tError error;
int radek;
int sloupec;


void perr(void);
void perrdet(void);
