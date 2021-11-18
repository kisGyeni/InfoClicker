#ifndef RAJZOL_H_INCLUDED
#define RAJZOL_H_INCLUDED

/*
* Ez a f�jl az k�perny�re rajzol�shoz haszn�lt seg�df�ggv�nyeket t�rolja.
* (pl: k�pfel�let l�trehoz�sa, USEREVENTek gener�l�sa, nem statikus megjelen�t�s kezel�se)
*/

#include <SDL2/SDL.h>

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer); // ablak l�trehoz�sa

Uint32 idozit(Uint32 ms, void *param); // id�z�t� be�ll�t�sa

void forgat (double *i, int speed); // a bagoly k�r�li "golw-effekt" forgat�sa

void fenyrender(SDL_Renderer *renderer, SDL_Texture *gui,  SDL_Rect feny, SDL_Rect fenyhely, int speed, SDL_DisplayMode DM); // a bagoly k�r�li "glow-effekt" kirajzol�sa

int minimum (int a, int b); // Visszaadja k�t eg�sz sz�m k�z�l a kisebbet

#endif // RAJZOL_H_INCLUDED
