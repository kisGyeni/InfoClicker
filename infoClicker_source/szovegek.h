#ifndef SZOVEGEK_H_INCLUDED
#define SZOVEGEK_H_INCLUDED

/*
* Ebben a fájlban vannak azok a függvények, melyek a szövegek képernyõre írását teszik lehetõvé.
* Az itt található függvények elengedhetetlenek a program szövegkiírásaihoz.
* (pl a kiírandó szám hármas tagolása, nagyságrendekre bontása, kiírandó szövegek igazítása ées a képernyõre renderelése mind itt van definiálva)
*/

#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

typedef enum align {center, left, right} align; // a szövegki függvény utolsó paramétere, ettõl föggõen a szöveg középra, balra vagy jobbra igazítva jelenik meg.

typedef struct sz // szöveg struktúra, mely a kiírni kívánt szöveg tulajdonságait tárolja
{
    TTF_Font *betutipus;
    SDL_Color szin;
    char szoveg[100]; //ezzel kene meg valamit kezdeni
    SDL_Rect hely;
} sz;

typedef struct Lebeges
{
    sz szoveg;
    int timer;
    struct Lebeges *kovetkezo;
} lebeges;

void szovegki(SDL_Renderer *renderer, SDL_Surface *felirat, SDL_Texture *felirat_t, sz szoveg, align igazitas); // sz típusú struktúrát renderel a képernyõre

sz feltolt(int red, int green, int blue, TTF_Font * betutipus, int x, int y, int h, int w); // egy sz típusú struktúrát tölt föl

void csoportosit(unsigned long long int szam, char * tomb); // a kiírandó számot megfelelõen csoportosítja, mielõtt az kiírásra kerülne

char * hogyirki (unsigned long long int kredit, double bgkredit, char * szoveg, char * prefix, char * suffix, bool tortkent, bool* nocount); // a csoportosít függvény segítségével véglegesíti a kiírandó szam + esetleges szöveg kombinációt

lebeges* klikklista (lebeges ** elso);

void elsoelemtorol(lebeges ** elso);

#endif // SZOVEGEK_H_INCLUDED
