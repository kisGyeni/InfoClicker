#ifndef FAJLOK_H_INCLUDED
#define FAJLOK_H_INCLUDED

/*
* A fájlkezelést végző függvények, itt találhatók. Ezek feladata többek között
* a játékállás mentése, indításkor betöltése, konstans adatok fájlból olvasása, rendszerezése.
*/

#include "gombok.h"

void fajlnev(int i, char * fajlnev); // betöltendő fájl nevének előállítása egy stringbe, hogy később iteratívan meg lehessen nyitni őket

void vonaltalanit (char * szoveg); // beolvasott mondatokban szóközök kezelése.

unsigned long long int mostaniar(epuletek * epulet, int x); // kikeresi adott fejlesztés adatait tároló fájlból vásárlás után a következő fejlesztés adatait

void jatekallas_mentese(double bgkredit, int gombdb, epuletek * epulet); //játékállás fájlba írása

#endif // FAJLOK_H_INCLUDED
