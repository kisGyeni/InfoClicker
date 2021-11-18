#include "gombok.h"
#include "debugmalloc.h"

double dist(SDL_Point a, SDL_Point b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+0.0);
}

bool baglyon(SDL_Point kurzor, SDL_DisplayMode DM)
{
    SDL_Point kozep = {DM.w/8.533, DM.h/2.16}; //itt a madar kozepe
    return (dist(kurzor, kozep) < DM.h/9);
}

bool gombon(SDL_Point kurzor, SDL_Rect gomb, SDL_DisplayMode DM)
{
    if (kurzor.x > gomb.x+DM.w/192.0 && kurzor.x < gomb.x+gomb.w-DM.w/192.0 && kurzor.y > gomb.y && kurzor.y < gomb.y+gomb.h)
    {
            return true;
    }
    return false;


}

bool gomboknal(SDL_Point kurzor, SDL_Rect gombok, int gombdb, SDL_DisplayMode DM)
{
    gombok.h *= gombdb;
    return (kurzor.x > gombok.x+DM.w/192.0 && kurzor.x < gombok.x+gombok.w-DM.w/192.0 && kurzor.y > gombok.y && kurzor.y < gombok.y+gombok.h);
}

double sumkps(epuletek * epulet)
{
    double sum = 0;
    for (int i = 0; i < 10; i++)
    {
        sum += epulet[i].db*epulet[i].kps*epulet[i].szorzo;
    }
    return sum;
}
