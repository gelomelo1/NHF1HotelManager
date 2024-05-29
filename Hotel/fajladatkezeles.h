#ifndef FAJLADATKEZELES_H_INCLUDED
#define FAJLADATKEZELES_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include "struktura.h"

ar alapertelmezettar();
adatoklista *Beszuras(adatoklista *start, int foglalasidatum, int meret);
adatoklista *TeljesFelszabadit(adatoklista *lista);
adatoklista *ReszlegesFelszabadit(adatoklista *lista, adatoklista *keresendo);
void Kereses(gomb *gombok, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista, int *datumertek);
bool FoglalasTorles(ListazandoFoglalasok *listazandofoglalsok, adatoklista **lista, int index);
void ToroltErtekBeallitas(ListazandoFoglalasok *listazandofoglalsok, int index);
bool TorlesUjraMeretezes(ListazandoFoglalasok *listazandofoglalsok, int index);
bool FoglalasFelvetel(ListazandoFoglalasok *listazandofoglalasok, adatoklista **lista, int index, char adatok[][31], int datumok[3], int ar);
void FoglalasBeiras(adatoklista *kivalasztottelem, int index, char adatok[][31], int datumok[3], int teljesar);
adatoklista *FoglalasListaLetrehozas(adatoklista *lista, int datum);
bool FoglalasUjraMeretezes(adatoklista *kivalasztottelem);
int ElemIndex(adatoklista *kivalasztottelem);
void DatumvaltasVissza(int datum, char *atvaltas);
int Datumatvaltas(char *datum, bool datumfajta);
int Sorosszeg(FILE *fajl);
void AdatInicializalas(szemelyadatok *foglalas, int meret, int kezdoertek);
void Adatkitoltes(szemelyadatok *foglalas, char *sor, int index);
bool FajlLetezik(char *fajlnev);
bool FajlbaIras(char *fajlnev, int meret, char adatok[][31]);
bool FajbolBeolvasas(char *fajlnev, int meret, char adatok[][31]);
bool FoglalasBeolvas(char *fajlnev, adatoklista **lista);
void FoglalasIras(char *fajlnev, adatoklista *lista);

#endif // FAJLADATKEZELES_H_INCLUDED
