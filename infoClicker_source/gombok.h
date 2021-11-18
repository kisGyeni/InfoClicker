#ifndef GOMBOK_H_INCLUDED
#define GOMBOK_H_INCLUDED

/*
* Ebben a fájlban vannak a gombok mûködéséhez szükséges függvények
* Ezek a függvények akkor hívódnak meg, amikor a gombok rajzolásával,
* és azok interakciójával akarunk foglalkozni.
* (pl: a gomb amin a kurzor van legyen másmilyen mint a többi, gombokra kattintás detektálása)
*/

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

typedef struct epuletek
{
    char nev[11];
    unsigned long long int ar;
    double kps;
    int db;
    char leiras[64];
    int szorzo;
    unsigned long long int fejlesztesar;
    int fejlesztesdb;
    char fejlesztesnev[64];
    char fejlesztesleiras[100];
    char fejlesztesmitcsinal[100];
} epuletek;

double dist(SDL_Point a, SDL_Point b); // 2 pont távolsága

bool baglyon(SDL_Point kurzor, SDL_DisplayMode DM); // a kurzor a baglyon van-e

bool gombon(SDL_Point kurzor, SDL_Rect gomb, SDL_DisplayMode DM); // a kurzor egy adott gombon van-e

bool gomboknal(SDL_Point kurzor, SDL_Rect gombok, int gombdb, SDL_DisplayMode DM); // a kurzor a feloldott gombok valamelyikén van-e

double sumkps(epuletek * epulet); // a megvásárolt épületek számának és fejlesztéseknek számából kiszámolja a másodpercenkénti kredittermelést

#endif // GOMBOK_H_INCLUDED
