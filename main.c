/**
 * Predmet: IFJ / IAL
 * Projekt: Implementace prekladace imperativniho jazyka IFJ17
 * Varianta:Tym 031, varianta I
 * Soubor:  main.c
 * Datum:   6.12.2017
 * Autori:  Kozouskova Aneta	<xkozou00@stud.fit.vutbr.cz>,
 *          Sencuch Filip	    <xsencu01@stud.fit.vutbr.cz>,
 *          Nguyen QuangTrang	<xnguye11@stud.fit.vutbr.cz>,
 *          Pribyl Tomas	    <xpriby17@stud.fit.vutbr.cz>
 */

/**
 * Hlavickove soubory
 */

#include "funkcie.h"
#include "garbage_collector.h"
#include "ial.h"
#include "syntakticky_analyzator.h"
#include "clean.h"
#include <signal.h>

tFile soubor;




/**
 * Zacatek programu
 */
int main(int argc, char **argv)
{
    (void) signal(SIGINT,clean_all);
  
    
	//zpracovani a kontrola argumentu
    if (argc != 2)  
    {
        error = EINTERN;
        fprintf(stderr, "Zadal jste spatny pocet argumentu.\n");
        return;
    }

    // pokusime se otevrit soubor
    if (!(soubor = fopen(argv[1], "r")))
    {
        error = EINTERN;
        fprintf(stderr, "Soubor nelze otevrit.\n");
        return;
    }

    // inicializujeme tabulku symbolu
    TSinit();
    if (error)
    {
        perr();
        return error;
    }

    //SYNTAKTICKA ANALYZA      
    error = parser();
    if (error == ELEX)
    {
        perrdet();
        return error;
    }
    else if (error)
    {
        perr();
        return error;
    }

    // zrusime tabulku symbolu
    TSdispose();

    //zrusime tabulku triadresneho kodu


    // zavreme soubor
    fclose(soubor);
    globalFree();

    return EOK;
}
