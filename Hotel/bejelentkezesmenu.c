#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "megjelenites.h"
#include "fajladatkezeles.h"
#include "eventek.h"
#include "bejelentkezesmenu.h"
#include "debugmalloc.h"

//GOMB MUVELETEK

void BelepesRegisztracioGomb(gomb *gombok, FelhasznaloAdatok *felhasznalo) //Kattintas a belepes vagy regisztracio gombra
{
    felhasznalo->felhasznalonev[0] = '\0'; felhasznalo->jelszo[0] = '\0'; //A beirt felhasznalonev, jelszo nullazasa, ha nem lepet be a felhasznalo, hanem vissza gombot nyomott
    gombok[1].nevvegposX = gombok[1].vegposX; //Ha beviteli mezonel elcsuszik a szovegdoboz, hogy egy betut ne a gomb teljses szelessegeben irjon ki, ez a parancs ennek az erteknek a visszaallitasa
    gombok[2].nevvegposX = gombok[2].vegposX;
    strcpy(gombok[1].nev, "Felhasznalonev"); //Lehet megvaltozott a gomb neve, beviteli mezo miatt ez eselyes, ezert visszairja az eredetit
    strcpy(gombok[2].nev, "Jelszo");
    GombTiltasFeloldas(gombok, 1, 0, false); //Belepes gomb letiltasa
    gombok[0].kivalasztva = false;
    GombMegjelenitesTiltasFeloldas(gombok, 5, 1, true);
    GombTiltasFeloldas(gombok, 5, 1, true); //Minden masik gomb engedelyezese
}



void FelhasznaloGomb(gomb *gombok) //AZ input engedelyezese, a felhasznalo gomb szavanak eltuntetese
{
    if(strcmp(gombok[1].nev, "Felhasznalonev") == 0)
        gombok[1].nev[0] = '\0';
    SDL_StartTextInput();
}

void JelszoGomb(gomb *gombok) //Engedelyezi az inputot, a jelszo szot elhagyja
{
       if(strcmp(gombok[2].nev, "Jelszo") == 0)
        gombok[2].nev[0] = '\0';
        SDL_StartTextInput();
}

void JelszoIras(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event) //Ugyanaz mint a felhasznalo irasnal
{
    if(strlen(felhasznalo->jelszo) < 20)
    {
        char betu[2]; betu[0] = event->text.text[0]; betu[1] = '\0';
        strcat(felhasznalo->jelszo, betu);
        strcat(gombok[2].nev, "*");
        gombok[2].nevvegposX = (gombok[2].vegposX - gombok[2].nevkezdposX) / 20 * strlen(felhasznalo->jelszo);
        gombok[2].nevvegposX += gombok[2].nevkezdposX;
    }
}

void JelszoTorles(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event) //Ugyanaz mint a felhasznalo torlesnel
{
    if(event->key.keysym.sym == SDLK_BACKSPACE && strlen(felhasznalo->jelszo) != 0)
    {
        felhasznalo->jelszo[strlen(felhasznalo->jelszo) - 1] = '\0';
        gombok[2].nev[strlen(gombok[2].nev) - 1] = '\0';
        gombok[2].nevvegposX = (gombok[2].vegposX - gombok[2].nevkezdposX) / 20 * strlen(felhasznalo->jelszo);
        gombok[2].nevvegposX += gombok[2].nevkezdposX;
    }
}



void VisszaGomb(gomb *gombok) //Visszateres a bejelentkezes gombra
{
    gombok[0].aktiv = true; //A bejelentkezesi gomb engedelyezese
    gombok[0].kivalasztva = true;
    GombMegjelenitesTiltasFeloldas(gombok, 5, 1, false); //Minden mas gomb tiltasa
    GombTiltasFeloldas(gombok, 5, 1, false);
}



void OkGomb(gomb *gombok, FelhasznaloAdatok *felhasznalo, bool *lapcsere, adatoklista **lista, ar *hotelar) //Ok gombnal, ha sikeres a fajlbeolvasas, akkor tovabb megy a felhasznalo oldalra
{
    if(strlen(felhasznalo->felhasznalonev) > 0 && strlen(felhasznalo->jelszo) > 0 && FajlbeolvasasEsellenorzes(gombok[0].nev, felhasznalo->felhasznalonev, felhasznalo->jelszo, lista, hotelar))
    {
        *lapcsere = true; //true ertek eseten megszunik a fo loop, s a program inicializalja a masik oldalt
    }
}

//TEXTIRAS MUVELETEK

void FelhasznaloIras(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event) //Beviteli mezobe iras
{
    if(strlen(felhasznalo->felhasznalonev) < 20) //Max 20 karakter lehet
    {
        char betu[2]; betu[0] = event->text.text[0]; betu[1] = '\0';
        strcat(felhasznalo->felhasznalonev, betu); //A felhasznalohoz hozzaadja a betut
        strcpy(gombok[1].nev, felhasznalo->felhasznalonev); //Masolja a gomb nevehez, hogy a kepernyon is megjelenjen a valtozas
        gombok[1].nevvegposX = (gombok[1].vegposX - gombok[1].nevkezdposX) / 20 * strlen(felhasznalo->felhasznalonev); //A szovegdoboz szelessegenek allitasa bizonyos beuszelessegure
        gombok[1].nevvegposX += gombok[1].nevkezdposX;
    }
}

void FelhasznaloTorles(gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event) //Backspace el torles
{
    if(event->key.keysym.sym == SDLK_BACKSPACE && strlen(felhasznalo->felhasznalonev) != 0)
    {
        felhasznalo->felhasznalonev[strlen(felhasznalo->felhasznalonev) - 1] = '\0';
        strcpy(gombok[1].nev, felhasznalo->felhasznalonev);
        gombok[1].nevvegposX = (gombok[1].vegposX - gombok[1].nevkezdposX) / 20 * strlen(felhasznalo->felhasznalonev);
        gombok[1].nevvegposX += gombok[1].nevkezdposX;
    }
}
