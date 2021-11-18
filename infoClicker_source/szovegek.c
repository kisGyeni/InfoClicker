#include "szovegek.h"
#include "debugmalloc.h"

// TTF_Font *font,char szoveg[], SDL_Color szin, SDL_Rect hova,
void szovegki(SDL_Renderer *renderer, SDL_Surface *felirat, SDL_Texture *felirat_t, sz szoveg, align igazitas)
{
    felirat = TTF_RenderUTF8_Blended(szoveg.betutipus, szoveg.szoveg, szoveg.szin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    if (igazitas == center)
        szoveg.hely.x = (szoveg.hely.x - felirat->w) / 2; //hova.x amit kap, a szoveg kozepe
    else if (igazitas == right)
        szoveg.hely.x = szoveg.hely.x - felirat->w;
    szoveg.hely.w = felirat->w;
    szoveg.hely.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &szoveg.hely);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

sz feltolt(int red, int green, int blue, TTF_Font * betutipus, int x, int y, int h, int w)
{
    sz toltott;
    toltott.szin.r = red;
    toltott.szin.g = green;
    toltott.szin.b = blue;
    toltott.szin.a = 255;
    toltott.betutipus = betutipus;
    toltott.hely.x = x;
    toltott.hely.y = y;
    toltott.hely.h = h;
    toltott.hely.w = w;
    strcpy(toltott.szoveg, "");
    return toltott;
}

void csoportosit(unsigned long long int szam, char * tomb)
{
    tomb[0] = '\0';
    if (szam < 1000)
    {
        char temp[5];
        sprintf(temp, "%I64u", szam);
        strcat(tomb, temp);
        return;
    }
    csoportosit(szam/1000, tomb);
    char temp2[6];
    sprintf(temp2, ",%03d", szam%1000);
    strcat(tomb, temp2);
    return;
}


char * hogyirki (unsigned long long int kredit, double bgkredit, char * szoveg, char * prefix, char * suffix, bool tortkent, bool* nocount)
{
    char temp[25];
    strcpy(szoveg, prefix);
    if (bgkredit < 0);
    else
    {

                if (kredit >= 10000000000000000000)
                {
                    sprintf(temp, "Az összes");
                    *nocount = true;
                }
                else if (kredit >= 1000000000000000000)
                {
                    sprintf(temp, "%3.2f trillió", bgkredit/1000000000000000000);
                }
                else if (kredit >= 1000000000000000)
                {
                    sprintf(temp, "%3.2f billiárd", bgkredit/1000000000000000);
                }
                else if (kredit >= 1000000000000)
                {
                    sprintf(temp, "%3.2f billió", bgkredit/1000000000000);
                }
                else if (kredit >= 1000000000)
                {
                    sprintf(temp, "%3.2f milliárd", bgkredit/1000000000);
                }
                else if (kredit >= 1000000)
                {
                    sprintf(temp, "%3.2f millió", bgkredit/1000000);
                }
                else if (bgkredit == 0)
                    sprintf(temp, "0");
                //tortkent
                else if (bgkredit < 1000 && tortkent)
                {
                    sprintf(temp, "%3.1f", bgkredit);
                }
                else
                {
                    csoportosit(bgkredit, temp);
                    sprintf(temp, "%s", temp);
                }
                strcat(szoveg, temp);
    }

            strcat(szoveg, suffix);
            return szoveg;
}

lebeges* klikklista (lebeges ** elso)
{
    if (*elso == NULL)
    {
        *elso = (lebeges*) malloc(sizeof(lebeges));
        (*elso)->timer = 0;
        (*elso)->kovetkezo = NULL;
        return *elso;
    }
    lebeges *mozgo = *elso;
    while (mozgo->kovetkezo != NULL)
    {
        mozgo = mozgo->kovetkezo;
    }
    mozgo->kovetkezo = (lebeges*) malloc(sizeof(lebeges));
    mozgo = mozgo->kovetkezo;
    mozgo->timer = 0;
    mozgo->kovetkezo = NULL;
    return mozgo;
}

void elsoelemtorol(lebeges ** elso)
{
    lebeges * regi;
    regi = *elso;
    *elso = (*elso)->kovetkezo;
    free(regi);
}
