#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "fajladatkezeles.h"
#include "megjelenites.h"
#include "felhasznalomenu.h"
#include "debugmalloc.h"

////FAJLMUVELETEK

bool FajlLetezik(char *fajlnev) //Megnezi, hogy letezik e az adott fajl, ezt az elso elinditaskor hasznalja a program, hogy eldöntse regisztralni kell, vagy belepni
{
    FILE *fajl = fopen(fajlnev, "r");
    if(fajl != NULL)
    {
        fclose(fajl);
        return true;
    }
    return false;
}

bool FajlbaIras(char *fajlnev, int meret, char adatok[][31]) //Fajlba iras, ezt az elso elinditaskor hasznalja a program, hogy binaris fajlba irja a felhasznalonevet es jelszot, valamint
{                                                            //az arat tartalmazo szovegfajlt tolti fel az alapertelmezett adatokkal
    FILE *fajl = fopen(fajlnev, "w");
    if(fajl != NULL)
    {
        for(int i = 0; i < meret; i++)
        {
        fprintf(fajl, adatok[i]);
        fprintf(fajl, "\n");
        }
        fclose(fajl);
        return true;
    }
    return false;
}

bool FajbolBeolvasas(char *fajlnev, int meret, char adatok[][31]) //Fajbol beolvasas, a felhasznalo adatokat, valamint az arakat tartalmazo szovegfajlt olvassa be
{
    FILE *fajl = fopen(fajlnev, "r");
    if(fajl != NULL)
    {
        for(int i = 0; i < meret; i++)
        {
            fgets(adatok[i], 31, fajl);
            adatok[i][strlen(adatok[i]) - 1] = '\0'; //uj sor kitorlese
        }
        return true;
    }
    return false;
}

void FoglalasIras(char *fajlnev, adatoklista *lista) //Foglalasi adatok fajlba irasa
{
    FILE *fajl = fopen(fajlnev, "w");
    adatoklista *leptetes;
    for(leptetes = lista; leptetes != NULL; leptetes = leptetes->kov)
    {
        char datum[2][11];
        DatumvaltasVissza(leptetes->foglalasidatum, datum[0]);
        char tartomany[12]; tartomany[0] = '#'; strcat(tartomany, datum[0]);
        fputs(tartomany, fajl);
        fputc('\n', fajl);
        int index = 0;
        char sor[130]; //A program altal maximum 129 + lezarao 0 karakter kerulhet egy sorba
        while((leptetes->szemelyek[index].startdatum != -1 && leptetes->szemelyek[index].stopdatum != -1) && index != leptetes->tmeret) //Az ures tombbelemekig megy a ciklus, vagy ha pont annyi elem van a tombbe amekkora a tomb, akkor a meretig
        {
            DatumvaltasVissza(leptetes->szemelyek[index].startdatum, datum[0]);
            DatumvaltasVissza(leptetes->szemelyek[index].stopdatum, datum[1]);;
            sprintf(sor, "%s|%s|%s|%s|%s|%d|%s", leptetes->szemelyek[index].nev, leptetes->szemelyek[index].tel, leptetes->szemelyek[index].email, datum[0], datum[1], leptetes->szemelyek[index].ar, leptetes->szemelyek[index].szobaszam);
            fputs(sor, fajl);
            fputc('\n', fajl);
            index++;
        }
        tartomany[1] = '\0';
        fputs(tartomany, fajl);
        fputc('\n', fajl);
    }
    fclose(fajl);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Figyelem", "Sikeres Mentes!", NULL);
}

bool FoglalasBeolvas(char *fajlnev, adatoklista **lista) //A foglalasi adatok beolvasasa, a fuggveny amint eler egy foglalasi datumot, megnezi, hogy alatta hany vendeg adatai szerepelnek,
{                                                        //mivel a vendegek adatai soronkent szerepelnek, ezert azt nezi, hogy mennyi sor van a kovetkezo foglalasi datumig, letrehoz egy
    FILE *fajl = fopen(fajlnev, "r");                    //lancolt listat, eltarolja a foglalasi datumot, amely szerint a vendegek csoportositva vannak, majd letrehoz egy dinamikus tombot, mely a vendegek
                                                         //tovabbi adatait tarolja
    char sor[130];   //A program altal maximum 129 + lezarao 0 karakter kerulhet egy sorba
    int sorok;
    int meret = 130;
    szemelyadatok *foglalas;
    if(fajl != NULL)
    {
        char c = fgetc(fajl);
        if(c != EOF)
            fseek(fajl, -1, SEEK_CUR);
        while(!feof(fajl))
        {
            fgets(sor, meret, fajl);
            sor[strlen(sor) - 1] = '\0'; //uj sor kitorlese
            if(strlen(sor) > 1) //ha nagyobb mint 0, akkor biztosan vendeg, foglalasi datum
            {
                if(sor[0] == '#') //hashtag jelzi a foglalasi datumot
                {
                    sorok = Sorosszeg(fajl); //vendeg adatokat tartalmazo sorok megszamlalasa
                    int sorokmeret = sorok + 5; //+5 vendegnek hely, hogy uj foglalasnal ne kelljen egybol ujrameretezni a tombot
                    *lista = Beszuras(*lista, Datumatvaltas(sor, true), sorokmeret);
                    foglalas = (szemelyadatok*) malloc(sorokmeret * sizeof(szemelyadatok));
                    AdatInicializalas(foglalas, sorokmeret, 0); //ures ertekkel valo feltoltes startdatum, stopdatum = -1
                    (*lista)->szemelyek = foglalas;
                    sorok = 0;
                }
                else
                {
                    Adatkitoltes(foglalas, sor, sorok); //vendeg sorok adatszetvalasztasa, s betoltese a lancolt listaba
                    sorok++;
                }
            }
        }
        fclose(fajl);
        return true;
    }
    return false;
}

int Sorosszeg(FILE *fajl) //Megszamolja, hogy egy foglalasi datumhoz hany vendeg sor tartozik
{
    int sum = 0;
    int eltolas = 0;
    char c;
    while(c != '#')
    {
        eltolas++;
        c = fgetc(fajl);
        if(c == '\n')
            sum++;
    }
    eltolas += sum;
    fseek(fajl, -eltolas, SEEK_CUR); //fajl pozicio visszatekerese, hogy ujbol be tudjuk olvasni a sorokat a dinamikus tombbe
    return sum;
}

void Adatkitoltes(szemelyadatok *foglalas, char *sor, int index) //A foglalasi adatok tordelese, s betoltese a lancolt listaban levo dinamikus tombbe
{
    char *ertek;
    ertek = strtok(sor, "|");
    strcpy(foglalas[index].nev, ertek);
    ertek = strtok(NULL, "|");
    strcpy(foglalas[index].tel, ertek);
    ertek = strtok(NULL, "|");
    strcpy(foglalas[index].email, ertek);
    ertek = strtok(NULL, "|");
    foglalas[index].startdatum = Datumatvaltas(ertek, false);
    ertek = strtok(NULL, "|");
    foglalas[index].stopdatum = Datumatvaltas(ertek, false);
    ertek = strtok(NULL, "|");
    foglalas[index].ar = atoi(ertek);
    ertek = strtok(NULL, "|");
    strcpy(foglalas[index].szobaszam, ertek);
}

void AdatInicializalas(szemelyadatok *foglalas, int meret, int kezdoertek) //A dinamikus tombben letrehozott plusz helyeket ures ertekkel tolti fel, az ures erteket a startdatum, stopdatum -1 erteke jelzi
{
    for(int i = kezdoertek; i < meret; i++)
    {
        foglalas[i].startdatum = -1;
        foglalas[i].stopdatum = -1;
    }
}

////SEGEDMUVELETEK

int Datumatvaltas(char *datum, bool datumfajta) //sztring datum atalakitasa int datumma
{
    int ev, honap, nap;
    if(datumfajta)
    sscanf(datum,"#%d.%d.%d", &ev, &honap, &nap);
    else
    sscanf(datum,"%d.%d.%d", &ev, &honap, &nap);
    int ertek = (ev * 100 + honap) * 100 + nap;
    return ertek;
}

void DatumvaltasVissza(int datum, char *atvaltas) //int datum atalakitasa sztring datumma
{
    sprintf(atvaltas, "%d", datum);
    int eltolas = 2;
    for(int i = 10; i > 3; i--)
    {
        atvaltas[i] = atvaltas[i - eltolas];
        if(i == 7 || i == 4)
        {
            eltolas--;
            atvaltas[i] = '.';
        }
    }
}

////ADATSTRUKTURA MUVELETEK

adatoklista *Beszuras(adatoklista *start, int foglalasidatum, int meret) //uj elem beszurasa a lancolt listaba
{
    adatoklista *uj;
    uj = (adatoklista*) malloc(sizeof(adatoklista));
    uj->foglalasidatum = foglalasidatum;
    uj->tmeret = meret;
    uj->kov = start;
    return uj;
}

adatoklista *ReszlegesFelszabadit(adatoklista *lista, adatoklista *keresendo) //Cim szerinti visszateres, ha a lista kezdocime megvaltozna, csak a megadott elemet szabaditja fel
{
  adatoklista *start = lista;
  adatoklista *leptetes = lista;
  adatoklista *lemarado = NULL;
  while(leptetes != NULL && leptetes != keresendo)
  {
      lemarado = leptetes;
      leptetes = leptetes->kov;
  }
  if(leptetes == NULL)
  {

  }
  else if(lemarado == NULL)
  {
      start = leptetes->kov;
      free(leptetes->szemelyek); //A lancolt listan belul levo dinamikusan foglalt sruct tomb felszabaditasa
      free(leptetes);
  }
  else
  {
      lemarado->kov = leptetes->kov;
      free(leptetes->szemelyek);
      free(leptetes);
  }
  return start;
}

adatoklista *TeljesFelszabadit(adatoklista *lista) //Program vegen hasznalt fuggveny, a teljes adatstrukturat felszabaditja
{
    adatoklista *masolas;
    while(lista != NULL)
    {
        masolas = lista->kov;
        free(lista->szemelyek);
        free(lista);
        lista = masolas;
    }
    return lista;
}

void Kereses(gomb *gombok, ListazandoFoglalasok *listazandofoglalasok, adatoklista *lista, int *datumertek)
{
    adatoklista *start;
    for(start = lista; start != NULL; start = start->kov)
    {
        for(int i = 0; i < start->tmeret; i++)
        {
            if(datumertek[0] < start->szemelyek[i].stopdatum && start->szemelyek[i].startdatum < datumertek[1])
            {
                int j = 16; bool talalt = false;
                while(!talalt && j < 41)
                {
                    if(strcmp(start->szemelyek[i].szobaszam, gombok[j].nev) == 0)
                    {
                        listazandofoglalasok->lista[j - 16] = start;
                        listazandofoglalasok->foglalasindex[j - 16] = i;
                        talalt = true;
                    }
                    j++;
                }
            }
        }
    }
}
bool TorlesUjraMeretezes(ListazandoFoglalasok *listazandofoglalsok, int index) //Igazat ad 10 szabad hely, eseten, vagyis akkor ha csokkenteni kell a memoriat kihasznalatlansag miatt
{
    int ureshelyek; //9 ig kell szamoljon, mivel a mostani torles hivas lesz a 10-dik ures memoria terulet
    for(int i = listazandofoglalsok->lista[index]->tmeret - 1; i > 1; i--) //1-ig csokkent, mivel ha csak 1 nem ures erteku elem van csak, akkor nem kell ujrameretezni, hiszen akkor felszabaditjuk a teljes memoriat
    {
        if(listazandofoglalsok->lista[index]->szemelyek[i].startdatum == -1 && listazandofoglalsok->lista[index]->szemelyek[i].stopdatum == -1)
        {
            ureshelyek++;
            if(ureshelyek == 9)
                return true;
        }
    }
    return false;
}

void ToroltErtekBeallitas(ListazandoFoglalasok *listazandofoglalsok, int index)
{
    listazandofoglalsok->lista[index]->szemelyek[listazandofoglalsok->foglalasindex[index]].startdatum = -1; //jelzo ertekek beallitasa az invalid, ures ertekre
    listazandofoglalsok->lista[index]->szemelyek[listazandofoglalsok->foglalasindex[index]].stopdatum = -1;
    int j = listazandofoglalsok->foglalasindex[index];
    while(listazandofoglalsok->lista[index]->szemelyek[j + 1].startdatum != -1 && listazandofoglalsok->lista[index]->szemelyek[j + 1].stopdatum != -1) //ures ertek eltolasa hatra, az ures tombelemekhez
    {
       szemelyadatok masolas = listazandofoglalsok->lista[index]->szemelyek[j];
       listazandofoglalsok->lista[index]->szemelyek[j] = listazandofoglalsok->lista[index]->szemelyek[j + 1];
       listazandofoglalsok->lista[index]->szemelyek[j + 1] = masolas;
       j++;
    }
}

bool FoglalasTorles(ListazandoFoglalasok *listazandofoglalsok, adatoklista **lista, int index)
{
    if(TorlesUjraMeretezes(listazandofoglalsok, index)) //Ha tul sok nem hasznalt hely van felszabaditas, es a torolt tomb elem ures ertekenek beallitasa
    {
        int ujmeret = listazandofoglalsok->lista[index]->tmeret - 5;
        szemelyadatok *meretezes = realloc(listazandofoglalsok->lista[index]->szemelyek, ujmeret * sizeof(szemelyadatok));
        if(meretezes == NULL)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Figyelem", "Nem sikerult a torles!", NULL);
            return false;
        }
        else
        {
            listazandofoglalsok->lista[index]->szemelyek = meretezes;
            listazandofoglalsok->lista[index]->tmeret = ujmeret;
            ToroltErtekBeallitas(listazandofoglalsok, index);
        }
    }
    else //Ha nincs tul sok nem hasznalt hely, torolt elem ures ertekenek beallitasa, vagy utolso elem eseten a memoria terulet felszabaditasa, listaelem torlese
    {
        if(listazandofoglalsok->lista[index]->szemelyek[1].startdatum == -1 && listazandofoglalsok->lista[index]->szemelyek[1].stopdatum == -1)
                *lista = ReszlegesFelszabadit(*lista, listazandofoglalsok->lista[index]);
        else
        ToroltErtekBeallitas(listazandofoglalsok, index);
    }
    return true;
}

bool FoglalasUjraMeretezes(adatoklista *kivalasztottelem) //Ellenorzi, hogy van e plussz lefoglalt memoria hely az aktualis foglalasi adatoknak
{
    if(kivalasztottelem->szemelyek[kivalasztottelem->tmeret - 1].startdatum == -1 && kivalasztottelem->szemelyek[kivalasztottelem->tmeret - 1].stopdatum == -1)
        return false;
    return true;
}

adatoklista *FoglalasListaLetrehozas(adatoklista *lista, int datum) //Ha talal listaelemet amely tartalmazza a foglalasi datumot, akkor visszadja annak az elemnek a pointeret, ha nem talal NULL pointert ad
{
    adatoklista *leptetes;
    for(leptetes = lista; leptetes != NULL; leptetes = leptetes->kov)
    {
        if(leptetes->foglalasidatum == datum)
            return leptetes;
    }
    return NULL;
}

void FoglalasBeiras(adatoklista *kivalasztottelem, int index, char adatok[][31], int datumok[3], int teljesar) //Foglalasi adatok beirasa a memoriaba
{
    kivalasztottelem->foglalasidatum = datumok[0];
    kivalasztottelem->szemelyek[index].startdatum = datumok[1];
    kivalasztottelem->szemelyek[index].stopdatum = datumok[2];
    kivalasztottelem->szemelyek[index].ar = teljesar;
    strcpy(kivalasztottelem->szemelyek[index].nev, adatok[0]);
    strcpy(kivalasztottelem->szemelyek[index].tel, adatok[1]);
    strcpy(kivalasztottelem->szemelyek[index].email, adatok[2]);
    strcpy(kivalasztottelem->szemelyek[index].szobaszam, adatok[3]);
}

int ElemIndex(adatoklista *kivalasztottelem) //Megtalalja a legelso szabad memoria hely indexet
{
    int index = 0;
    while(kivalasztottelem->szemelyek[index].startdatum != -1 && kivalasztottelem->szemelyek[index].stopdatum != -1)
        index++;
    return index;
}

bool FoglalasFelvetel(ListazandoFoglalasok *listazandofoglalasok, adatoklista **lista, int index, char adatok[][31], int datumok[3], int ar)
{
    adatoklista *foglalas = FoglalasListaLetrehozas(*lista, datumok[0]);
    if(foglalas != NULL) //Ha talal listaelemet, akkor vagy ujrameretezi a szemelyek tombot, vagy szabad hely eseten egybol kitolti az adatokkal
    {
         if(FoglalasUjraMeretezes(foglalas))
         {
             int ujmeret = foglalas->tmeret + 5;
             szemelyadatok *meretezes = realloc(foglalas->szemelyek, ujmeret * sizeof(szemelyadatok));
             if(meretezes == NULL)
             {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Figyelem", "Nem sikerult a foglalas!", NULL);
                return false;
             }
             else
             {
                 foglalas->szemelyek = meretezes;
                 AdatInicializalas(foglalas->szemelyek, ujmeret, foglalas->tmeret);
                 listazandofoglalasok->lista[index] = foglalas;
                 listazandofoglalasok->foglalasindex[index] = foglalas->tmeret; //A meret meg nem valtozott, az uj elem indexenek megfelel
                 FoglalasBeiras(foglalas, foglalas->tmeret, adatok, datumok, ar);
                 foglalas->tmeret = ujmeret;
             }
         }
         else
         {
                listazandofoglalasok->lista[index] = foglalas;
                int mutato = ElemIndex(foglalas);
                listazandofoglalasok->foglalasindex[index] = mutato;
                FoglalasBeiras(foglalas, mutato, adatok, datumok, ar);
         }
    }
    else
    {
        *lista = Beszuras(*lista, datumok[0], 6);
        szemelyadatok *foglalas;
        foglalas = (szemelyadatok*) malloc(6 * sizeof(szemelyadatok));
        AdatInicializalas(foglalas, 6, 0);
        (*lista)->szemelyek = foglalas;
        listazandofoglalasok->lista[index] = *lista;
        listazandofoglalasok->foglalasindex[index] = 0; //Uj lista elem, ezert nem kell indexet szamolni
        FoglalasBeiras(*lista, 0, adatok, datumok, ar);
    }
    return true;
}

////ALAPERTELMEZETT ERTEK

ar alapertelmezettar()
{
    ar alapertelmezett = {.arak[0] = 20000, .arak[1] = 25000, .arak[2] = 30000, .arak[3] = 35000, .arak[4] = 1000, .arak[5] = 2000};
    return alapertelmezett;
}
