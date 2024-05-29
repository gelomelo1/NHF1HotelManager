#ifndef EVENTEK_H_INCLUDED
#define EVENTEK_H_INCLUDED

#include <stdbool.h>
#include "struktura.h"

void ProgramEvenetek(SDL_Renderer *renderer, SDL_Window *window, gomb *gombok, int gombmeret, alakzat *alakzatok, int alakzatmeret, szo *szavak, int szomeret, bool loginaktivoldal, bool *programfut, adatoklista **lista, ar *hotelar);
bool FajlbeolvasasEsellenorzes(char *autentikacio, char *felhasznalonev, char *jelszo, adatoklista **lista, ar *hotelar);
void LoginLap(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak);
void FelhasznaloLap(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak);
void Hoteladatok(hotelszobak *hoteladat);
bool Fedes(gomb *cel, int x, int y);
int Gombindex(gomb *gombok, int meret, SDL_Event *event);
void LoginOldalGombfunkciok(int index, gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event, int billentyuevent, bool *lapcsere, adatoklista **lista, ar *hotelar);
void FelhasznaloOldalGombfunkciok(int index, gomb *gombok, alakzat *alakzatok, szo *szavak, hotelszobak *szobak, SDL_Event *event, ListazandoFoglalasok *listazandofoglalasok, int billentyuevent, adatoklista **lista, ar *hotelar);

#endif // EVENTEK_H_INCLUDED
