#include "rajzol.h"
#include "debugmalloc.h"

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, SDL_WINDOW_FULLSCREEN_DESKTOP); //SDL_WINDOW_FULLSCREEN_DESKTOP
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

Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

void forgat (double *i, int speed)
{
    if(*i < 360) *i+=20.0/(1000/speed);
    else *i = 0;
}

void fenyrender(SDL_Renderer *renderer, SDL_Texture *gui,  SDL_Rect feny, SDL_Rect fenyhely, int speed, SDL_DisplayMode DM)
{
            static double r = 0;
            SDL_Point fenykozep = {DM.w/9.143, DM.h/5.294};
            SDL_RenderCopyEx(renderer, gui, &feny, &fenyhely, r, &fenykozep, 0);
            SDL_RenderCopyEx(renderer, gui, &feny, &fenyhely, -r, &fenykozep, 0);
            forgat(&r, speed);
}

int minimum (int a, int b)
{
    return (a < b) ? a : b;
}
