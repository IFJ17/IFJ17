
#pragma once

typedef enum
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


/**
 * Struktura reprezentujuca token
 */
typedef struct
{
    tStav stav;     // stav lexemu
    char *data;     // hodnota lexemu
    int radek;     // riadok lexemu
    int sloupec;     // stlpec lexemu
} tToken;

/**
 * Globalna premenna
 */
extern tToken token;

/**
 * Prototypy funkcii
 */
tToken getToken(void);  // vrati token
