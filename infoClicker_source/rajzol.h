#ifndef RAJZOL_H_INCLUDED
#define RAJZOL_H_INCLUDED

/*
* Ez a fájl az képernyõre rajzoláshoz használt segédfüggvényeket tárolja.
* (pl: képfelület létrehozása, USEREVENTek generálása, nem statikus megjelenítés kezelése)
*/

#include <SDL2/SDL.h>

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer); // ablak létrehozása

Uint32 idozit(Uint32 ms, void *param); // idõzítõ beállítása

void forgat (double *i, int speed); // a bagoly körüli "golw-effekt" forgatása

void fenyrender(SDL_Renderer *renderer, SDL_Texture *gui,  SDL_Rect feny, SDL_Rect fenyhely, int speed, SDL_DisplayMode DM); // a bagoly körüli "glow-effekt" kirajzolása

int minimum (int a, int b); // Visszaadja két egész szám közül a kisebbet

#endif // RAJZOL_H_INCLUDED
