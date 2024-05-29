#ifndef FELHASZNALOMENU_H_INCLUDED
#define FELHASZNALOMENU_H_INCLUDED

#include "struktura.h"

void ListaInicializalas(ListazandoFoglalasok *listazandofoglalasok);
bool Formatum(gomb *gombok, int index);
void Listazas(gomb *gombok, szo *szavak, int index, char *szoveg);
int Napszamlalas(int *ev, int *ho, int *nap);
int Arszamlalas(gomb *gombok, ar *hotelar, hotelszobak szoba);
int KivalasztottSzoba(gomb *gombok, alakzat *alakzatok, char *szobaszam);
void AdatTorles(gomb *gombok, SDL_Event *event, int index, int meret);
void AdatIras(gomb *gombok, SDL_Event *event, int index, int meret);
void Lezaras(bool *lapcsere);
void Mentes(adatoklista *lista);
void FoglalasGomb(gomb *gombok, alakzat *alakzatok, szo *szavak, adatoklista **lista, ListazandoFoglalasok *listazandofoglalasok);
void TorlesGomb(gomb *gombok, alakzat *alakzatok, szo *szavak, adatoklista **lista, ListazandoFoglalasok *listazandofoglalasok);
void FoglalasGombMegjelenites(gomb *gombok);
void UjFoglalasAdatMegjelenites(gomb *gombok, szo *szavak, ar *hotelar, hotelszobak *szobak, int index);
void FoglalasiAdatGomb();
void FoglalasAdatMegjelenites(gomb *gombok, szo *szavak, int index, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista);
void FoglaltSzobak(gomb *gombok, int *datumertek, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista);
void HotelKijelolesVege(alakzat *alakzatok, gomb *gombok, szo *szavak);
void HotelKijeloles(gomb *gombok, alakzat *alakzatok, int index);
bool DatumFormatumEllenorzes(gomb *gombok, int *datumertek);
void DatumGomb(gomb *gombok, int index);
void RadioGombok(int index, gomb *gombok, alakzat *alakzatok, szo *szavak, hotelszobak *szobak);
bool szuro(hotelszobak *szoba, int ferohelyszuro, int klimaszuro, int teraszuro, int szuroszam);
#endif // FELHASZNALOMENU_H_INCLUDED
