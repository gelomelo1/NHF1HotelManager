#ifndef BEJELENTKEZESMENU_H_INCLUDED
#define BEJELENTKEZESMENU_H_INCLUDED

#include "struktura.h"

void BelepesRegisztracioGomb(gomb *gombok, FelhasznaloAdatok *felhasznalo);
void FelhasznaloGomb(gomb *gombok);
void FelhasznaloIras(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event);
void FelhasznaloTorles(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event);
void JelszoGomb(gomb *gombok);
void JelszoIras(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event);
void JelszoTorles(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event);
void VisszaGomb(gomb *gombok);
void OkGomb(gomb *gombok, FelhasznaloAdatok *felhasznalo, bool *lapcsere, adatoklista **lista, ar *hotelar);

#endif // BEJELENTKEZESMENU_H_INCLUDED
