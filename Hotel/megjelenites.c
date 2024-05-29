#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "fajladatkezeles.h"
#include "megjelenites.h"
#include "debugmalloc.h"

/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}
void AblakLetrehozas(SDL_Renderer **renderer, SDL_Window **window) //Ablak letrehozasa
{
 TTF_Init();
 sdl_init("Hotel", 1136, 640, window, renderer);
 Kepernyotorles(*renderer);
 SDL_RenderPresent(*renderer);
}

//INICIALIZALASI MUVELETEK

void GombhozRendeles(gomb *gombok, int meret, int koordinatak[][4], char nevek[][31], int szinek[][3], bool *aktivak, bool *kivalasztottak) //Hozzarendeli a gomb tombhoz a kezdoertekeket
{
    for(int i = 0; i < meret; i++)
    {
        gombok[i].kezdposX = koordinatak[i][0]; gombok[i].kezdposY = koordinatak[i][1]; gombok[i].vegposX = koordinatak[i][2]; gombok[i].vegposY = koordinatak[i][3];
        gombok[i].nevkezdposX = koordinatak[i][0]; gombok[i].nevkezdposY = koordinatak[i][1]; gombok[i].nevvegposX = koordinatak[i][2]; gombok[i].nevvegposY = koordinatak[i][3];
        strcpy(gombok[i].nev, nevek[i]);
        gombok[i].r = szinek[i][0]; gombok[i].g = szinek[i][1]; gombok[i].b = szinek[i][2];
        gombok[i].aktiv = aktivak[i];
        gombok[i].kivalasztva = kivalasztottak[i];
    }
}

void SzohozRendeles(szo *szavak, int meret, int koordinatak[][4], char nevek[][31], int szinek[][3]) //Hozzarendeli a szo tombhoz a kezdoertekeket
{
    for(int i = 0; i < meret; i++)
    {
        szavak[i].kezdposX = koordinatak[i][0]; szavak[i].kezdposY = koordinatak[i][1]; szavak[i].hossz = koordinatak[i][2]; szavak[i].szelesseg = koordinatak[i][3];
        strcpy(szavak[i].nev, nevek[i]);
        szavak[i].r = szinek[i][0]; szavak[i].g = szinek[i][1]; szavak[i].b = szinek[i][2];
    }
}

void AlakzathozRendeles(alakzat *alakzatok, int meret, int koordinatak[][4], int szinek[][3]) //Hozzarendeli a az alakzatok tombhoz a kezdoertekeket
{
    for(int i = 0; i < meret; i++)
    {
        alakzatok[i].kezdposX = koordinatak[i][0]; alakzatok[i].kezdposY = koordinatak[i][1]; alakzatok[i].vegposX = koordinatak[i][2]; alakzatok[i].vegposY = koordinatak[i][3];
        alakzatok[i].r = szinek[i][0]; alakzatok[i].g = szinek[i][1]; alakzatok[i].b = szinek[i][2];
    }
}

void LogOldal(SDL_Renderer *renderer, char *letezik, gomb *gombok, szo *szavak) //Belepes oldal gombjainak, szavainak, alakzatainak inicializalasa
{
    int koordinatak[5][4] = {{450, 350, 650, 400}, {440, 250, 660, 300}, {440, 350, 660, 400}, {440, 450, 540, 500}, {560, 450, 660, 500}};
    char nevek[5][31] = {"", "Felhasznalonev", "Jelszo", "Vissza", "Ok"};
    int szinek[5][3] = {{119, 73, 54}, {245, 208, 197}, {245, 208, 197}, {119, 73, 54}, {119, 73, 54}};
    bool aktivak[5] = {true, false, false, false, false};
    bool kivalasztottak[5] = {true, false, false, false, false};
    strcpy(nevek[0], letezik);
    int szokoordinatak[1][4] = {{450, 50, 200, 200}};
    char szonevek[1][31] = {"Hotel"};
    int szoszinek[1][3] = {{0, 0, 0}};
    SzohozRendeles(szavak, 1, szokoordinatak, szonevek, szoszinek);
    GombhozRendeles(gombok, 5, koordinatak, nevek, szinek, aktivak, kivalasztottak);
    Kepernyofrissites(renderer, gombok, 5, NULL, 0, szavak, 1);
}

void FelhasznaloOldal(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak) //Felhasznalo oldal gombjainak, szavainak, alakzatainak inicializalasa
{
    int alakzatkoordinatak[9][4] = {{20, 100, 650, 500}, {30, 110, 640, 490}, {30, 182, 640, 187}, {30, 259, 640, 264}, {30, 336, 640, 341}, {30, 413, 640, 418}, {660, 146, 665, 454}, {720, 100, 1116, 352}, {0, 0, 0, 0}};
    int alakzatszinek[9][3] = {{119, 73, 54}, {245, 208, 197}, {60, 0, 0}, {60, 0, 0}, {60, 0, 0}, {60, 0, 0}, {0, 0, 0}, {245, 208, 197}, {0, 0, 0}};
    int gombkoordinatak[41][4] = {{800, 20, 1016, 80}, {800, 20, 1016, 80}, {430, 570, 680, 620}, {780, 570, 1030, 620}, {790, 105, 1111, 131}, {790, 136, 1111, 162}, {810, 172, 1111, 190}, {930, 208, 1111, 234}, {865, 360, 915, 410}, {940, 360, 990, 410}, {1015, 360, 1065, 410}, {1090, 360, 1140, 410}, {875, 420, 925, 470}, {1010, 420, 1060, 470}, {890, 480, 940, 530}, {1025, 480, 1075, 530}};
    char gombnevek[41][31] = {"Torles", "Uj felvetel", "EEEE.HH.NN", "EEEE.HH.NN", "", "", "", "", "","","","","","","",""};
    int gombszinek[41][3];
    bool gombaktivak[41] = {false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false};
    bool gombkivalasztottak[41] = {false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
        for(int i = 0; i < 16; i++)
    {
             gombszinek[i][0] = 245;
             gombszinek[i][1] = 208;
             gombszinek[i][2] = 197;
    }
    int x = 62; int y = 121; int index = 16; int szoba = 401;
        for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            gombkoordinatak[index][0] = x; gombkoordinatak[index][1] = y; gombkoordinatak[index][2] = x + 62; gombkoordinatak[index][3] = y + 50;
            gombszinek[index][0] = 0; gombszinek[index][1] = 255; gombszinek[index][2] = 0;
            sprintf(gombnevek[index], "%03d", szoba);
            gombaktivak[index] = false;
            gombkivalasztottak[index] = false;
            index++; x += 122; szoba++;
        }
        x = 62; y += 77; szoba -= 105;
    }
    int szokoordinatak[25][4] = {{20, 560, 150, 70}, {200, 560, 150, 70}, {690, 570, 70, 50}, {1040, 570, 70, 50}, {675, 418, 30, 36}, {675, 359, 30, 36}, {675, 287, 30, 36}, {675, 215, 30, 36}, {675, 143, 30, 36}, {725, 105, 60, 26}, {725, 136, 60, 26}, {725, 172, 80, 26}, {725, 208, 200, 26}, {725, 244, 170, 26}, {725, 280, 260, 26}, {725, 316, 40, 26}, {770, 316, 100, 26}, {720, 360, 140, 50}, {920, 360, 15, 50}, {995, 360, 15, 50}, {1070, 360, 15, 50}, {720, 420, 150, 50}, {930, 420, 75, 50}, {720, 480, 165, 50}, {945, 480, 75, 50}};
    char szonevek[25][31] = {"L: Lezaras", "S: Mentes", "-tol", "-ig", "0.", "1.", "2.", "3.", "4.", "Nev:", "Tel:", "Email:", "Foglalas idopontja:", "Foglalt napok:", "", "Ar:", "", "Ferohely: 1", "2", "3", "4", "Klima: van", "nincs", "Terasz: van", "nincs"};
    int szoszinek[25][3] = {{119, 73, 54}, {119, 73, 54}};
    for(int i = 2; i < 25; i++)
    {
             szoszinek[i][0] = 0;
             szoszinek[i][1] = 0;
             szoszinek[i][2] = 0;
    }
    GombhozRendeles(gombok, 41, gombkoordinatak, gombnevek, gombszinek, gombaktivak, gombkivalasztottak);
    AlakzathozRendeles(alakzatok, 9, alakzatkoordinatak, alakzatszinek);
    SzohozRendeles(szavak, 25, szokoordinatak, szonevek, szoszinek);
    Kepernyofrissites(renderer, gombok, 0, alakzatok, 9, szavak, 0);
}

//GRAFIKUS MEGJELENITES MUVELETEK

void SzoMegjelenites(SDL_Renderer *renderer, szo *szavak, int szomeret, gomb *gombszo) //Szot jelenit, ket lehetoseg van benne, az egyik mikor egy gombnak a szavat jeleniti meg, oda nem kell loop
{                                                                                      //Valamint a teljes kepernyofrissitesnel is ez hivodik meg, akkor viszont a gomb nelkuli szavakon loopol

    if(szavak == NULL)
    {
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 100);
    SDL_Color color = {0, 0, 0};
    SDL_Surface *felulet = TTF_RenderText_Solid(font, gombszo->nev, color);
    SDL_Texture *szovegTexture = SDL_CreateTextureFromSurface(renderer, felulet);
    SDL_Rect szovegRect;
    szovegRect.x = gombszo->nevkezdposX;
    szovegRect.y = gombszo->nevkezdposY;
    szovegRect.w = gombszo->nevvegposX - gombszo->nevkezdposX;
    szovegRect.h = gombszo->nevvegposY - gombszo->nevkezdposY;
    SDL_RenderCopy(renderer, szovegTexture, NULL, &szovegRect);
    SDL_DestroyTexture(szovegTexture);
    SDL_FreeSurface(felulet);
    }
    else
    {
        for(int i = 0; i < szomeret; i++)
        {
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 100);
    SDL_Color color = {szavak[i].r, szavak[i].g, szavak[i].b};
    SDL_Surface *felulet = TTF_RenderText_Solid(font, szavak[i].nev, color);
    SDL_Texture *szovegTexture = SDL_CreateTextureFromSurface(renderer, felulet);
    SDL_Rect szovegRect;
    szovegRect.x = szavak[i].kezdposX;
    szovegRect.y = szavak[i].kezdposY;
    szovegRect.w = szavak[i].hossz;
    szovegRect.h = szavak[i].szelesseg;
    SDL_RenderCopy(renderer, szovegTexture, NULL, &szovegRect);
    SDL_DestroyTexture(szovegTexture);
    SDL_FreeSurface(felulet);
        }
    }
    }

void TeglalapMegjelenites(SDL_Renderer *renderer, alakzat *alakzatok, int alakzatmeret, gomb *gombhatter) //Teglalapot jelenit meg
{
    if(alakzatok == NULL && gombhatter != NULL)
        boxRGBA(renderer, gombhatter->kezdposX, gombhatter->kezdposY, gombhatter->vegposX, gombhatter->vegposY, gombhatter->r, gombhatter->g, gombhatter->b, 255);
    else
    {
        for(int i = 0; i < alakzatmeret; i++)
            boxRGBA(renderer, alakzatok[i].kezdposX,  alakzatok[i].kezdposY, alakzatok[i].vegposX, alakzatok[i].vegposY, alakzatok[i].r, alakzatok[i].g, alakzatok[i].b, 255);
    }
}

void Gombmegjelenites(SDL_Renderer *renderer ,gomb *gombok, int meret) //Megjeleniti a gombot, ha megjelenitheto
{
    for(int i = 0; i < meret; i++)
    {
        if(gombok[i].kivalasztva == true)
        {
        TeglalapMegjelenites(renderer, NULL, 0, &gombok[i]); //A gomb egy teglalapbol, es egy szobol all
        SzoMegjelenites(renderer, NULL, 0, &gombok[i]);
        }
    }
}

void GombTiltasFeloldas(gomb *gombok, int meret, int kezdoertek, bool valaszt) //Ezzel a funkcióval lehet engedelyezni, tiltani a gomb kattinthatosagat
{
    for(int i = kezdoertek; i < meret; i++)
        gombok[i].aktiv = valaszt; //Ez a bool hatarozza meg, hogy tiltas, vagy engedelyezes
}

void GombMegjelenitesTiltasFeloldas(gomb *gombok, int meret, int kezdoertek, bool valaszt) //Ezzel a funkcioval a gombok megjeleniteset tiltani vagy engedelyezni
{
        for(int i = kezdoertek; i < meret; i++)
        gombok[i].kivalasztva = valaszt; //Ez a bool hatarozza meg, hogy tiltas, vagy engedelyezes
}

void Kepernyotorles(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 214, 159, 126, 255);
    SDL_RenderClear(renderer);
}

void FoglaltsagStatuszMegjelenites(gomb *gombok, ListazandoFoglalasok *listazandofoglalasok) //Pirosra szinezi a foglalt, zoldre a nem foglalt szobakat
{
    for(int i = 0; i < 25; i++)
    {
     if(listazandofoglalasok->lista[i] != NULL)
     {
         gombok[i + 16].r = 255;
         gombok[i + 16].g = 0;
         gombok[i + 16].b = 0;
     }
     else
     {
         gombok[i + 16].r = 0;
         gombok[i + 16].g = 255;
         gombok[i + 16].b = 0;
     }
    }

}

void Kepernyofrissites(SDL_Renderer *renderer, gomb *gombok, int gombmeret, alakzat *alakzatok, int alakzatmeret, szo *szavak, int szomeret) //Egyszerre frissiti a teljes ablakot
{
    Kepernyotorles(renderer);
    TeglalapMegjelenites(renderer, alakzatok, alakzatmeret, NULL);
    Gombmegjelenites(renderer, gombok, gombmeret);
    SzoMegjelenites(renderer, szavak, szomeret, NULL);
    SDL_RenderPresent(renderer);
}
