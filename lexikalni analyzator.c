typedef enum     //zapiseme vsechny stavy
{
    START,
    KLIC_SLOVO,
    REZ_SLOVO,
    CHYBA,
    IDENTIFIK,
    INTEGER,
    DOUBLE_KONTR,
    DOUBLE,
    EXP_KONTR,
    EXP_KONTR02,
    EXP,
    STREDNIK,
    PRIRAZENI,
    LEVA_ZAVORKA,
    PRAVA_ZAVORKA,
    CARKA,
    PLUS,
    MINUS,
    KRAT,
    DELENO_DESET,
    DELENO_CELY,
    VETSI,
    MENSI,
    JE_ROVNO,
    VETSI_ROVNO,
    MENSI_ROVNO,
    NENI_ROVNO,
    ENDOFFILE,
    EOL,
    RADK_KOMENT,
    BLOK_KOMENT,
    BLOK_KOMENT02,
    RETEZEC,
    RETEZEC01,
} tStav;



int sloupec, radek, error;  //deklarace promenych



typedef struct       //vytvoreni struktury kam ukladame vysledky
{
    tStav stav;
    char *data;
    int radek;
    int sloupec;
} tToken;


extern tToken token;



#include <stdio.h>    //knihovny
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define POCET_KLUCOVYCH_SLOV 22
#define POCET_REZERVOVANYCH_SLOV 13
#define POKRACUJ 0
#define STOP 1

static void rozsirToken(int znak, int *i);   //prototypy funkci
static void vratZnak(int znak);
static tStav prejdiRezervovaneSlova(char *slovo);
tToken getToken(void);

char *klucoveSlova[POCET_KLUCOVYCH_SLOV] =    //pole klicovych slov
{
    "as\0, asc\0, declare\0, dim\0, do\0, double\0, else\0, end\0, chr\0",
    "function\0, if\0, input\0, integer\0, length\0, loop\0, print\0, return\0",
    "scope\0, string\0, subStr\0, then\0, while\0"
};

char *rezervovaneSlova[POCET_REZERVOVANYCH_SLOV] =       //pole rezervovanych slov
{
    "and\0, boolean\0, continue\0, elseif\0, exit\0, false\0, for\0, next\0",
    "not\0, or\0, shared\0, static\0, true\0"
};

static void rozsirToken(int znak, int *i)   //funkce nacte do dat znak tokenu
{

    if ((token.data = (char *) advRealloc(token.data, (*i) + 2)))
    {
        token.data[(*i) + 1] = '\0';
        token.data[(*i)] = znak;
        (*i)++;
    }
    else
        error = 2;
}


static void vratZnak(int znak)    //funkce vrati nacteny znak
{

    if (!isspace(znak))
        ungetc(znak, soubor);


    if (isprint(znak))
        sloupec--;
}

static tStav prejdiRezervovaneSlova(char *slovo)    //funkce zjisti zda se jedna o identifikator, klicove nebo rezervovane slovo
{
    int j;


    for (j = 0; j < POCET_KLUCOVYCH_SLOV; j++)
        if (!(strcmp(slovo, klucoveSlova[j])))
            return KLIC_SLOVO;


    for (j = 0; j < POCET_REZERVOVANYCH_SLOV; j++)
        if (!(strcmp(slovo, rezervovaneSlova[j])))
            return REZ_SLOVO;

    return IDENTIFIK;
}

tToken getToken()     //funkce prochazi souborem, nacita token a urcuje jeho typ
{

tStav stav = START;   //inicializujeme promene
int pokracuj = 0;
int i = 0;
int c, znak;

token.stav = START;
token.data = NULL;
token.radek = radek;
token.sloupec = sloupec;



while (pokracuj == 0)   //cyklus bude nacitat znaky dokud ho nezastavime
{
    znak = getc(soubor);  //nacteme znak

    switch (stav)    //automat, ktery urcuje stav a kdy se ma cyklus zastavit
    {
        case START:
        {
            if ((isalpha(znak)) || (znak == '_'))  stav = IDENTIFIK;   //stavy, kdy znak zapiseme
            else if (isdigit(znak))                stav = INTEGER;
            else if (znak == '=')                  stav = PRIRAZENI;
            else if (znak == ';')                  stav = STREDNIK;
            else if (znak == '(')                  stav = LEVA_ZAVORKA;
            else if (znak == ')')                  stav = PRAVA_ZAVORKA;
            else if (znak == ',')                  stav = CARKA;
            else if (znak == '+')                  stav = PLUS;
            else if (znak == '-')                  stav = MINUS;
            else if (znak == '*')                  stav = KRAT;
            else if (znak == '/')                  stav = DELENO_DESET;
            else if (znak == 92)     /*   \  */    stav = DELENO_CELY;
            else if (znak == '>')                  stav = VETSI;
            else if (znak == '<')                  stav = MENSI;
            else if (znak == EOF)                  stav = ENDOFFILE;
            else if (znak == '\n')                 stav = EOL;       //stavy, kdy znak nezapiseme
            else if (znak == 39)      //  '
            {
                stav = RADK_KOMENT;
                break;
            }
            else if (znak == '!')
            {
                stav = RETEZEC01;
                break;
            }
            else if (isspace(znak))    //prazdna mista se ignoruji
            {
                stav = START;
                break;
            }
            else                     //cokoliv jineho znamena chybu
            {
                stav = CHYBA;
                break;
            }

            rozsirToken(znak, &i);    //zapsani znaku
            break;

        }
        case IDENTIFIK:         //zkoumame zda dalsi nacteny znak je cislo, pismeno nebo podtrzitko 
        {

            if ((isalpha(znak) || isdigit(znak) || (znak == '_')))   //pokud ano cyklus se opakuje
            {
                stav = IDENTIFIK;
                if (znak>64 && znak<91)  //pokud je znak velke pismeno zmensi se
                {
                    znak += ('a'-'A');
                }
                rozsirToken(znak, &i);
            }

            else
            {
                token.stav = prejdiRezervovaneSlova(token.data);  //pokud ne cyklus se ukonci a znak se vrati
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }


        case INTEGER:  
        {
            if (isdigit(znak))   //pokud nasleduje cislo, je to integer
            {
                stav = INTEGER;
                rozsirToken(znak, &i);
            }
            else if (znak == '.')   //pokud . je to double
            {
                stav = DOUBLE_KONTR;
                rozsirToken(znak, &i);
            }
            else if ((znak == 'e') || (znak == 'E'))   //pokud e nebo E je to exponencialni cislo
            {
                stav = EXP_KONTR;
                rozsirToken(znak, &i);
            }
            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case DOUBLE_KONTR:   //kontrolujeme zda za teckou je cislo
        {
            if (isdigit(znak))
            {
                stav = DOUBLE;
                rozsirToken(znak, &i);
            }
            else
            {
                token.stav = stav;   //pokud ne je to chyba
                stav = CHYBA;
                vratZnak((char) znak);
            }

            break;
        }

        case DOUBLE:
        {
            if (isdigit(znak))   
            {
                stav = DOUBLE;
                rozsirToken(znak, &i);
            }
            else if ((znak == 'e') || (znak == 'E'))  //i z doublu se muze stat exp cislo
            {
                stav = EXP_KONTR;
                rozsirToken(znak, &i);
            }
            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case EXP_KONTR:
        {
            if ((znak == '-') || (znak == '+'))   //za e nebo E muze byt + nebo -
            {
                stav = EXP_KONTR02;
                rozsirToken(znak, &i);
            }
            else if (isdigit(znak))        //pokud nejsou, tak tam musi byt cislo
            {
                stav = EXP;
                rozsirToken(znak, &i);
            }
            else
            {
                token.stav = stav;      //cokoliv jineho je chyba
                stav = CHYBA;
                vratZnak((char) znak);
            }

            break;
        }

        case EXP_KONTR02:
        {
            if (isdigit(znak))   //za + nebo - musi byt cislo 
            {
                stav = EXP;
                rozsirToken(znak, &i);
            }
            else
            {
                token.stav = stav;  //jinak chyba
                stav = CHYBA;
                vratZnak((char) znak);
            }

            break;
        }

        case EXP:            //cokoliv jineho nez cislo znamena konec cyklu
        {
            if (isdigit(znak))
            {
                stav = EXP;
                rozsirToken(znak, &i);
            }

            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case DELENO_DESET:    //pokud za znakem / nasleduje ' je to blokovy komentar
        {
            if (znak == 39)
            {
                stav = BLOK_KOMENT;
            }

            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case PRIRAZENI:  //pokud za znakem je = je to prirovnani
        {
            if (znak == '=')
            {
                stav = JE_ROVNO;
                rozsirToken(znak, &i);
            }

            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case VETSI:   //pokud za znakem je = je to vetsi nebo rovno
        {
            if (znak == '=')
            {
                stav = VETSI_ROVNO;
                rozsirToken(znak, &i);
            }

            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case MENSI:  //pokud za znakem je = je to mensi nebo rovno
        {
            if (znak == '=')
            {
                stav = MENSI_ROVNO;
                rozsirToken(znak, &i);
            }

            else if (znak == '>')   //pokud za znakem je > je to neni rovno
            {
                stav = NENI_ROVNO;
                rozsirToken(znak, &i);
            }

            else
            {
                token.stav = stav;
                pokracuj = 1;
                vratZnak((char) znak);
            }

            break;
        }

        case KLIC_SLOVO:  // zde jsou stavy kdy jenom vratime znak a cyklus ukoncime, protoze to co je za nimi nas nezajima
        case REZ_SLOVO:
        case PLUS:
        case MINUS:
        case KRAT:
        case DELENO_CELY:
        case VETSI_ROVNO:
        case MENSI_ROVNO:
        case NENI_ROVNO:
        case JE_ROVNO:
        case LEVA_ZAVORKA:
        case PRAVA_ZAVORKA:
        case CARKA:
        case ENDOFFILE:
        case EOL:
        case STREDNIK:
        {
            token.stav = stav;
            pokracuj = 1;
            vratZnak((char) znak);
            break;
        }

        case RADK_KOMENT:         
        {
            if (znak == '\n')   //dokud nenarazime na konec radku tak vse ignorujeme a automat se pote nastavi na start
            {
                stav = START;
                i = 0;
            }

            else if (znak == EOF)   //na konci komentare muze byz i EOF
            {
                stav = ENDOFFILE;
            }

            else
            {
                stav = RADK_KOMENT;
            }
            break;
        }

        case BLOK_KOMENT:    //ignorujeme vse krome '
        {
            if (znak == 39)
            {
                stav = BLOK_KOMENT02;
            }

            else
            {
                stav = BLOK_KOMENT;
            }

            break;
        }

        case BLOK_KOMENT02:
        {
            if (c == '/')    //pokud narazime na / je to konec blok. komentare
            {
                stav = START;    
                i = 0;
            }

            else if (c == 39)   //pokud na ' zkoumame zda za nim neni /
            {
                stav = BLOK_KOMENT02;
            }

            else           //pokud nan neco jineho ignorujeme to a zacneme s hledanim znovu
            {
                stav = BLOK_KOMENT;
            }

            break;
        }

        case RETEZEC01: 
        {
            if (c == '"')        //pokud za ! neni " je to chyba 
            {
                stav = RETEZEC;
            }

            else
            {
                token.stav = stav;
                stav = CHYBA;
                vratZnak((char) znak);
            }

            break;
        }

        case RETEZEC:      // nyni hledame dalsi ", vse statni zapiseme
        {
            if (c == '"')
            {
                token.stav = RETEZEC;
                pokracuj = 1;
            }

            else
            {
                stav = RETEZEC;
                rozsirToken(c, &i);
            }

            break;
        }

        case CHYBA:        //stav pro chybu
        {
            error = 1;
            pokracuj = 1;
            break;
        }



        if (error)        //pokud nastala chyba analyzator ukoncime
            break;

        if (znak == '\n')   //pocitadla radku a sloupcu
        {
            radek++;
            sloupec = 1;
        }
        else if (isprint(znak))
            sloupec++;

    }
}

return token;
}
