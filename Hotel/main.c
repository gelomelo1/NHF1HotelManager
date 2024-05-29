#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>

#include "struktura.h"
#include "megjelenites.h"
#include "fajladatkezeles.h"
#include "eventek.h"
#include "debugmalloc.h"

int main(int argc, char *argv[]) {
bool loginaktivoldal = true; //jeloli, hogy melyik az aktiv oldal
bool programfut = true; //ha kilep a program a fo loopbol, s ez a valtozo false-ra van allitva nem inditja el a masik oldal inicializalasat, hanem kilep
SDL_Renderer *renderer;
SDL_Window *window;
gomb gombok[41]; //Tombok a gombokhoz, szavakhoz, alakzatokhoz, ez nem mindig van teljesen feltoltve
szo szavak[25];
alakzat alakzatok[9];
adatoklista *lista = NULL; //lancolt lista
ar hotelar; //a hotelarak
int gombmeret, alakzatmeret, szomeret;

AblakLetrehozas(&renderer, &window);
while(programfut)
{
 if(loginaktivoldal)
{
     if(lista != NULL)
     lista = TeljesFelszabadit(lista);
     LoginLap(renderer, gombok, alakzatok, szavak); //Bejelentkezes oldal kirajzolasa, szavak, gombok, alakzatok hozzarendelese a tombhoz, fajlbeolvasas
     gombmeret = 5; alakzatmeret = 0; szomeret = 1;
}
else
{
     FelhasznaloLap(renderer, gombok, alakzatok, szavak); //Felhasznaloi oldal letrehozasa
     gombmeret = 41; alakzatmeret = 9; szomeret = 25;
}
ProgramEvenetek(renderer, window, gombok, gombmeret, alakzatok, alakzatmeret, szavak, szomeret, loginaktivoldal, &programfut, &lista, &hotelar); //Fo loop
loginaktivoldal = !loginaktivoldal; //Ha kilep a loopbol, vagy vege a programfutasnak, vagy automatikusan a masik oldal aktivalodik
}
if(lista != NULL)
lista = TeljesFelszabadit(lista); //Lancolt lista, s a listaban levo dinamikus tomb tlejse felszabaditasa
debugmalloc_log_file("hiba.txt");
SDL_Quit();
    return 0;
}
