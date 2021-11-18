#include "rajzol.h"
#include "szovegek.h"
#include "gombok.h"
#include "fajlok.h"

#include "debugmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>


int main(int argc, char *argv[]) {
    /* ablak létrehozása, függvénykönyvtárakk inicializálása */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("InfoCClicker", 0, 0, &window, &renderer);
    TTF_Init();

    /* feliratok letrehozasa */
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);


    /* váltotók és konstansok definiálása */
    unsigned long long int kredit;                          // kredit = kiírandó kredit
    int gombdb = 2, meddig, speed = 60, nehamentsen = 1;      //gombdb = megjelenítendő gombok száma, meddig = kirajzolandó gombok száma (gombdb > 10 esetén az overflowtól véd), speed = render tickspeed
    double bgkredit = 0, kps = 0;                           //bgkredit = törteket is tárolni képes számláló, kps = kredits per second
    bool quit = false, gombrakatt = false, upgraderekatt = false, nocount = false, lekatt = false, torol = false;     //quit = kilepes, gombrakatt = a gomboknál történt-e a kattintás
    lebeges * kattszoveg = NULL;
    char mentszoveg[8] = "Mentés";

    SDL_Texture *binary = IMG_LoadTexture(renderer, "resources/binary.png");
    SDL_Texture *darkbinary = IMG_LoadTexture(renderer, "resources/binarybw.png");

    SDL_Texture *fa = IMG_LoadTexture(renderer, "resources/fa.jpg");
    SDL_Texture *gui = IMG_LoadTexture(renderer, "resources/GUI.png");
    SDL_Texture *hatter = IMG_LoadTexture(renderer, "resources/hatter.jpg");
    SDL_Rect bg = { 0, 0, DM.w, DM.h  };

    epuletek epulet[10];

    FILE* mentes;
    mentes = fopen("save.txt", "rt");
        if (mentes == NULL)
        {
            for (int i = 0; i < 10; i++)
            {
                epulet[i].db = 0;
                epulet[i].szorzo = 1;
                epulet[i].fejlesztesdb = 0;
            }
        }
        else
        {
            fscanf(mentes, "%lf", &bgkredit);
            fscanf(mentes, "%d", &gombdb);
            for (int i = 0; i < 10; i++)
            {
                fscanf(mentes, "%d", &epulet[i].db);
                fscanf(mentes, "%d", &epulet[i].szorzo);
                fscanf(mentes, "%d", &epulet[i].fejlesztesdb);
            }
        }
    fclose(mentes);
    FILE* gombadatok;
    gombadatok = fopen("resources/gombadatok.txt", "rt");
        if (gombadatok == NULL) return 1;

    for (int i = 0; i < 10; i++)
    {
        fscanf(gombadatok, "%s", &epulet[i].nev);
        fscanf(gombadatok, "%llu", &epulet[i].ar);
        epulet[i].ar = ceil(epulet[i].ar * pow(1.15, epulet[i].db));
        fscanf(gombadatok, "%lf", &epulet[i].kps);
        fscanf(gombadatok, "%s", &epulet[i].leiras);
        vonaltalanit(epulet[i].leiras);
    }
    fclose(gombadatok);


    for (int i = 0; i < 10; i++)
    {
        char upgnev[18];
        fajlnev(i, upgnev);
        //printf("%s", upgnev);
        FILE* fejl;
        fejl = fopen(upgnev, "rt");
        if (fejl == NULL)
            return i;
        for (int j = 0; j <= epulet[i].fejlesztesdb; j++)
        {
            fscanf(fejl, "%llu", &epulet[i].fejlesztesar);
            fscanf(fejl, "%s", &epulet[i].fejlesztesnev);
            vonaltalanit(epulet[i].fejlesztesnev);
            fscanf(fejl, "%s", &epulet[i].fejlesztesleiras);
            vonaltalanit(epulet[i].fejlesztesleiras);
            fscanf(fejl, "%s", &epulet[i].fejlesztesmitcsinal);
            vonaltalanit(epulet[i].fejlesztesmitcsinal);
        }
        printf("%llu\n", epulet[i].fejlesztesar);
        fclose(fejl);
    }

    //char buyablename[][11] = {"Félév", "Diploma", "Anna-Krisz", "Neptun", "Corvinus", "Schönherz", "VIK", "BME", "Nobel-díj", "Czirkos"}; // épületek fix nevei
    //unsigned long long int pricevalues[] = {30, 210, 1100, 12000, 130000, 1400000, 20000000, 330000000, 5500000000, 75000000000}; // épületek első példányának fix árai
    //unsigned long long int aktualfejlesztesar[] = {100, 500}; //
    //double basekps[10] = {0.1, 1, 8, 47, 260, 1400, 7800, 44000, 260000, 1600000}; // épületek által generált kps
    //int epuletdb[10] = {0}; // egyes épületekből birtokolt darabokSDL_Rect plusz = {550, 600, 100, 100};
    //char descs[][64] = {"Automatikusan generál egy kreditet tíz másodpercenként.", "Papírod van arról, hogy krediteket érdemelsz.", "Az Anna-Krisz mostantól kreditekkel támogat.", "Feltörve a Neptunt, csakúgy özönlenek a kreditek.", "A Corvinus teljes kreditbevétele a te számládra irányul.", "A közösségi pontjaid is kreditekre válthatod.", "Kódot írsz, ami folyamatosan krediteket ír outputra.", "A BME összes diákja és tanára a te kreditjeidért dolgozik.", "Már az igazán nagyok között vagy.", "Puszta tekintetével a semmit is kreditekké materializálja."};
    //int szorzo[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    double cheat = 0;

    /*Gombok és ikonok helye, és utóbbi fájlból dinamukusan olvasása*/
    SDL_Rect gombok[10];
    SDL_Rect fejlesztesek[10];
    SDL_Rect ikonok[10];
    SDL_Rect ikonhely[10];

    int starty = 0, starth = DM.h/10;
    for (int i = 0; i < 10; i++)
    {
        gombok[i].x = DM.w-(DM.w/5.189);
        gombok[i].y = starty+starth*i;
        gombok[i].w = DM.w/5.486;
        gombok[i].h = DM.h/10.588;

        fejlesztesek[i].x = DM.w-DM.w/3.45; ///??
        fejlesztesek[i].y = starty+starth*i;
        fejlesztesek[i].w = DM.w/14.545;
        fejlesztesek[i].h = DM.h/10.588;

        ikonok[i].x = 100*i;
        ikonok[i].y = 0;
        ikonok[i].w = 100;
        ikonok[i].h = 100;

        ikonhely[i].x = DM.w-DM.w/5.7; ///??
        ikonhely[i].y = DM.h/60.0+i*starth;
        ikonhely[i].w = DM.w/31.475;
        ikonhely[i].h = DM.h/17.705;

    }

    SDL_Rect kisbagoly = {gombok[0].x+DM.w/20.2105, gombok[0].y+DM.h/19.636, DM.w/80.0, DM.h/45.0};

    SDL_Rect vilagosgomb = {381, 308, 349, 102}; // kijelölt gomb textúra helye a guiban
    SDL_Rect simagomb = {381, 448, 349, 102}; // sima gomb textúra helye a guiban
    SDL_Rect sotetgomb = {12, 448, 349, 102}; // sötét gomb textúra helye a guiban
    SDL_Rect aktivgomb = {12, 308, 349, 102}; // rákattintott gomb textúra helye a guiban

    SDL_Rect savegomb = {DM.w/96.0, DM.h/1.1675, DM.w/7.0, DM.h/10.588};
    SDL_Rect resetgomb = {DM.w/6.4, DM.h/1.1675, DM.w/14.545, DM.h/10.588};
    SDL_Rect resetnem = {DM.w/3.6, DM.h/1.7, DM.w/5.486, DM.h/10.588};
    SDL_Rect resetigen = {DM.w/1.85, DM.h/1.7, DM.w/5.486, DM.h/10.588};


    SDL_Rect aktivfejl = {0, 1000, 132, 102}; // kijelölt kisgomb textúra helye a guiban
    SDL_Rect sotetfejl = {150, 1000, 132, 102}; // sima kisgomb textúra helye a guiban
    SDL_Rect simafejl = {300, 1000, 132, 102}; // sötét kisgomb textúra helye a guiban
    SDL_Rect vilagosfejl = {450, 1000, 132, 102}; // rákattintott kisgomb textúra helye a guiban

    SDL_Rect plusz = {550, 600, 100, 100};
    SDL_Rect kereszt = {550, 700, 100, 100};
    SDL_Rect nyil = {550, 800, 100, 100};
    SDL_Rect nyilhely = {DM.w/5.7, DM.h/1.14, DM.w/33.0, DM.h/18.0};

    SDL_Rect vizualis = {DM.w/4.0, 0, DM.w/24.0, DM.h/13.5};


    SDL_Rect leiras = {0, 560, 516, 398}; // a leírás mező hátterének textúrája a guiban
    SDL_Rect leirashely = {0, 0, DM.w/3.721, DM.h/4.696};
    SDL_Rect megerosit = {DM.w/4, DM.h/4, DM.w/2, DM.h/2};

    SDL_Rect fuggoleges = {424, 0, 40, 1047}; // fa szeparátor textúra
    SDL_Rect sep_pos = {DM.w/1.271, 0, DM.w/96.0, DM.h}; // fa szeparátor helye
    SDL_Rect bagolymellett = {DM.w/4.3, 0, DM.w/96.0, DM.h}; // fa szeparátor helye a bagoly mellett
    SDL_Rect vizszintes = {DM.w/4.5/2, DM.h/1.7, DM.w/96.0, DM.w/4.3}; // fa szeparátor helye a bagoly alatt

    SDL_Rect binaris = {DM.w/4.3, 0, 500, DM.h};
    SDL_Rect b_kozep = {DM.w/4.3, 0, DM.w/1.271-DM.w/10.45-DM.w/4.3, DM.h};
    SDL_Rect b_fejl = {DM.w/1.271-DM.w/10.45, 0, DM.w/1.271-DM.w/1.271+DM.w/10.4, DM.h};
    SDL_Rect b_epul = {DM.w/1.271, 0, DM.w-DM.w/1.273, DM.h};

    SDL_Rect feny = { 750, 300, 400, 400 }; // glow effekt textúra helye a guiban
    SDL_Rect fenyhely = { DM.w/128.0, DM.h/3.763, DM.w/4.8, DM.h/2.7 }; // glow effekt helye

    SDL_Rect bagoly = { 1000, 0, 250, 270 }; // bagoly textúra helye a guiban
    SDL_Rect sotetbagoly = { 1250, 0, 250, 270 }; // sötét bagoly textúra helye a guiban
    SDL_Rect bagolyhely = { DM.w/19.2, DM.h/2.959, DM.w/7.68, DM.h/4.0 }; // kettintandó bagoly helye
    printf("%d", DM.h);

    //1920 * 1080
    SDL_Rect elvalasztovonal = {0, 960, 499, 39};

    sz counter = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), DM.w/4.267, DM.h/5.4, 0, 0); // kreditet számláló szöveg
    sz persec = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/67.5), DM.w/4.267, DM.h/4.5, 0, 0); // kredit/sec-et számláló szöveg
    sz epuletek = feltolt(157, 155, 142, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/20.769), gombok[0].x+DM.w/6.1935, gombok[0].y, 0, 0); // épületeket számláló szöveg
    sz buyables = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), gombok[0].x+DM.w/19.2, gombok[0].y+DM.h/54.0, 0, 0); // épöletek neveit kiíró szöveg
    sz prices = feltolt(0, 0, 0, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/45.0), gombok[0].x+DM.w/15.484, gombok[0].y+DM.h/21.6, 0, 0); // épületek ára
    sz elment = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), savegomb.x+DM.w/6.5, savegomb.y+DM.h/35.0, 0, 0); // mentes gomb
    sz torles = feltolt(203, 43, 43, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), resetigen.x+DM.w/14.0, resetigen.y+DM.h/35.0, 0, 0); // torles igen gomb
    sz megse = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), resetnem.x+DM.w/14.0, resetnem.y+DM.h/35.0, 0, 0); // torles nem gomb
    sz warning = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/23.5), megerosit.x+DM.w/1.35, megerosit.y+DM.h/18.0, 0, 0); // torles figyelmezteto szoveg
    sz logo = feltolt(255, 255, 255, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/20.769), DM.w/4.25, DM.h/20.0, 0, 0); // cím


    SDL_TimerID id = SDL_AddTimer(speed, idozit, NULL);
    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_USEREVENT:
            if (bgkredit < 0) bgkredit = 0;
            if (nehamentsen % ((1000/speed)*60) == 0)
            {
                nehamentsen = 1;
                printf("Mentve.\n");
                jatekallas_mentese(bgkredit, gombdb, epulet);
            }
            nehamentsen++;

            SDL_Point kurzor;
            SDL_GetMouseState(&kurzor.x, &kurzor.y);

            SDL_RenderCopy(renderer, hatter, &bg, &bg);

            SDL_RenderCopy(renderer, gui, &simagomb, &savegomb);
            if (gombon(kurzor, savegomb, DM))
                SDL_RenderCopy(renderer, gui, &vilagosgomb, &savegomb);
            if (gombon(kurzor, savegomb, DM) && lekatt)
            {
                SDL_RenderCopy(renderer, gui, &aktivgomb, &savegomb);
                jatekallas_mentese(bgkredit, gombdb, epulet);
                strcpy(mentszoveg, "Mentve!");
            }


            SDL_RenderCopy(renderer, gui, &simafejl, &resetgomb);
            if (gombon(kurzor, resetgomb, DM))
                SDL_RenderCopy(renderer, gui, &vilagosfejl, &resetgomb);
            if (gombon(kurzor, resetgomb, DM) && lekatt)
            {
                SDL_RenderCopy(renderer, gui, &aktivfejl, &resetgomb);
                torol = true;
            }


            SDL_RenderCopy(renderer, gui, &nyil, &nyilhely);

            strcpy(elment.szoveg, hogyirki(-1, -1, elment.szoveg, mentszoveg, "", false, &nocount));
            szovegki(renderer, felirat, felirat_t, elment, center);
            strcpy(mentszoveg, "Mentés");



            if (kurzor.x > DM.w/4.3 && kurzor.x < DM.w/1.271-DM.w/10.45)
                SDL_RenderCopy(renderer, binary, &binaris, &b_kozep);
            else
                SDL_RenderCopy(renderer, darkbinary, &binaris, &b_kozep);
            if (kurzor.x > DM.w/1.271-DM.w/10.45 && kurzor.x < DM.w/1.271)
                SDL_RenderCopy(renderer, binary, &binaris, &b_fejl);
            else
                SDL_RenderCopy(renderer, darkbinary, &binaris, &b_fejl);
            if (kurzor.x > DM.w/1.271)
                SDL_RenderCopy(renderer, binary, &binaris, &b_epul);
            else
                SDL_RenderCopy(renderer, darkbinary, &binaris, &b_epul);

            fenyrender(renderer, gui, feny, fenyhely, speed, DM);
            SDL_RenderCopy(renderer, gui, &bagoly, &bagolyhely);
            SDL_RenderCopy(renderer, fa, &fuggoleges, &bagolymellett);
            SDL_RenderCopyEx(renderer, fa, &fuggoleges, &vizszintes, 90, NULL, 0);

            for (int i = 0; i < 2; i++)
            {
                SDL_Rect temphely = sep_pos;
                temphely.x = temphely.x-(i*DM.w)/10.5;
                SDL_RenderCopy(renderer, fa, &fuggoleges, &temphely);

            }


            if (gombdb>10) meddig = 10;
            else meddig = gombdb;
            for (int i = 0; i < meddig; i++) //vizualis reprezentációja az épületek számának
            {
                SDL_Rect temphely;
                int mennyit = minimum(epulet[i].db, 18);
                    for (int j = 0; j < mennyit; j++)
                    {
                        if (j == 0)
                        {
                            temphely = vizualis;
                            temphely.y = gombok[i].y;
                        }
                        if (j % 2 == 0)
                            temphely.y += DM.h/50.0;
                        else
                            temphely.y -= DM.h/50.0;
                        SDL_RenderCopy(renderer, gui, &ikonok[i], &temphely); ///mas ikonok?
                        temphely.x += DM.w/42.2;
                    }

            }


            for (int i = 0; i < gombdb-2; i++) //fejlesztesek
            {
                SDL_RenderCopy(renderer, gui, &simafejl, &fejlesztesek[i]);
                SDL_Rect temphely = {fejlesztesek[i].x+DM.w/54.857, fejlesztesek[i].y+DM.h/54.0, fejlesztesek[i].w-DM.w/27.4285, fejlesztesek[i].h-DM.h/27.0};
                SDL_RenderCopy(renderer, gui, &plusz, &temphely);
                if (kredit < epulet[i].fejlesztesar)
                {
                    SDL_RenderCopy(renderer, gui, &sotetfejl, &fejlesztesek[i]);
                    SDL_Rect temphely = {fejlesztesek[i].x+DM.w/54.857, fejlesztesek[i].y+DM.h/54.0, fejlesztesek[i].w-DM.w/27.4285, fejlesztesek[i].h-DM.h/27.0};
                    SDL_RenderCopy(renderer, gui, &kereszt, &temphely);
                }
                else if (upgraderekatt)
                {
                    if (gombon(kurzor, fejlesztesek[i], DM))
                    {
                        SDL_RenderCopy(renderer, gui, &aktivfejl, &fejlesztesek[i]);
                        SDL_Rect temphely = {fejlesztesek[i].x+DM.w/54.857, fejlesztesek[i].y+DM.h/54.0, fejlesztesek[i].w-DM.w/27.4285, fejlesztesek[i].h-DM.h/27.0};
                        SDL_RenderCopy(renderer, gui, &plusz, &temphely);
                    }
                }
                else if (gomboknal(kurzor, fejlesztesek[0], gombdb, DM) && gombon(kurzor, fejlesztesek [i], DM))
                    {
                        SDL_RenderCopy(renderer, gui, &vilagosfejl, &fejlesztesek[i]);
                        SDL_Rect temphely = {fejlesztesek[i].x+DM.w/54.857, fejlesztesek[i].y+DM.h/54.0, fejlesztesek[i].w-DM.w/27.4285, fejlesztesek[i].h-DM.h/27.0};
                        SDL_RenderCopy(renderer, gui, &plusz, &temphely);
                    }

                if (gomboknal(kurzor, fejlesztesek[0], gombdb, DM) && gombon(kurzor, fejlesztesek [i], DM))
                {
                    if (kurzor.y <= DM.h-DM.h/4.696)  leirashely.y = kurzor.y;
                    else leirashely.y = DM.h-DM.h/4.696;
                    leirashely.x = fejlesztesek[0].x-leirashely.w;
                    SDL_RenderCopy(renderer, gui, &leiras, &leirashely);
                    sz tempszoveg = prices;
                    tempszoveg.hely.x = leirashely.x+DM.w/4.129;
                    tempszoveg.hely.y = leirashely.y+DM.h/20.0;
                    if (kredit >= epulet[i].fejlesztesar)
                    {
                        tempszoveg.szin.r = 102;
                        tempszoveg.szin.g = 255;
                        tempszoveg.szin.b = 102;
                    }
                    else
                    {
                        tempszoveg.szin.r = 138;
                        tempszoveg.szin.g = 70;
                        tempszoveg.szin.b = 75;
                    }
                    strcpy(tempszoveg.szoveg, hogyirki(epulet[i].fejlesztesar, epulet[i].fejlesztesar, epuletek.szoveg, "", "", false, &nocount));
                    if (epulet[i].fejlesztesar != 0) szovegki(renderer, felirat, felirat_t, tempszoveg, right);
                    tempszoveg = feltolt(220, 220, 220, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/67.5), leirashely.x+DM.w/21.333, leirashely.y+DM.h/19.636, 0, 0);
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, epuletek.szoveg, "[Fejlesztés]", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, left);
                    tempszoveg = buyables;
                    tempszoveg.hely.x = leirashely.x+DM.w/21.333;
                    tempszoveg.hely.y = leirashely.y+DM.h/54.0;

                    SDL_Rect temphely = {leirashely.x+DM.h/96.0, leirashely.y+DM.h/54.0, DM.w/31.475, DM.h/17.705};
                    SDL_RenderCopy(renderer, gui, &ikonok[i], &temphely);

                    SDL_Rect elvalasztohely = {leirashely.x+25, leirashely.y+90, DM.w/3.765, DM.h/216.0};
                    SDL_RenderCopy(renderer, gui, &elvalasztovonal, &elvalasztohely);

                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, epuletek.szoveg, epulet[i].fejlesztesnev, "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, left);

                    tempszoveg = feltolt(220, 220, 220, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/67.5), leirashely.x+DM.w/54.857, leirashely.y+DM.h/9.818, 0, 0);
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, epuletek.szoveg, epulet[i].fejlesztesleiras, "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, left);

                    tempszoveg.hely.y += DM.h/21.6;
                    tempszoveg.hely.x = leirashely.x + leirashely.w-DM.w/96.0;

                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, epuletek.szoveg, epulet[i].fejlesztesmitcsinal, "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, right);

                }
            }

            for (int i = 0; i < meddig; i++) //annysiszor fut le renderelesenkent ahany gombot meg kell jeleniteni
            {


                SDL_RenderCopy(renderer, gui, &simagomb, &gombok[i]);
                ikonok[i].y = 0;

                if (gombdb-2 > i)
                {
                    if (kredit < epulet[i].ar)
                    {
                        SDL_RenderCopy(renderer, gui, &sotetgomb, &gombok[i]);
                        ikonok[i].y = 100;
                        kisbagoly.y=gombok[i].y+DM.h/19.636;
                        SDL_RenderCopy(renderer, gui, &sotetbagoly, &kisbagoly);
                    }

                    else if (gombrakatt)
                    {
                        if (gombon(kurzor, gombok[i], DM))
                        {
                            SDL_RenderCopy(renderer, gui, &aktivgomb, &gombok[i]);
                        }
                    }
                    else if (gomboknal(kurzor, gombok[0], gombdb, DM) && gombon(kurzor, gombok[i], DM))
                    {
                        SDL_RenderCopy(renderer, gui, &vilagosgomb, &gombok[i]);
                    }

                    sprintf(buyables.szoveg, epulet[i].nev);
                    buyables.hely.y=gombok[i].y+DM.h/60.0;
                    szovegki(renderer, felirat, felirat_t, buyables, left);
                    kisbagoly.y=gombok[i].y+DM.h/19.636;
                    SDL_RenderCopy(renderer, gui, &bagoly, &kisbagoly);

                    //
                }
                else
                {
                    SDL_RenderCopy(renderer, gui, &sotetgomb, &gombok[i]);
                    ikonok[i].y = 200;
                    sprintf(buyables.szoveg, "???");
                    buyables.hely.y=gombok[i].y+DM.h/60.0;
                    szovegki(renderer, felirat, felirat_t, buyables, left);
                    kisbagoly.y=gombok[i].y+DM.h/19.636;
                    SDL_RenderCopy(renderer, gui, &sotetbagoly, &kisbagoly);
                }




                sprintf(epuletek.szoveg, "%d", epulet[i].db);
                epuletek.hely.y=gombok[i].y+DM.h/108.0;
                if (epulet[i].db != 0) szovegki(renderer, felirat, felirat_t, epuletek, right);


                if (kredit < epulet[i].ar)
                {
                    prices.szin.r = 138;
                    prices.szin.g = 70;
                    prices.szin.b = 75;
                }
                else
                {
                    prices.szin.r = 102;
                    prices.szin.g = 255;
                    prices.szin.b = 102;
                }
                strcpy(prices.szoveg, hogyirki(epulet[i].ar, epulet[i].ar, prices.szoveg, "", "", false, &nocount));
                prices.hely.y = gombok[i].y+DM.h/21.176;
                szovegki(renderer, felirat, felirat_t, prices, left);

                SDL_RenderCopy(renderer, gui, &ikonok[i], &ikonhely[i]);
                if (bgkredit >= epulet[gombdb-2].ar-(gombdb-2)*epulet[gombdb-2].ar/20 && gombdb < 12) gombdb++;


                if (gomboknal(kurzor, gombok[0], gombdb, DM) && gombon(kurzor, gombok[i], DM))
                    {
                        if (kurzor.y <= DM.h-DM.h/4.6967)  leirashely.y = kurzor.y;
                        else leirashely.y = DM.h-DM.h/4.6967;
                        leirashely.x = gombok[0].x-leirashely.w;
                        SDL_RenderCopy(renderer, gui, &leiras, &leirashely);
                        SDL_Rect temphely = {leirashely.x+DM.h/96.0, leirashely.y+DM.h/54.0, DM.w/31.475, DM.h/17.705};
                        SDL_RenderCopy(renderer, gui, &ikonok[i], &temphely);
                        sz tempszoveg = buyables;
                        tempszoveg.hely.y = temphely.y;
                        tempszoveg.hely.x = temphely.x+DM.w/27.4285;
                        szovegki(renderer, felirat, felirat_t, tempszoveg, left);
                        tempszoveg = feltolt(220, 220, 220, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/67.5), temphely.x+DM.w/27.4285, temphely.y+DM.h/30.857, 0, 0);
                        strcpy(tempszoveg.szoveg, hogyirki(epulet[i].db, epulet[i].db, epuletek.szoveg, "[megvásárolva: ", "]", false, &nocount));
                        szovegki(renderer, felirat, felirat_t, tempszoveg, left);
                        tempszoveg = prices;
                        tempszoveg.hely.y = leirashely.y+DM.w/56.4705;
                        tempszoveg.hely.x = leirashely.x+DM.h/2.3225;
                        szovegki(renderer, felirat, felirat_t, tempszoveg, right);
                        //SDL_Rect tempikon = {leirashely.x+450, leirashely.y+38, 24, 24};
                        //SDL_RenderCopy(renderer, gui, &bagoly, &tempikon);
                        SDL_Rect elvalasztohely = {leirashely.x+DM.w/76.8, leirashely.y+DM.h/12.0, DM.w/3.765, DM.h/216.0};
                        SDL_RenderCopy(renderer, gui, &elvalasztovonal, &elvalasztohely);
                        if (i < gombdb-2)
                        {
                            elvalasztohely.y+=DM.h/30.857;
                            SDL_RenderCopy(renderer, gui, &elvalasztovonal, &elvalasztohely);
                            tempszoveg = feltolt(220, 220, 220, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/67.5), temphely.x+DM.w/192.0, temphely.y+DM.h/13.5, 0, 0);
                            strcpy(tempszoveg.szoveg, hogyirki(-1, -1, epuletek.szoveg, epulet[i].leiras, "", false, &nocount));
                            szovegki(renderer, felirat, felirat_t, tempszoveg, left);
                            tempszoveg.hely.y += DM.h/30.857;
                            strcpy(tempszoveg.szoveg, hogyirki(epulet[i].kps*epulet[i].szorzo, epulet[i].kps*epulet[i].szorzo, epuletek.szoveg, "Darabonként ", " kredit/sec", true, &nocount));
                            szovegki(renderer, felirat, felirat_t, tempszoveg, left);

                            tempszoveg.hely.y += DM.h/54.0;
                            strcpy(tempszoveg.szoveg, hogyirki(epulet[i].db*epulet[i].kps*epulet[i].szorzo, epulet[i].db*epulet[i].kps*epulet[i].szorzo, epuletek.szoveg, "Összesen ", " kredit/sec", true, &nocount));
                            szovegki(renderer, felirat, felirat_t, tempszoveg, left);

                            tempszoveg.hely.y += DM.h/54.0;
                            strcpy(tempszoveg.szoveg, hogyirki((epulet[i].db*epulet[i].kps/kps)*100*epulet[i].szorzo, (epulet[i].db*epulet[i].kps/kps)*100*epulet[i].szorzo, epuletek.szoveg, "Ez az összes bevétel ", "%-a.", true, &nocount));
                            if (kps != 0) szovegki(renderer, felirat, felirat_t, tempszoveg, left);
                        }
                    }
                    //minden gombnal

            }

            strcpy(counter.szoveg, hogyirki(kredit, bgkredit, counter.szoveg, "", " kredit", false, &nocount));
            strcpy(persec.szoveg, hogyirki(kps, kps, persec.szoveg, "", " kredit/sec", true, &nocount));

            szovegki(renderer, felirat, felirat_t, counter, center);
            szovegki(renderer, felirat, felirat_t, persec, center);

            if (nocount && kredit < 10000000000000000000)
                nocount = false;
            if (!nocount)
            {
                bgkredit+=kps/(1000/speed);
                kredit = bgkredit;
            }
            kps = sumkps(epulet) + cheat; // kps kiszámítása

                lebeges *jelen;
                jelen = kattszoveg;
                while (jelen != NULL)
                {
                    jelen->timer++;
                    jelen->szoveg.hely.y -= DM.h/540.0;
                    //jelen->szoveg.szin.r -= 2;
                    jelen->szoveg.szin.g -= 2;
                    //jelen->szoveg.szin.b -= 2;
                    if (jelen->timer == 1) strcpy(jelen->szoveg.szoveg, hogyirki(1*epulet[0].szorzo, 1*epulet[0].szorzo, jelen->szoveg.szoveg, "+", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, jelen->szoveg, center);
                    jelen = jelen->kovetkezo;
                }
                if (kattszoveg!=NULL)
                if (kattszoveg->timer >= (1000/speed)*3)
                {
                    elsoelemtorol(&kattszoveg);
                }

                if (torol)
                {
                    SDL_RenderCopy(renderer, gui, &leiras, &megerosit);
                    SDL_RenderCopy(renderer, gui, &simagomb, &resetigen);
                    SDL_RenderCopy(renderer, gui, &simagomb, &resetnem);

                    if (gombon(kurzor, resetigen, DM))
                        SDL_RenderCopy(renderer, gui, &vilagosgomb, &resetigen);
                    if (gombon(kurzor, resetigen, DM) && lekatt)
                    {
                        SDL_RenderCopy(renderer, gui, &aktivgomb, &resetigen);
                        remove("save.txt");
                        quit = true;
                    }

                    if (gombon(kurzor, resetnem, DM))
                        SDL_RenderCopy(renderer, gui, &vilagosgomb, &resetnem);
                    if (gombon(kurzor, resetnem, DM) && lekatt)
                    {
                        SDL_RenderCopy(renderer, gui, &aktivgomb, &resetnem);
                        torol = false;
                    }

                    sz tempszoveg = warning;
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "Biztosan törölni akarod a játékállást?", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, center);
                    tempszoveg.hely.y += DM.h/10.8;
                    tempszoveg.betutipus = megse.betutipus;
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "A mostani mentés nem lesz visszaállítható!", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, center);
                    tempszoveg.hely.y += DM.h/13.5;
                    tempszoveg.betutipus = prices.betutipus;
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "Törölve lesz az összes megvásárolt fejlesztés és épület", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, center);
                    tempszoveg.hely.y += DM.h/43.2;
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "és a kreditek száma valamint a kps is nullára lesz visszaállítva.", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, center);
                    tempszoveg.hely.y += DM.h/21.6;
                    tempszoveg.szin = torles.szin;
                    strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "Igenre kattintva a program kilép.", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, tempszoveg, center);



                    strcpy(torles.szoveg, hogyirki(-1, -1, torles.szoveg, "Igen", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, torles, left);

                    strcpy(megse.szoveg, hogyirki(-1, -1, megse.szoveg, "Nem", "", false, &nocount));
                    szovegki(renderer, felirat, felirat_t, megse, left);

                }
                sz tempszoveg = logo;
                strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "Info    licker", "", false, &nocount));
                szovegki(renderer, felirat, felirat_t, tempszoveg, center);

                tempszoveg.betutipus = persec.betutipus;
                tempszoveg.hely.y += DM.h/18.0;
                strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "by Gyeni", "", false, &nocount));
                szovegki(renderer, felirat, felirat_t, tempszoveg, center);

                tempszoveg.szin = (SDL_Color) {237, 178, 27};
                tempszoveg.hely = (SDL_Rect) {DM.w/10.5, DM.h/27.0, 0, 0};

                tempszoveg.betutipus = TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/16.0);
                strcpy(tempszoveg.szoveg, hogyirki(-1, -1, tempszoveg.szoveg, "C", "", false, &nocount));
                szovegki(renderer, felirat, felirat_t, tempszoveg, left);


            break;
        case SDL_MOUSEBUTTONDOWN:
            lekatt = true;
            if (baglyon(kurzor, DM))
            {
                lebeges * uccso;
                uccso = klikklista(&kattszoveg);
                uccso->szoveg = feltolt(40, 190, 40, TTF_OpenFont("resources/Kavoon-Regular.ttf", DM.h/33.75), 100, 100, 0, 0);

                uccso->szoveg.hely.x = kurzor.x+DM.w/7.68+rand()%50-25;
                uccso->szoveg.hely.y = kurzor.y-DM.h/43.2;

                bgkredit+=1*epulet[0].szorzo;
            }
            if (gomboknal(kurzor, gombok[0], gombdb, DM)) gombrakatt = true;
            if (gomboknal(kurzor, fejlesztesek[0], gombdb, DM)) upgraderekatt = true;

            break;
        case SDL_MOUSEBUTTONUP:
            lekatt = false;
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                if (gombrakatt)
                    for (int i = 0; i < meddig; i++)
                        if (gombon(kurzor, gombok[i], DM) && kredit >= epulet[i].ar)
                        {
                            epulet[i].db++;
                            bgkredit-=epulet[i].ar;
                            epulet[i].ar = ceil(epulet[i].ar*1.15);
                        }
                if (upgraderekatt)
                    for (int i = 0; i < meddig; i++)
                        if (gombon(kurzor, fejlesztesek[i], DM) && kredit >= epulet[i].fejlesztesar)
                        {
                            /*Upgrade árkezelése ide*/
                            epulet[i].fejlesztesdb++;
                            bgkredit-=epulet[i].fejlesztesar;
                            if (epulet[i].fejlesztesar != 0) epulet[i].szorzo *= 2;
                            epulet[i].fejlesztesar = mostaniar(epulet, i);


                        }
            }
            upgraderekatt = false;
            gombrakatt = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                jatekallas_mentese(bgkredit, gombdb, epulet);
                quit=true;
                break;
            case SDLK_UP: // for debugging
                cheat = 1+ cheat*1.2;
                break;
            case SDLK_RIGHT:
                epulet[0].szorzo *=2;
                break;
            case SDLK_SPACE: //for debugging
                bgkredit++;
                //gombdb++;
                //bgkredit = 0;
                break;
            case SDLK_s:
                jatekallas_mentese(bgkredit, gombdb, epulet);
                break;
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }

        SDL_RenderPresent(renderer);
    }

    while (kattszoveg != NULL)
    {
        elsoelemtorol(&kattszoveg);
    }

    fclose(gombadatok);

    TTF_CloseFont(epuletek.betutipus);
    TTF_CloseFont(counter.betutipus);
    TTF_CloseFont(persec.betutipus);
    TTF_CloseFont(buyables.betutipus);
    TTF_CloseFont(prices.betutipus);
    TTF_CloseFont(elment.betutipus);
    TTF_CloseFont(torles.betutipus);
    TTF_CloseFont(megse.betutipus);
    TTF_CloseFont(warning.betutipus);


    SDL_DestroyTexture(binary);
    SDL_DestroyTexture(fa);
    SDL_DestroyTexture(gui);
    SDL_DestroyTexture(hatter);


    SDL_RemoveTimer(id);
    SDL_Quit();

    return 0;
}
