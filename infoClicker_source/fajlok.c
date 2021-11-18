#include "fajlok.h"
#include "debugmalloc.h"

void vonaltalanit (char * szoveg)
{
    for (int i = 0; szoveg[i] != 0; i++)
        if (szoveg[i] == '_') szoveg[i] = ' ';
}

void fajlnev(int i, char * fajlnev)
{
        strcpy(fajlnev, "upgrades/upg"); //"upgrades/upgN.txt" mindig 17 karakter + \0
        char fajlszam[2] = {i + '0'};
        strcat(fajlnev, fajlszam);
        strcat(fajlnev, ".txt");
}

unsigned long long int mostaniar(epuletek * epulet, int x)
{
    char upgnev[18];
    fajlnev(x, upgnev);
    FILE* fejl;
    fejl = fopen(upgnev, "rt");
    for (int j = 0; j <= epulet[x].fejlesztesdb; j++)
    {
        fscanf(fejl, "%llu", &epulet[x].fejlesztesar);
        fscanf(fejl, "%s", &epulet[x].fejlesztesnev);
        vonaltalanit(epulet[x].fejlesztesnev);
        fscanf(fejl, "%s", &epulet[x].fejlesztesleiras);
        vonaltalanit(epulet[x].fejlesztesleiras);
        fscanf(fejl, "%s", &epulet[x].fejlesztesmitcsinal);
        vonaltalanit(epulet[x].fejlesztesmitcsinal);
    }
    //printf("%llu\n", epulet[x].fejlesztesar);
    fclose(fejl);
    return epulet[x].fejlesztesar;
}

void jatekallas_mentese(double bgkredit, int gombdb, epuletek * epulet)
{
    FILE* mentes;
    mentes = fopen("save.txt", "wt");

    fprintf(mentes, "%lf ", bgkredit);
    fprintf(mentes, "%d\n", gombdb);
    for (int i = 0; i < 10; i++)
    {
        fprintf(mentes, "%d ", epulet[i].db);
        fprintf(mentes, "%d ", epulet[i].szorzo);
        fprintf(mentes, "%d\n", epulet[i].fejlesztesdb);
    }

    fclose(mentes);
}
