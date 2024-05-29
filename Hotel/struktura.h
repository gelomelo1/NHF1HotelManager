#ifndef STRUKTURA_H_INCLUDED
#define STRUKTURA_H_INCLUDED

#include <stdbool.h>

typedef struct gomb
{
    int kezdposX, kezdposY, vegposX, vegposY;
    int nevkezdposX, nevkezdposY, nevvegposX, nevvegposY;
    char nev[41];
    bool aktiv;
    bool kivalasztva;
    int r, g, b;
}gomb;

typedef struct alakzat
{
    int kezdposX, kezdposY, vegposX, vegposY;
    int r, g, b;
}alakzat;

typedef struct szo
{
    int kezdposX, kezdposY, hossz, szelesseg;
    char nev[41];
    int r, g, b;
}szo;

typedef struct ar
{
    int arak[6];
}ar;

typedef struct adatoklista
{
   int foglalasidatum;
   int tmeret;
   struct  szemelyadatok *szemelyek;
   struct  adatoklista *kov;
}adatoklista;

typedef struct szemelyadatok
{
    char nev[31];
    char tel[31];
    char email[31];
    char szobaszam[4];
    int startdatum;
    int stopdatum;
    int ar;
}szemelyadatok;

typedef struct ListazandoFoglalasok
{
    adatoklista *lista[25];
    int foglalasindex[25];
}ListazandoFoglalasok;

typedef struct FelhasznaloAdatok
{
    char felhasznalonev[21];
    char jelszo[21];
}FelhasznaloAdatok;

typedef struct hotelszobak
{
    int ferohely;
    int klima;
    int terasz;
}hotelszobak;

#endif // STRUKTURA_H_INCLUDED
