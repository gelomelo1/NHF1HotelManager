#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "felhasznalomenu.h"
#include "megjelenites.h"
#include "fajladatkezeles.h"
#include "bejelentkezesmenu.h"
#include "debugmalloc.h"

//FAJLMUVELET

bool FajlbeolvasasEsellenorzes(char *autentikacio, char *felhasznalonev, char *jelszo, adatoklista **lista, ar *hotelar) //Igazat ad vissza ha sikeres a fajbol olvasas, vagy a fajlbol iras
{
    bool autentikaciosikeres = false; //felhasznalo es jelszot tartalmazo fajl ellenorzesere szolgal
    bool egysegarsikeres = false; //arakat tartalmazo fajl ellenorzesere szolgal
    bool foglalasadatoksikeres = false; //foglalasi adatokat tartalmazo fajl ellenorzesere szolgal
    char eredmeny[6][31];
    if(strcmp(autentikacio, "Belepes") == 0) //Ket lehetoseg van, vagy regisztracio, vagy belepes, belepesnel fajbol olvasat ellenoriz, regisztracional fajlba irast ellenorzi
    {
        autentikaciosikeres = FajbolBeolvasas("autentikacio.bin", 2, eredmeny);
        if(autentikaciosikeres)
        {
            if(strcmp(felhasznalonev, eredmeny[0]) != 0 || strcmp(jelszo, eredmeny[1]) != 0) //Ha nem 0 hosszusagu a felhasznalonev, es jelszo, leelenorzi, hogy helyesek e
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Figyelem", "Helytelen felhasznalonev vagy jelszo!", NULL);
                return false;
            }
            egysegarsikeres = FajbolBeolvasas("egysegar.txt", 6, eredmeny); //Ha helyesek akkor tovabb fut a fuggveny, es beolvassa a fajlokat
            for(int i = 0; i < 6; i++)
                hotelar->arak[i] = atoi(eredmeny[i]);
            foglalasadatoksikeres = FoglalasBeolvas("foglalasok.txt", lista);
            if(egysegarsikeres && foglalasadatoksikeres)
                return true;
        }
    }
    else
    {
       strcpy(eredmeny[0], felhasznalonev);
       strcpy(eredmeny[1], jelszo);
       autentikaciosikeres = FajlbaIras("autentikacio.bin", 2, eredmeny); //Elso elinditaskor letrehozza a binaris fajlt, illetve a ketto szovegfajlt
       ar alap = alapertelmezettar();
       for(int i = 0; i < 6; i++)
       sprintf(eredmeny[i], "%d", alap.arak[i]); //alapertelmezett arakkal valo feltoltes
       egysegarsikeres = FajlbaIras("egysegar.txt", 6, eredmeny);
       for(int i = 0; i < 6; i++)
           hotelar->arak[i] = alap.arak[i];
       char mezo[1][31] = {""};
       foglalasadatoksikeres = FajlbaIras("foglalasok.txt", 1, mezo); //Ures mezovel valo kitoltes
       if(autentikaciosikeres && egysegarsikeres && foglalasadatoksikeres)
        return true;
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Figyelem", "Hiba tortent a fajlbeolvasas soran!", NULL); //Ha nem sikerul a fajbeolvasas
    return false;
}

//INICIALIZALAS ALAPERTEKEKKEL

void LoginLap(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak) //Letrehozza a loginlapot
{
    if(FajlLetezik("autentikacio.bin")) //Ha elso elinditas, akkor regisztraciot kell kiirni, ha tobbszori inditas akkor belepest
        LogOldal(renderer, "Belepes", gombok, szavak);
    else
        LogOldal(renderer, "Regisztracio", gombok, szavak);
    alakzatok = NULL;
}

void FelhasznaloLap(SDL_Renderer *renderer, gomb *gombok, alakzat *alakzatok, szo *szavak) //Letrehozza a felhasznalolapot
{
    FelhasznaloOldal(renderer, gombok, alakzatok, szavak);
}

void Hoteladatok(hotelszobak *hoteladat) //A hotelszobak alapertelmezett adataival valo feltoltes
{
    int ferohely[25] = {2, 3, 1, 4, 4, 1, 3, 2, 2, 2, 3, 1, 2, 2, 4, 4, 4, 2, 3, 1, 2, 4, 4, 4, 3}; //Ferohely szamot jelol
    int klima[25] = {2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 1}; //Egyes jeloli ha van, kettes ha nincs
    int terasz[25] = {1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 2, 2, 1};
    for(int i = 0; i < 25; i++)
    {
        hoteladat[i].ferohely = ferohely[i];
        hoteladat[i].klima = klima[i];
        hoteladat[i].terasz = terasz[i];
    }
}

//SEGEDFUGGVENYEK

bool Fedes(gomb *cel, int x, int y) //Eger kattintasnal ellenorzi, hogy a kattintas pozicioja egybeesik e a gomb poziciojaval
{
    if((x >= cel->kezdposX && x <= cel->vegposX) && (y >= cel->kezdposY && y <= cel->vegposY))
        return true;
    return false;
}

int Gombindex(gomb *gombok, int meret, SDL_Event *event) //Meghatarozza, hogy kattintas eseten, melyik gombot nyomjuk meg
{
    int index = 0;
    while(index < meret) //Mivel ezt a fuggvenyt hasznaljuk a felhasznalo, es a login lapnal is, megadjuk a meretet, hogy hany gombot kell ellenorizni
    {
        if(event->button.button == SDL_BUTTON_LEFT && Fedes(&gombok[index], event->button.x, event->button.y) && gombok[index].aktiv == true)
            return index;
        index++;
    }
    return -1;
}

//FELHASZNALO EVENTEK FO LOOPHOZ TARTOZO FUGGVENYEK

void LoginOldalGombfunkciok(int index, gomb *gombok, FelhasznaloAdatok *felhasznalo, SDL_Event *event, int billentyuevent, bool *lapcsere, adatoklista **lista, ar *hotelar) //Ez a fuggveny hivja meg a login oldalnal a megfelelo indexu gomb funkciojat
{
        if(index == 0)
            BelepesRegisztracioGomb(gombok, felhasznalo);
        if(index == 1)
        {
            if(billentyuevent == 0)
                FelhasznaloGomb(gombok);
            if(billentyuevent == 1)
                FelhasznaloTorles(gombok, felhasznalo, event);
            if(billentyuevent == 2)
                FelhasznaloIras(gombok, felhasznalo, event);
        }
        if(index == 2)
        {
            if(billentyuevent == 0)
                JelszoGomb(gombok);
            if(billentyuevent == 1)
                JelszoTorles(gombok, felhasznalo, event);
            if(billentyuevent == 2)
                JelszoIras(gombok, felhasznalo, event);
        }
        if(index == 3)
            VisszaGomb(gombok);
        if(index == 4)
            OkGomb(gombok, felhasznalo, lapcsere, lista, hotelar);

}


void FelhasznaloOldalGombfunkciok(int index, gomb *gombok, alakzat *alakzatok, szo *szavak, hotelszobak *szobak, SDL_Event *event, ListazandoFoglalasok *listazandofoglalasok, int billentyuevent, adatoklista **lista, ar *hotelar) //Ez a fuggveny hivja meg a felhasznalo oldalnal a megfelelo indexu gomb funkciojat
{ //A billentyuevent valtozo azt jeloli, hogy eppen kattintas van, szovegbevitel, vagy backspace
    if(index >= 8 && index <= 15 && billentyuevent == 0) //Radigombok
    {
             RadioGombok(index, gombok, alakzatok, szavak, szobak);
    }
   else if(index == 2 || index == 3) //-tol -ig datum gombok
    {
        HotelKijelolesVege(alakzatok, gombok, szavak);
        if(billentyuevent == 0)
        DatumGomb(gombok, index);
        if(billentyuevent == 1)
        AdatTorles(gombok, event, index, 10);
        if(billentyuevent == 2)
        AdatIras(gombok, event, index, 10);
        int datumertek[2];
        bool megjelenites = DatumFormatumEllenorzes(gombok, datumertek); //Leellenorzi, hogy helyes e a ket datum formaja, ervenyessege, valamint azt, hogy az -ig datum nagyobb e a -tol datumnal
        bool vanszuro = false;
        if(megjelenites) //Ha megjelenithetok a szobak, akkor a foglalasi adatok kilistazasa, s megjelenitese
        {
            int i = 8;
            while(!vanszuro && i < 16)
            {
                if(strcmp(gombok[i].nev, "") != 0)
                    vanszuro = true;
                i++;
            }
            FoglaltSzobak(gombok, datumertek, listazandofoglalasok, *lista);
            GombTiltasFeloldas(gombok, 16, 8, true);
        }
        if(!vanszuro) //Ha nincs szuro akkor engedelyezni vagy tiltani kell minden gombot, attol fuggoen, hogy a -tol -ig datum helyes e
        {
        GombTiltasFeloldas(gombok, 41, 16, megjelenites);
        GombMegjelenitesTiltasFeloldas(gombok, 41, 16, megjelenites);
        }
        if(!megjelenites) //Ha nem jelenithetoek meg, a szuro gombokat nem lehet hasznalni
        {
            for(int i = 8; i < 16; i++)
                strcpy(gombok[i].nev, "");
            GombTiltasFeloldas(gombok, 16, 8, false);
        }
            GombTiltasFeloldas(gombok, 8, 4, false);
    }
   else if(index >= 16) //16 indextol felfele csak szoba gombok vannak
    {
        if(listazandofoglalasok->lista[index - 16] != NULL) //A listazandofoglalasok struct tartalmazza a 25 elemu pointer tombot, ami szobankent jeloli, hogy van e ott foglalas vagy nincs, null pointer eseten nincs foglalas
        {
            FoglalasAdatMegjelenites(gombok, szavak, index, listazandofoglalasok, *lista);
            GombTiltasFeloldas(gombok, 8, 4, false); //Foglalt szoba, ezert a foglalsi adatok beviteli mezoit tiltani kell, mert ott a foglalt szoba adatai vannak
        }
        else //Ha nincs foglalas akkor a fix adatok megjelenitese, s a beviteli mezok engedelyezese
        {
            HotelKijelolesVege(alakzatok, gombok, szavak);
            UjFoglalasAdatMegjelenites(gombok, szavak, hotelar, szobak, index);
            GombTiltasFeloldas(gombok, 8, 4, true);
        }
        HotelKijeloles(gombok, alakzatok, index);
    }
  else  if(index >= 4 && index <= 7) //Beviteli mezok
    {
        if(billentyuevent == 0)
            FoglalasiAdatGomb();
        if(billentyuevent == 1)
            AdatTorles(gombok, event, index, 30);
        if(billentyuevent == 2)
            AdatIras(gombok, event, index, 30);
        FoglalasGombMegjelenites(gombok);
    }
   else if(index == 0) //Torles gomb
        TorlesGomb(gombok, alakzatok, szavak, lista, listazandofoglalasok);
  else  if(index == 1) //Foglalas gomb
        FoglalasGomb(gombok, alakzatok, szavak, lista, listazandofoglalasok);
}

void ProgramEvenetek(SDL_Renderer *renderer, SDL_Window *window, gomb *gombok, int gombmeret, alakzat *alakzatok, int alakzatmeret, szo *szavak, int szomeret, bool loginaktivoldal, bool *programfut, adatoklista **lista, ar *hotelar)
{ //Ez a fo loop, itt regisztralodnak a felhasznaloi eventek kilepesig, vagy oldalvaltasig, oldalvaltas utan mas parameterekkel, ugyanez a loop fut
    bool irasengedelyezes = false; //arra szolgal, hogy a S mentes es az L lezaras billentyuket lenyomhassuk, es ez ne szoljon bele abba mikor szovegbevitel van
    bool lapcsere = false; //Ha ezt megvaltoztatjuk megszunik a loop
    FelhasznaloAdatok felhasznalo; //Jelszo, felhasznalonev struct
    hotelszobak szobak[25]; //hotelszobak adatait tartalmazo struct
    Hoteladatok(szobak);
    ListazandoFoglalasok listazandofoglalasok; //struct amely egy ketto 25 elemu tombbot tartalmaz, kilistazasnal van szerepe, ez hatarozza meg, hogy mely szobak foglaltak, s mely szobak szabadok
    ListaInicializalas(&listazandofoglalasok); //a pointer tomb szobankent vagy null pointert vagy annak a lancolt lista elemnek a pointeret tartalmazza amelyikben benne van a foglalasi adat
    SDL_StopTextInput();                       //a masik int tomb azt adja, hogy a lancolt listan beluli dinamikus tombben melyik index alatt talalhato a szobahoz tartozo foglalasi adat
    SDL_Event event;
    int gombindex;
    while(*programfut && !lapcsere)
    {

        SDL_WaitEvent(&event);
        switch(event.type)
        {

        case SDL_MOUSEBUTTONDOWN: //Kattintas eseten a gomb indexenek meghatarozasa, majd aktiv oldaltol fuggoen a megfelelo fuggvenyhivas
            {
                SDL_StopTextInput();
                irasengedelyezes = false;
                gombindex = Gombindex(gombok, gombmeret, &event);
                if(gombindex != -1)
                {
                   if(loginaktivoldal)
                   LoginOldalGombfunkciok(gombindex, gombok, &felhasznalo, &event, 0, &lapcsere, lista, hotelar);
                   else
                   {
                        if(gombindex >= 2 && gombindex <= 7)
                            irasengedelyezes = true;
                        FelhasznaloOldalGombfunkciok(gombindex, gombok, alakzatok, szavak, szobak, &event, &listazandofoglalasok, 0, lista, hotelar);
                   }
                }
                break;
            }
        case SDL_KEYDOWN: //Backspace, illetve az l, es s billentyuket regisztralja
            {
                    if(loginaktivoldal)
                    LoginOldalGombfunkciok(gombindex, gombok, &felhasznalo, &event, 1, &lapcsere, lista, hotelar);
                    else
                    {
                        if(!irasengedelyezes)
                        {
                            if(event.key.keysym.sym == SDLK_s)
                                Mentes(*lista);
                            if(event.key.keysym.sym == SDLK_l)
                                Lezaras(&lapcsere);
                        }
                        else
                        FelhasznaloOldalGombfunkciok(gombindex, gombok, alakzatok, szavak, szobak, &event, &listazandofoglalasok, 1, lista, hotelar);
                    }
                break;
            }
        case SDL_TEXTINPUT: //Szoveg bevitel eseten a megfelelo fuggveny meghivasa
            {
                if(loginaktivoldal)
                LoginOldalGombfunkciok(gombindex, gombok, &felhasznalo, &event, 2, &lapcsere, lista, hotelar);
                else
                FelhasznaloOldalGombfunkciok(gombindex, gombok, alakzatok, szavak, szobak, &event, &listazandofoglalasok, 2, lista, hotelar);
                break;
            }
        case SDL_QUIT: //Kilepesnel a kulso main loop valtozojat hamisra allitjuk
            {
                *programfut = false;
                break;
            }
        }
        if(event.type != SDL_MOUSEMOTION) //Akkor updateljuk a kirajzolast, amikor felhasznaloi input van, de nem minden esetben, ugyanis eger mozgatas eseten felesleges
            Kepernyofrissites(renderer, gombok, gombmeret, alakzatok, alakzatmeret, szavak, szomeret);
    }
}
