#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#include "fajladatkezeles.h"
#include "felhasznalomenu.h"
#include "megjelenites.h"
#include "debugmalloc.h"

//GOMB MUVELETEK VALAMINT GYAKRAN HASZNALT MUVELETEK

void RadioGombok(int index, gomb *gombok, alakzat *alakzatok, szo *szavak, hotelszobak *szobak) //Szuri a szobakat a bejelolt szurok szerint
{
    int kezdszam, vegeszam;
    if(index >= 8 && index <= 11) //Meghatarozza hogy a harom fajta szuresbol, melyik szurest haszbaljuk
    {
        kezdszam = 8; vegeszam = 12; //Ferohely szerinti szures
    }
    if(index >= 12 && index <= 13)
    {
        kezdszam = 12; vegeszam = 14; //Klima szerinti szures
    }
        if(index >= 14 && index <= 15)
    {
        kezdszam = 14; vegeszam = 16; //Terasz szerinti szures
    }
       for(int i = kezdszam; i < vegeszam; i++)
     {
         if(i != index)
            strcpy(gombok[i].nev, ""); //Vegigmegy az adott tartomanyon, s minden olyan helyrol ami nem az aktualisan kattintott szuro gomb kitorli az X-et
     }                                 //Ez valojaban azt akadalyozza meg, hogy egy tipushoz tartozo tobb szurogombbol tobb legyen beikszelve
    if(strcmp(gombok[index].nev, "") == 0)
    strcpy(gombok[index].nev, "X"); //Ha ures a gomb, beikszeli
    else
    strcpy(gombok[index].nev, ""); //Ha be van ikszelve akkor kitorli
    int ferohelyszuro = 0; //Alapertelmezett ertekek, azt jelenti hogy nincs szures ebben a tipusban
    int klimaszuro = 0;
    int teraszuro = 0;
    int szuroszam = 0;
    for(int i = 8; i < 16; i++)
    {
        if(strcmp(gombok[i].nev, "X") == 0) //Megadja a szuroszamot, vagyis hogy hany tipus szerint lesz a szures, valamint, tipusonkent, hogy melyik szurk vannak beikszelve
        {
            if(i >= 8 && i <= 11)
            {
                ferohelyszuro = i - 7; szuroszam++;
            }
            if(i >= 12 && i <= 13)
            {
                klimaszuro = i - 11; szuroszam++;
            }
            if(i >= 14 && i <= 15)
            {
                teraszuro = i - 13; szuroszam++;
            }
        }
    }
    for(int i = 0; i < 25; i++) //Vegigmegy a hotelszoba gombokon
    {
        if(szuro(&szobak[i], ferohelyszuro, klimaszuro, teraszuro, szuroszam)) //Benne van e a szuresben vagy nincs
        {
            gombok[i + 16].aktiv = true; //i + 16, mert a gomb tombben a 16-os indexel kezdodnek a hotelgombok
            gombok[i + 16].kivalasztva = true;
        }
        else
        {
            gombok[i + 16].aktiv = false;
            gombok[i + 16].kivalasztva = false;
            if(alakzatok[8].kezdposX == gombok[i + 16].kezdposX - 5 && alakzatok[8].kezdposY == gombok[i + 16].kezdposY - 5 && alakzatok[8].vegposX == gombok[i + 16].vegposX + 5 && alakzatok[8].vegposY == gombok[i + 16].vegposY + 5)
            {
                    GombTiltasFeloldas(gombok, 8, 4, false); //Ha epp ki van jelolve egy hotelszoba, akkor megszunteti a kijelolest, valamint tiltja az adatok beviteli mezoit
                    HotelKijelolesVege(alakzatok, gombok, szavak);
            }
        }
    }
}

void DatumGomb(gomb *gombok, int index) //Engedelyezi az inputot, valamint kitorli a formatum jelolo szot
{
        if(strcmp(gombok[index].nev, "EEEE.HH.NN") == 0)
        gombok[index].nev[0] = '\0';
    SDL_StartTextInput();
}

bool DatumFormatumEllenorzes(gomb *gombok, int *datumertek) //A -tol -ig datum megadasakor osszehasonlitja a ket datumot, hogy a -tol datum kisebb e az -ig datumnal
{
    int datum1;
    int datum2;
    if(strlen(gombok[2].nev) == 10 && strlen(gombok[3].nev) == 10) //Akkor ellenoriz ha a mindket datum eleri a 10 hosszusagot, vagyis az EEEE.HH.NN formatumot
    {
        bool form1, form2;
        form1 = Formatum(gombok, 2);
        form2 = Formatum(gombok, 3);
        if(form1 && form2)
        {
                  int ev[2], ho[2], nap[2];
                  datum1 = Datumatvaltas(gombok[2].nev, false); //A ket datum visszaadasa a hivonak
                  datum2 = Datumatvaltas(gombok[3].nev, false);
                  sscanf(gombok[2].nev, "%4d.%2d.%2d", &ev[0], &ho[0], &nap[0]);
                  sscanf(gombok[3].nev, "%4d.%2d.%2d", &ev[1], &ho[1], &nap[1]);
                  int napok = Napszamlalas(ev, ho, nap); //Ket datum kozotti napok szamanak meghatarozasa
                  datumertek[0] = datum1;
                  datumertek[1] = datum2;
                  if(napok > 0 && napok < 32) // Ha nagyobb az -ig ertek, valmint ha beleseik a maximalisan foglalhato 31 napba
                    return true;
        }
    }
    return false;
}

void HotelKijeloles(gomb *gombok, alakzat *alakzatok, int index) //Beallitja a kijelolo negyzet poziciojat a kijelolt hotelszoba gombra
{
     alakzatok[8].kezdposX = gombok[index].kezdposX - 5;
     alakzatok[8].kezdposY = gombok[index].kezdposY - 5;
     alakzatok[8].vegposX = gombok[index].vegposX + 5;
     alakzatok[8].vegposY = gombok[index].vegposY + 5;
}

void HotelKijelolesVege(alakzat *alakzatok, gomb *gombok, szo *szavak) //A kijelolo negyzetetet eltunteti
{
    alakzatok[8].kezdposX = 0;
    alakzatok[8].kezdposY = 0;
    alakzatok[8].vegposX = 0;
    alakzatok[8].vegposY = 0;
    GombTiltasFeloldas(gombok, 2, 0, false); //Ha a foglalas vagy torles gomb aktiv akkor azokat kikapcsolja
    GombMegjelenitesTiltasFeloldas(gombok, 2, 0, false);
    for(int i = 4; i < 8; i++)
    {
        gombok[i].nev[0] = '\0';
    }
    szavak[14].nev[0] = '\0'; //Kitorli a beviteli bezokbe gepelt szoveget
    szavak[16].nev[0] = '\0';
}

void FoglaltSzobak(gomb *gombok, int *datumertek, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista) //Megjeleniti a szobakat a foglaltsag alapjan
{
    ListaInicializalas(listazandofoglalasok);
    Kereses(gombok, listazandofoglalasok, lista, datumertek); //Adatok betoltese
    FoglaltsagStatuszMegjelenites(gombok, listazandofoglalasok); //A szobak szinezese a foglaltsag alapjan
}

void FoglalasAdatMegjelenites(gomb *gombok, szo *szavak, int index, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista) //Kilistazza a foglalasi adatokat
{
    GombTiltasFeloldas(gombok, 2, 1, false); //Letiltja a foglalas gombot, mert lehet meg van jelenitve
    GombMegjelenitesTiltasFeloldas(gombok, 2, 1, false);
    GombTiltasFeloldas(gombok, 1, 0, true); //A torles gomb engedelyzese
    GombMegjelenitesTiltasFeloldas(gombok, 1, 0, true);
    Listazas(gombok, NULL,  4, listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].nev); //Nev kiirasa
    Listazas(gombok, NULL, 5, listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].tel); //Telefonszam kiirasa
    Listazas(gombok, NULL,  6, listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].email); //Email kiirasa
    char datum[22];
    DatumvaltasVissza(listazandofoglalasok->lista[index - 16]->foglalasidatum, datum); //int tipusu datum atvaltasa sztringbe
    Listazas(gombok, NULL, 7, datum); //Foglalasi datum kilistazasa
    char masikdatum[11];
    DatumvaltasVissza(listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].startdatum, datum);
    DatumvaltasVissza(listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].stopdatum, masikdatum);
    datum[10] = '-'; datum[11] = '\0'; strcat(datum, masikdatum);
    Listazas(NULL, szavak, 14, datum); //tol-ig datum kilistazasa
    char ar[33]; sprintf(ar, "%dFt", listazandofoglalasok->lista[index - 16]->szemelyek[listazandofoglalasok->foglalasindex[index - 16]].ar);
    Listazas(NULL, szavak, 16, ar); //ar kilistazasa
}

void FoglalasiAdatGomb() //Input engedelyezese
{
    SDL_StartTextInput();
}

void UjFoglalasAdatMegjelenites(gomb *gombok, szo *szavak, ar *hotelar, hotelszobak *szobak, int index) //Foglalatlan szobanal beirja a fix ertekeket, vagyis a foglalt napokat, es az arat
{
    char datum[22];
    strcpy(datum, gombok[2].nev);
    datum[10] = '-'; datum[11] = '\0';
    strcat(datum, gombok[3].nev);
    Listazas(NULL, szavak, 14, datum);
    char teljeshotelar[33]; sprintf(teljeshotelar, "%dFt", Arszamlalas(gombok, hotelar, szobak[index - 16])); //A hotelar kiszamitasa
    Listazas(NULL, szavak, 16, teljeshotelar);
}

void FoglalasGombMegjelenites(gomb *gombok) //Ha minden mezo ki van toltve, megjeleniti a foglalas gombot
{
    bool kitolteve = true;
    int index = 4;
    while(kitolteve && index < 8)
    {
        if(strlen(gombok[index].nev) == 0)
            kitolteve = false;
        index++;
    }
    if(kitolteve && Formatum(gombok, 7) && strlen(gombok[7].nev) == 10) //A kitoltott mezo mellett a helyes foglalasi datum megadasa
    {
        GombTiltasFeloldas(gombok, 2, 1, true);
        GombMegjelenitesTiltasFeloldas(gombok, 2, 1, true);
    }
    else //Ha meg van jelenitve a foglalas gomb, viszont nem teljesulnek a feltetelek akkor tiltas
    {
        GombTiltasFeloldas(gombok, 2, 1, false);
        GombMegjelenitesTiltasFeloldas(gombok, 2, 1, false);
    }
}

void TorlesGomb(gomb *gombok, alakzat *alakzatok, szo *szavak, adatoklista **lista, ListazandoFoglalasok *listazandofoglalasok) //Elinditja a torles folyamatot
{
    if(FoglalasTorles(listazandofoglalasok, lista, KivalasztottSzoba(gombok, alakzatok, NULL))) //Ha a torles sikeres, akkor megszunteti az adott szoba kijeloleset, s ujralistazza a foglalt szobakat
    {
        int datumertek[2];
        datumertek[0] = Datumatvaltas(gombok[2].nev, false);
        datumertek[1] = Datumatvaltas(gombok[3].nev, false);
        HotelKijelolesVege(alakzatok, gombok, szavak);
        FoglaltSzobak(gombok, datumertek, listazandofoglalasok, *lista);
    }
}

void FoglalasGomb(gomb *gombok, alakzat *alakzatok, szo *szavak, adatoklista **lista, ListazandoFoglalasok *listazandofoglalasok) //Elinditja a foglalas folyamatot
{
    char adatok[4][31]; strcpy(adatok[0], gombok[4].nev); strcpy(adatok[1], gombok[5].nev); strcpy(adatok[2], gombok[6].nev); //Nev, tel, email, szobaszam masolas
    int datum[3]; datum[0] = Datumatvaltas(gombok[7].nev, false); datum[1] = Datumatvaltas(gombok[2].nev, false); datum[2] = Datumatvaltas(gombok[3].nev, false); //foglalasi datum, startdatum, stopdatum
    int ar = atoi(szavak[16].nev);
    if(FoglalasFelvetel(listazandofoglalasok, lista, KivalasztottSzoba(gombok, alakzatok, adatok[3]), adatok, datum, ar)) //Ha sikeres a foglalas, megszunteti a kijelolest, azonban most nem kell teljes frissitest csinalni, mert hozzaadtunk elemet, biztosan tudjuk, hogy csak ez az elem adodott hozza a listahoz
    {
        HotelKijelolesVege(alakzatok, gombok, szavak);
        FoglaltsagStatuszMegjelenites(gombok, listazandofoglalasok);
    }
}

void Mentes(adatoklista *lista) //Elinditja a mentes folyamatot
{
    FoglalasIras("foglalasok.txt", lista);
}

void Lezaras(bool *lapcsere) //Oldal valtas
{
    *lapcsere = true;
}

//TEXTIRASMUVELETEK

void AdatIras(gomb *gombok, SDL_Event *event, int index, int meret) //Az adott indexu beviteli mezobe adatot ir be
{
    if(strlen(gombok[index].nev) < meret) //A megadott meretig lehet irni a mezobe
    {
        char betu[2]; betu[0] = event->text.text[0]; betu[1] = '\0';
        strcat(gombok[index].nev, betu);
        gombok[index].nevvegposX = (gombok[index].vegposX - gombok[index].nevkezdposX) / meret * strlen(gombok[index].nev); //Szovegdoboz atmeretezese
        gombok[index].nevvegposX += gombok[index].nevkezdposX;
    }
}

void AdatTorles(gomb *gombok, SDL_Event *event, int index, int meret) //Az adott indexu beviteli mezobe irt adatt torlese backspaceel
{
    if(event->key.keysym.sym == SDLK_BACKSPACE && strlen(gombok[index].nev) != 0)
    {
        gombok[index].nev[strlen(gombok[index].nev) - 1] = '\0';
        gombok[index].nevvegposX = (gombok[index].vegposX - gombok[index].nevkezdposX) / meret * strlen(gombok[index].nev);
        gombok[index].nevvegposX += gombok[index].nevkezdposX;
    }
}

//SEGED MUVELETEK

int KivalasztottSzoba(gomb *gombok, alakzat *alakzatok, char *szobaszam) //Megadja 0-25 ig az indexet a kivalasztott szobanak, a kivalasztasi lista tartomanya 0-25, valojaban a szoba gombjai 16-os indextol indulnak, ezert van +16 az indexben
{
    int i = 0;
    bool talalt = false;
    while(!talalt && i < 25)
    {
        if(alakzatok[8].kezdposX == gombok[i + 16].kezdposX - 5 && alakzatok[8].kezdposY == gombok[i + 16].kezdposY - 5 && alakzatok[8].vegposX == gombok[i + 16].vegposX + 5 && alakzatok[8].vegposY == gombok[i + 16].vegposY + 5) //Ugy szerzi meg a helyes indexet, hogy megnezi hol van a kijelolo teglalap
            talalt = true;
        i++;
    }
    i--;
    if(szobaszam != NULL) //Ha a szobaszam nem null pointer akkor a beadott karaktertombbe bemasolja a szobaszamot, ez foglalasnal szukseges
    strcpy(szobaszam, gombok[i + 16].nev);
    return i;
}

int Arszamlalas(gomb *gombok, ar *hotelar, hotelszobak szoba) //Kiszamolja, hogy mennyibe kerul a hotelszoba az adott napra
{
  int ev[2], ho[2], nap[2];
  sscanf(gombok[2].nev, "%4d.%2d.%2d", &ev[0], &ho[0], &nap[0]); //a ket datum beolvasasa ev, honap, nap int valtozokba
  sscanf(gombok[3].nev, "%4d.%2d.%2d", &ev[1], &ho[1], &nap[1]);
  int napok = Napszamlalas(ev, ho, nap); //kiszamolja, hogy mennyi napot maradtak a vendegek
  int teljesar = (hotelar->arak[szoba.ferohely - 1] + abs((szoba.klima - 2)) * hotelar->arak[4] + abs((szoba.terasz - 2)) * hotelar->arak[5]) * napok; //az arak tombbol kiveszi a megfelelo indexu ertekeket, s megszorozza a napok szamaval
  return teljesar;                                    //abszolutertek azert kell mert a 2-es jeloli, ha nincs a szobanak az adott szolgaltatasa, ekkor a szorzat erteke 0, 1-es jeloli ha van a szobanak szolgaltatsa, viszont ilyenkor kell az abszolutertek, mert minuszba megy at a kulonbseg erteke
}

int Napszamlalas(int *ev, int *ho, int *nap) //Megszamolja, hogy ket datum kozott mennyi nap telt el
{
    int honapnapok[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int szokoevek = ev[0];
    if(ho[0] <= 2) //szokoevek a kezdesi datumig
        szokoevek--;
    szokoevek = szokoevek / 4 - szokoevek / 100 + szokoevek / 400;
    long int kezdnap = ev[0] * 365 + nap[0]; //idoszamitas ota eltelt evek es napok
    for(int i = 0; i < ho[0] - 1; i++)
          kezdnap += honapnapok[i]; //honapok napjait adja hozza
    kezdnap += szokoevek; //mivel a szokoevek 366 naposak, igy a szokoevek szamat
    long int vegenap = ev[1] * 365 + nap[1]; //ugyanez a vege datum kiszamitasa
    szokoevek = ev[1];
    if(ho[1] <= 2)
    szokoevek--;
    szokoevek = szokoevek / 4 - szokoevek / 100 + szokoevek / 400;
    for(int i = 0; i < ho[1] - 1; i++)
    vegenap += honapnapok[i];
    vegenap += szokoevek;
    return vegenap - kezdnap;
}

void Listazas(gomb *gombok, szo *szavak, int index, char *szoveg) //Kiirja a foglalasi adatokat, az adott gomb helyere, vagy az adott szo helyere
{
    if(gombok != NULL) //Ha a gomb nem NULL pointer akkor gomb szavat kell kiirni, ezert a gomb strukturaval dolgozik
    {
    gombok[index].nev[0] = '\0';
    strcpy(gombok[index].nev, szoveg);
    gombok[index].nevvegposX = (gombok[index].vegposX - gombok[index].nevkezdposX) / 30 * strlen(gombok[index].nev);
    gombok[index].nevvegposX += gombok[index].nevkezdposX;
    }
    else //Maskulonben egy egyszeru szot kell kiirni, ezert a szo strukturaval dolgozik
    {
      szavak[index].nev[0] = '\0';
      strcpy(szavak[index].nev, szoveg);
    }
}

bool szuro(hotelszobak *szoba, int ferohelyszuro, int klimaszuro, int teraszuro, int szuroszam) //Ha egy hotelszoba teljseiti a kriteriumokat, akkor igazat ad vissza
{
    int db = 0;
    if(ferohelyszuro != 0 && szoba->ferohely == ferohelyszuro)
        db++;
    if(klimaszuro != 0 && szoba->klima == klimaszuro)
        db++;
    if(teraszuro != 0 && szoba->terasz == teraszuro)
        db++;
    if(db == szuroszam)
        return true;
    return false;
}

bool Formatum(gomb *gombok, int index) //Igazad ad vissza ha a datum formatuma, es ervenyessege jo
{
    int ev, honap, nap;
    int honapnapok[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool csakszam = true;
    int j = 0;
    while(gombok[index].nev[j] != '\0' && csakszam)
    {
        if((gombok[index].nev[j] < '0' || gombok[index].nev[j] > '9') && gombok[index].nev[j] != '.')
           csakszam = false;
        j++;
    }
    if(csakszam)
    {
    sscanf(gombok[index].nev,"%4d.%2d.%2d", &ev, &honap, &nap);
    if((honap >= 1 && honap <= 12) && nap >= 1 && nap <= 31)
    {
    bool szokoev = (ev%400==0 || (ev%100!=0 && ev%4==0));
    if((szokoev && honap == 2 && nap <= 29) || (!szokoev && honap == 2 && nap <= 28) || honap != 2 && nap <= honapnapok[honap - 1])
    {
        return true;
    }
    }
    }
    return false;
}

void ListaInicializalas(ListazandoFoglalasok *listazandofoglalasok) //Feltolti NULL ertekkel azt a pointer tombot, ami a kilistazando foglalasok pointereit tartalmazza
{
    for(int i = 0; i < 25; i++)
        listazandofoglalasok->lista[i] = NULL;
}
