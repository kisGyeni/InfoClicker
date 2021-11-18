#ifndef GOMBOK_H_INCLUDED
#define GOMBOK_H_INCLUDED

/*
* Ebben a f�jlban vannak a gombok m�k�d�s�hez sz�ks�ges f�ggv�nyek
* Ezek a f�ggv�nyek akkor h�v�dnak meg, amikor a gombok rajzol�s�val,
* �s azok interakci�j�val akarunk foglalkozni.
* (pl: a gomb amin a kurzor van legyen m�smilyen mint a t�bbi, gombokra kattint�s detekt�l�sa)
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

double dist(SDL_Point a, SDL_Point b); // 2 pont t�vols�ga

bool baglyon(SDL_Point kurzor, SDL_DisplayMode DM); // a kurzor a baglyon van-e

bool gombon(SDL_Point kurzor, SDL_Rect gomb, SDL_DisplayMode DM); // a kurzor egy adott gombon van-e

bool gomboknal(SDL_Point kurzor, SDL_Rect gombok, int gombdb, SDL_DisplayMode DM); // a kurzor a feloldott gombok valamelyik�n van-e

double sumkps(epuletek * epulet); // a megv�s�rolt �p�letek sz�m�nak �s fejleszt�seknek sz�m�b�l kisz�molja a m�sodpercenk�nti kredittermel�st

#endif // GOMBOK_H_INCLUDED
