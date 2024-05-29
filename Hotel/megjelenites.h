#ifndef MEGJELENITES_H_INCLUDED
#define MEGJELENITES_H_INCLUDED

#include <stdbool.h>
#include "struktura.h"

void AblakLetrehozas(SDL_Renderer **renderer, SDL_Window **window);
void GombhozRendeles(gomb *gombok, int meret, int koordinatak[][4], char nevek[][31], int szinek[][3], bool *aktivak, bool *kivalasztottak);
void SzohozRendeles(szo *szavak, int meret, int koordinatak[][4], char nevek[][31], int szinek[][3]);
void AlakzathozRendeles(alakzat *alakzatok, int meret, int koordinatak[][4], int szinek[][3]);
void LogOldal(SDL_Renderer *renderer, char *letezik, gomb *gombok, szo *szavak);
void FelhasznaloOldal(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak);
void SzoMegjelenites(SDL_Renderer *renderer, szo *szavak, int szomeret, gomb *gombszo);
void TeglalapMegjelenites(SDL_Renderer *renderer, alakzat *alakzatok, int alakzatmeret, gomb *gombhatter);
void Gombmegjelenites(SDL_Renderer *renderer ,gomb *gombok, int meret);
void GombTiltasFeloldas(gomb *gombok, int meret, int kezdoertek, bool valaszt);
void GombMegjelenitesTiltasFeloldas(gomb *gombok, int meret, int kezdoertek, bool valaszt);
void FoglaltsagStatuszMegjelenites(gomb *gombok, ListazandoFoglalasok *listazandofoglalasok);
void Kepernyotorles(SDL_Renderer *renderer);
void Kepernyofrissites(SDL_Renderer *renderer, gomb *gombok, int gombmeret, alakzat *alakzatok, int alakzatmeret, szo *szavak, int szomeret);

#endif // MEGJELENITES_H_INCLUDED
