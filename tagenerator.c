/**
 * Predmet: IFJ / IAL
 * Projekt: Implementace prekladace imperativniho jazyka IFJ17
 * Varianta:Tym 031, varianta I
 * Soubor:  tagenerator.c
 * Autori:  Kozouskova Aneta	<xkozou00@stud.fit.vutbr.cz>,
 *          Sencuch Filip	    <xsencu01@stud.fit.vutbr.cz>,
 *          Nguyen QuangTrang	<xnguye11@stud.fit.vutbr.cz>,
 *          Pribyl Tomas	    <xpriby17@stud.fit.vutbr.cz>
 */

# include "tagenerator.h"
#include <math.h>
#include "garbage_collector.h"

/*****
* Inicializace tabulky
*****/
void ta_Init(TA_Item **ta_table)
{
    *ta_table = NULL;
}

/*****
 * Pridani do tabulky
 *****/
void ta_Insert(TA_Item **ta_table, size_t operation, tBTSUzolPtr op1, tBTSUzolPtr op2, tBTSUzolPtr result)
{
    int end;
    bool prepis = true;

    if( *ta_table == NULL )
    {
        prepis = true;
        end = 0;
    }
    else
    {
        prepis = false;
        end = (*ta_table)[0].end; //na prvni polozce je ulozen index posledni instrukce
    }
    //nez pridame to tabulky musime zjistit, zda mame dost mista, pripadne prialkoujeme
    if( (end % ALL_MORE) == 0 )
    {
        *ta_table = advRealloc( *ta_table, (ALL_MORE + end) * sizeof(TA_Item));
        if( ta_table == NULL)  // chyba alokace
        {
            error = EOST;
            return;
        }
    }

    //zapis instrukce
    (*ta_table)[end].operation = operation;
    (*ta_table)[end].op1 = op1;
    (*ta_table)[end].op2 = op2;
    (*ta_table)[end].result = result;
    (*ta_table)[end].jump = -1;

    (*ta_table)[0].end++;

    return;

}
/*****
 * Nacteni z tabulky
 ******/
void ta_Get(TA_Item **ta_table, size_t *operation, tBTSUzolPtr *op1, tBTSUzolPtr *op2, tBTSUzolPtr *result, int position)
{
    //kdybychom sahali mimo nebo byla tabulka prazdna tak nemame co cist
    if( (position > (*ta_table)[0].end) || (*ta_table == NULL) )
    {
        error = EOST;
        return;
    }

    *operation = (size_t)(*ta_table)[position].operation;
    *op1 = (tBTSUzolPtr)(*ta_table)[position].op1;
    *op2 = (tBTSUzolPtr)(*ta_table)[position].op2;
    *result = (tBTSUzolPtr)(*ta_table)[position].result;
}

/*****
 * Uklizeni tabulky
 *****/
void ta_Destroy(TA_Item **ta_table)
{

}
/*****
 * Pridani do tabulky - jumpy a labely
 ******/
void ta_InsertJump(TA_Item **ta_table, size_t operation, tBTSUzolPtr op1, int jump)
{
    int end;
    bool prepis = true;
    if( *ta_table == NULL )
    {
        end = 0;
        prepis = true;
    }
    else
    {
        prepis = false;
        end = (*ta_table)[0].end; //na prvni polozce je ulozen index posledni instrukce
    }
    //nez pridame to tabulky musime zjistit, zda mame dost mista, pripadne prialkoujeme
    if( (end % ALL_MORE) == 0 )
    {
        *ta_table = advRealloc( *ta_table, (ALL_MORE + end) * sizeof(TA_Item));
        if( ta_table == NULL)  // chyba alokace
        {
            error = EOST;
            return;
        }
    }
    //zapis instrukce
    (*ta_table)[end].operation = operation;
    if(op1 != NULL)
    {
        (*ta_table)[end].op1 = op1;
    }
    else
    {
        (*ta_table)[end].op1 = NULL;
    }
    (*ta_table)[end].op2 = NULL;
    (*ta_table)[end].result = NULL;
    (*ta_table)[end].jump = jump;

    (*ta_table)[0].end++;

    return;
}

