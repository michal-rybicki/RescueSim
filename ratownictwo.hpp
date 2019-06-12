#ifndef ratownictwo_hpp
#define ratownictwo_hpp

#include <conio.h> //do sterowania klawiszami
#include <iomanip> //do wyswietlania z zaokrogleniem
#include <fstream> //do obslugi plikow
#include <iostream> // do strumienia wejscia/wyjscia
#include <windows.h> //do koloru
#include <stdlib.h> //do koloru.
#include <cctype> //do analizy znakow (funkcja tolower)
#include <cstdlib> //do losowania
#include <ctime> //do losowania
#include <cmath> //do obliczen czasu podrozy
#include <vector> //do przechowywania zgloszen
#include "zegar.hpp" //do obslugi czasu

class wspolrzedne;
class pojazd;
class lokacja;

//-----WSPOLRZEDNE---------------------------------------------------------------------------------

class wspolrzedne//klasa pomocnicza, przechowuje wspolrzedne
{
public:
    int x, y;
    wspolrzedne(int x0, int y0);
    ustaw(int x0, int y0);
};

wspolrzedne::wspolrzedne(int x0=0,int y0=0)
{
    x=x0;
    y=y0;
}

wspolrzedne::ustaw(int x0=0,int y0=0)
{
    x=x0;
    y=y0;
}

//-----LOKACJE---------------------------------------------------------------------------------
class lokacja
{
public:
    wspolrzedne lokalizacja;
    int typ;
    //0 - baza ratownictwa
    //1 - szpital
    //2 - ranny
    //3 - puste miejsce

};

//-----LOKACJE MIEJSCE WYPADKU---------------------------------------------------------------------------------

class ranny:public lokacja
{
public:
    int nr; //nr zgloszenia
    int priorytet;
    //0 - niski
    //1 - wysoki
    int status;
    //0 - oczekuje pomocy
    //1 - uratowany
    //-1 - nie zyje
    zegar zdarzenie;//godzina zgloszenia wypadku
    zegar zgon;//godzina po ktorej pacjent umiera, jezeli pomoc nie dotarla
    zegar nadeszla_pomoc;//godzina o ktorej pomoc dotarla do rannego

    ranny();
    void ustaw_piorytet(int x,int y);

};

ranny::ranny()
{
    typ=2;
    status=0;
}

void ranny::ustaw_piorytet(int x,int y)
{
    if ((zgon-zdarzenie)<abs(x-lokalizacja.x)+abs(y-lokalizacja.y))
    {
        priorytet=1;
    }
    else
    {
        priorytet=0;
    }
}

//-----LOKACJE SZPITAL---------------------------------------------------------------------------------

class szpital_typ:public lokacja
{
public:
    szpital_typ();
};

szpital_typ::szpital_typ()
{
    typ=1;
}

//-----LOKACJE BAZA RATOWNICTWA---------------------------------------------------------------------------------

class baza_typ:public lokacja
{
public:
    pojazd * pojazdy[5];
    int pojazdy_w_bazie;
    baza_typ();
};

baza_typ::baza_typ()
{
    pojazdy_w_bazie=5;
    typ=0;
}

//-----LOKACJE PUSTE MIEJSCE---------------------------------------------------------------------------------

class puste:public lokacja
{
public:
    puste();
};

puste::puste()
{
    typ=3;
}

//-----POJAZDY---------------------------------------------------------------------------------

class pojazd
{
public:
    int nr_zgloszenia;//nr zgloszenia, na ktore reaguje pojazd
    wspolrzedne GPS;//aktualne wspolrzedne pojazdu
    wspolrzedne garaz;//wspolrzedne garazu pojazdu
    zegar grafik[6];//godziny w ktorych pojazd bedzie konczyl poszczegolne etapy udzielania pomocy
                    //stworzony w momencie otrzymania wezwania
    //0 - pojazd dostepny
    //1 - w drodze na miejsce wypadku
    //2 - udziela pomocy
    //3 - w drodze do szpitala
    //4 - w drodze do bazy
    //5 - w serwisie
    int typ;
    //0 - ambulans
    //1 - helikopter
    virtual int podroz(lokacja cel, double trudnosc)=0;//funkcja podrozy do lokacji
    virtual int podroz(wspolrzedne cel, double trudnosc)=0;//funkcja podrozy do wspolrzednych
    virtual void udziel_pomocy(ranny& cel, zegar aktualny_czas, szpital_typ szpital)=0;//funkcja udzielania pomocy rannemu
    pojazd(int x0, int y0);
};

pojazd::pojazd(int x0=0, int y0=0)
{
    GPS.ustaw(x0,y0);
    garaz.ustaw(x0,y0);
}

//-----POJAZDY AMBULANS---------------------------------------------------------------------------------

class ambulans:public pojazd
{
public:
    int podroz(wspolrzedne cel, double trudnosc=1)//funkcja podrozy do wspolrzednych
    {
        int czas=trudnosc*(abs(cel.x-GPS.x)+abs(cel.y-GPS.y));
        GPS.x=cel.x;
        GPS.y=cel.y;
        return czas;
    }
    int podroz(lokacja cel, double trudnosc=1)//funkcja podrozy do lokacji
    {
        int czas=trudnosc*(abs(cel.lokalizacja.x-GPS.x)+abs(cel.lokalizacja.y-GPS.y));
        GPS.x=cel.lokalizacja.x;
        GPS.y=cel.lokalizacja.y;
        return czas;
    }
    void udziel_pomocy(ranny& cel, zegar aktualny_czas, szpital_typ szpital);
    ambulans(int x0, int y0);
};

ambulans::ambulans(int x0=0, int y0=0)//x0,y0 - wspolrzedne garazu ambulansu
{
    typ=0;
    nr_zgloszenia=1;
    GPS.ustaw(x0,y0);
    garaz.ustaw(x0,y0);
}

void ambulans::udziel_pomocy(ranny& cel, zegar aktualny_czas, szpital_typ szpital)//funkcja udzielania pomocy rannemu
{
    nr_zgloszenia=cel.nr;
    grafik[0]=aktualny_czas;
    grafik[1]=grafik[0]+podroz(cel);
    cel.nadeszla_pomoc=grafik[1];
    if (grafik[1]<cel.zgon)
    {
        grafik[2]=grafik[1]+15;
        cel.status=1;
        grafik[3]=grafik[2]+podroz(szpital);
    }
    else
    {
        grafik[2]=grafik[1]+5;
        cel.status=-1;
        grafik[3]=grafik[2];
    }
    grafik[4]=grafik[3]+podroz(garaz);
    grafik[5]=grafik[4]+10;
}

//-----POJAZDY HELIKOPTER---------------------------------------------------------------------------------

class helikopter:public pojazd
{
public:
    int podroz(wspolrzedne cel, double trudnosc=1)//funkcja podrozy do wspolrzednych
    {
        int czas=ceil(sqrt(abs(cel.x-GPS.x)*abs(cel.x-GPS.x)+abs(cel.y-GPS.y)*abs(cel.y-GPS.y))/2);
        GPS.x=cel.x;
        GPS.y=cel.y;
        return czas;
    }
    int podroz(lokacja cel, double trudnosc=1)//funkcja podrozy do lokacji
    {
        int czas=(sqrt(abs(cel.lokalizacja.x-GPS.x)*abs(cel.lokalizacja.x-GPS.x)+abs(cel.lokalizacja.y-GPS.y)*abs(cel.lokalizacja.y-GPS.y))/2);
        GPS.x=cel.lokalizacja.x;
        GPS.y=cel.lokalizacja.y;
        return czas;
    }
    void udziel_pomocy(ranny& cel, zegar aktualny_czas, szpital_typ szpital);
    helikopter(int x0, int y0);
};

helikopter::helikopter(int x0=0, int y0=0)//x0,y0 - wspolrzedne garazu ambulansu
{
    typ=1;
    nr_zgloszenia=1;
    GPS.ustaw(x0,y0);
    garaz.ustaw(x0,y0);
}

void helikopter::udziel_pomocy(ranny& cel, zegar aktualny_czas, szpital_typ szpital)//funkcja udzielania pomocy rannemu
{
    nr_zgloszenia=cel.nr;
    grafik[0]=aktualny_czas;
    grafik[1]=grafik[0]+podroz(cel);
    cel.nadeszla_pomoc=grafik[1];
    if (grafik[1]<cel.zgon)
    {
        grafik[2]=grafik[1]+20;
        cel.status=1;
        grafik[3]=grafik[2]+podroz(szpital);
    }
    else
    {
        grafik[2]=grafik[1]+10;
        cel.status=-1;
        grafik[3]=grafik[2];
    }
    grafik[4]=grafik[3]+podroz(garaz);
    grafik[5]=grafik[4]+15;
}

//-----MIASTO---------------------------------------------------------------------------------

class miasto_bazowe
{
public:
    int m,n;
    szpital_typ szpital;
    baza_typ baza;
    char** plan_miasta;

    char** generuj_miasto(int Sx, int Sy, int Bx, int By);
    void ustaw_szpital();
    void ustaw_baze();
    void rysuj_plan_miasta();

    virtual bool funkcja_miasta(int x, int y)=0;

};

char** miasto_bazowe::generuj_miasto(int Sx=0, int Sy=0, int Bx=0, int By=0)
{
    if (plan_miasta!=NULL)
    {
    for (int i=0; i<n; i++)
        delete [] plan_miasta[i];
    delete [] plan_miasta;
    }
    baza.lokalizacja.ustaw(Bx,By);
    szpital.lokalizacja.ustaw(Sx,Sy);
    char ** plan;
    plan=new char*[n];
    for (int i=0; i<n; i++)
        plan[i]=new char[m];
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            if (funkcja_miasta(j,i))
                plan[i][j]='+';
            else
                plan[i][j]='-';
    if (By!=0) plan[By][Bx]='B';
    if (Sy!=0) plan[Sy][Sx]='S';
    return plan;
}

void miasto_bazowe::ustaw_szpital()
{
    char wybor='Q';
    int x=1, y=1;
    plan_miasta=generuj_miasto(x,y,0,0);
    system( "cls" ); //czyszczenie ekranu
    cout << "Zlokalizuj szpital:" << endl;
    rysuj_plan_miasta();
    do
    {
        wybor=getch();
        if ((wybor=='D' || wybor=='d') && x<m-2)
            x++;
        else if ((wybor=='A' || wybor=='a') && x>1)
            x--;
        else if ((wybor=='S' || wybor=='s') && y<n-2)
            y++;
        else if ((wybor=='W' || wybor=='w') && y>1)
            y--;
        plan_miasta=generuj_miasto(x,y,0,0);
        system( "cls" ); //czyszczenie ekranu
        cout << "Zlokalizuj szpital:" << endl;
        rysuj_plan_miasta();
    }
    while(tolower(wybor)!='e');
}

void miasto_bazowe::ustaw_baze()
{
    char wybor='Q';
    int x=1, y=1;
    plan_miasta=generuj_miasto(szpital.lokalizacja.x,szpital.lokalizacja.y,x,y);
    system( "cls" ); //czyszczenie ekranu
    cout << "Zlokalizuj baze ratownictwa:" << endl;
    rysuj_plan_miasta();
    do
    {
        wybor=getch();
        if ((wybor=='D' || wybor=='d') && x<m-2)
            x++;
        else if ((wybor=='A' || wybor=='a') && x>1)
            x--;
        else if ((wybor=='S' || wybor=='s') && y<n-2)
            y++;
        else if ((wybor=='W' || wybor=='w') && y>1)
            y--;
        plan_miasta=generuj_miasto(szpital.lokalizacja.x,szpital.lokalizacja.y,x,y);
        system( "cls" ); //czyszczenie ekranu
        cout << "Zlokalizuj baze ratownictwa:" << endl;
        rysuj_plan_miasta();
    }
    while(tolower(wybor)!='e');

    for (int i=0; i<5; i++) baza.pojazdy[i]->garaz.ustaw(baza.lokalizacja.x,baza.lokalizacja.y);
}

void miasto_bazowe::rysuj_plan_miasta()
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            cout << plan_miasta[i][j];
        }
        cout << endl;
    }
}

//-----MIASTO OKRAGLE---------------------------------------------------------------------------------

class miasto_okragle:public miasto_bazowe
{
public:
    miasto_okragle(int Sx, int Sy, int Bx, int By);
    bool funkcja_miasta(int x, int y);
};

miasto_okragle::miasto_okragle(int Sx=10, int Sy=10, int Bx=20, int By=5)
{
    n=20;
    m=50;
    plan_miasta=NULL;
    plan_miasta=generuj_miasto(Sx,Sy,Bx,By);
    for (int i=0; i<4; i++)
        baza.pojazdy[i]=new ambulans(baza.lokalizacja.x,baza.lokalizacja.y);
    baza.pojazdy[4]=new helikopter(baza.lokalizacja.x,baza.lokalizacja.y);
}

bool miasto_okragle::funkcja_miasta(int x, int y)
{
    if ((y-n/2+2)*(y-n/2+2)+(x-m/2+8)*(x-m/2+8)<40)
        return 1;
    else
        return 0;
}

//-----MIASTO SKOSNE---------------------------------------------------------------------------------

class miasto_skosne:public miasto_bazowe
{
public:
    miasto_skosne(int Sx, int Sy, int Bx, int By);
    bool funkcja_miasta(int x, int y);
};

miasto_skosne::miasto_skosne(int Sx=10, int Sy=10, int Bx=20, int By=5)
{
    n=20;
    m=50;
    plan_miasta=NULL;
    plan_miasta=generuj_miasto(Sx,Sy,Bx,By);
    for (int i=0; i<4; i++)
        baza.pojazdy[i]=new ambulans(baza.lokalizacja.x,baza.lokalizacja.y);
    baza.pojazdy[4]=new helikopter(baza.lokalizacja.x,baza.lokalizacja.y);
}

bool miasto_skosne::funkcja_miasta(int x, int y)
{
    if (y<x && y>x-8)
        return 1;
    else
        return 0;
}

//-----MIASTO KRZYZOWE---------------------------------------------------------------------------------

class miasto_krzyzowe:public miasto_bazowe
{
public:
    miasto_krzyzowe(int Sx, int Sy, int Bx, int By);
    bool funkcja_miasta(int x, int y);
};

miasto_krzyzowe::miasto_krzyzowe(int Sx=10, int Sy=10, int Bx=20, int By=5)
{
    n=20;
    m=50;
    plan_miasta=NULL;
    plan_miasta=generuj_miasto(Sx,Sy,Bx,By);
    for (int i=0; i<4; i++)
        baza.pojazdy[i]=new ambulans(baza.lokalizacja.x,baza.lokalizacja.y);
    baza.pojazdy[4]=new helikopter(baza.lokalizacja.x,baza.lokalizacja.y);
}

bool miasto_krzyzowe::funkcja_miasta(int x, int y)
{
    if ((x>2*m/8 && x<3*m/8) || (y>3*n/8 && y<4*n/8))
        return 1;
    else
        return 0;
}

//-----SYMULACJA---------------------------------------------------------------------------------

class symulacja_typ
{
public:
    //do generowania symulacji
    miasto_bazowe * miasto;
    zegar aktualny_czas;
    vector<ranny> zgloszenie;

    //do przechowywania wynikow
    int liczba_zgonow;
    double czas_do_przybycia_pomocy;
    int liczba_interwencji[5];
    char ** mapa_zdarzen;
    fstream raport;

    //metody do generowania symulcaji
    symulacja_typ(miasto_bazowe * miasto);
    void generuj_zgloszenia();
    void start();

    //metody do opisu wynikow symulacji
    void generuj_statystyki();
    void wyswietl_statystyki();
    void wyswietl_raport();
    void rysuj_mape_zdarzen();

    //metody informacyjne
    void wyswietl_zalozenia();

};

symulacja_typ::symulacja_typ(miasto_bazowe * miasto0)
{
    mapa_zdarzen=NULL;
    miasto=miasto0;

}

void symulacja_typ::generuj_zgloszenia()
{
    zgloszenie.clear();
    int x0, y0;
    zegar nowe_zdarzenie(0,1);
    ranny nowy_ranny;
    zegar koniec(23,59);
    while (nowe_zdarzenie<koniec)
    {
        nowe_zdarzenie=nowe_zdarzenie+10+rand()%40+1;
        nowy_ranny.zdarzenie=nowe_zdarzenie;
        nowy_ranny.zgon=nowe_zdarzenie+15+rand()%40+1;
        nowy_ranny.nr=zgloszenie.size()+1;
        if (rand()%2==1)
        {
            do
            {
                x0=rand()%miasto->m;
                y0=rand()%miasto->n;
            }
            while(miasto->funkcja_miasta(x0,y0)==1 || ((x0==miasto->szpital.lokalizacja.x && y0==miasto->szpital.lokalizacja.y) || (x0!=miasto->baza.lokalizacja.x && y0==miasto->baza.lokalizacja.y)));
            nowy_ranny.lokalizacja.x=x0;
            nowy_ranny.lokalizacja.y=y0;
        }
        else
        {
            do
            {
                x0=rand()%miasto->m;
                y0=rand()%miasto->n;
            }
            while(miasto->funkcja_miasta(x0,y0)==0 || ((x0==miasto->szpital.lokalizacja.x && y0==miasto->szpital.lokalizacja.y) || (x0==miasto->baza.lokalizacja.x && y0==miasto->baza.lokalizacja.y)));
            nowy_ranny.lokalizacja.x=x0;
            nowy_ranny.lokalizacja.y=y0;
        }
        nowy_ranny.ustaw_piorytet(miasto->baza.lokalizacja.x,miasto->baza.lokalizacja.y);
        zgloszenie.push_back(nowy_ranny);
    }
}

void symulacja_typ::start()
{
    generuj_zgloszenia();
    for (int i=0; i<5; i++)
    {
        liczba_interwencji[i]=0;
        miasto->baza.pojazdy[i]->grafik[5].ustaw_zegar(0,1);
    }
    zegar czas_symulacji(0,1); //ustawia zegar na 00:01
    raport.open("raport.txt",ios::out);
    int przyjete_zgloszenie=0;
    int obsluzone_zgloszenie=0;
    while (obsluzone_zgloszenie<zgloszenie.size() || miasto->baza.pojazdy_w_bazie<5)//Generuje komunikaty dopoki nie obsluzy wszystkich zgloszen i wszystkie pojazdy nie wroca do bazy
    {
        //Przyjmowanie zgloszen
        if (czas_symulacji==zgloszenie[przyjete_zgloszenie].zdarzenie)
        {
            przyjete_zgloszenie++;
            czas_symulacji.wyswietl_czas(raport);
            raport << " Przyjeto zgloszenie nr " << przyjete_zgloszenie << endl;
        }
        //Obsluga zgloszen
        if (przyjete_zgloszenie>obsluzone_zgloszenie)
        {
            //Piorytetowe wyslanie smiglowca
            if (miasto->baza.pojazdy[4]->grafik[5]<czas_symulacji && przyjete_zgloszenie>obsluzone_zgloszenie && zgloszenie[obsluzone_zgloszenie].priorytet==1)
            {
                miasto->baza.pojazdy[4]->udziel_pomocy(zgloszenie[obsluzone_zgloszenie],czas_symulacji,miasto->szpital);
                liczba_interwencji[4]++;
                czas_symulacji.wyswietl_czas(raport);
                raport << " Helikopter wylatuje do zgloszenia nr " << miasto->baza.pojazdy[4]->nr_zgloszenia << " (priorytet)" << endl;
                obsluzone_zgloszenie++;
                miasto->baza.pojazdy_w_bazie--;
            }

            //Obsluga regularnych zgloszen
            for (int i=0; i<5; i++)
            {
                if (miasto->baza.pojazdy[i]->grafik[5]<czas_symulacji && przyjete_zgloszenie>obsluzone_zgloszenie)
                {
                    miasto->baza.pojazdy[i]->udziel_pomocy(zgloszenie[obsluzone_zgloszenie],czas_symulacji,miasto->szpital);
                    liczba_interwencji[i]++;
                    czas_symulacji.wyswietl_czas(raport);
                    if (miasto->baza.pojazdy[i]->typ==0)
                        raport << " Ambulans " << i+1 << " wyjezdza do zgloszenia nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << endl;
                    else
                        raport << " Helikopter wylatuje do zgloszenia nr " << miasto->baza.pojazdy[i]->nr_zgloszenia <<endl;
                    obsluzone_zgloszenie++;
                    miasto->baza.pojazdy_w_bazie--;
                }
            }
        }

        //Wyswietlanie statusu pojazdow
        for (int i=0; i<5; i++)
        {
            if (miasto->baza.pojazdy[i]->grafik[1]==czas_symulacji && zgloszenie[miasto->baza.pojazdy[i]->nr_zgloszenia-1].status==1)
            {
                czas_symulacji.wyswietl_czas(raport);
                if (miasto->baza.pojazdy[i]->typ==0)
                    raport << " Ambulans " << i+1 << " udziela pomocy rannemu nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << endl;
                else
                    raport << " Helikopter udziela pomocy rannemu nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << endl;
            }
            else if (miasto->baza.pojazdy[i]->grafik[1]==czas_symulacji && zgloszenie[miasto->baza.pojazdy[i]->nr_zgloszenia-1].status==-1)
            {
                czas_symulacji.wyswietl_czas(raport);
                if (miasto->baza.pojazdy[i]->typ==0)
                    raport << " Ambulans " << i+1 << " stwierdza zgon rannego nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << endl;
                else
                    raport << " Helikopter stwierdza zgon rannego nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << endl;
            }
            else if (miasto->baza.pojazdy[i]->grafik[3]==czas_symulacji && zgloszenie[miasto->baza.pojazdy[i]->nr_zgloszenia-1].status==1)
            {
                czas_symulacji.wyswietl_czas(raport);
                if (miasto->baza.pojazdy[i]->typ==0)
                    raport << " Ambulans " << i+1 << " dostarczyl rannego nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << " do szpitala" << endl;
                else
                    raport << " Helikopter dostarczyl rannego nr " << miasto->baza.pojazdy[i]->nr_zgloszenia << " do szpitala" << endl;
            }
            else if (miasto->baza.pojazdy[i]->grafik[4]==czas_symulacji)
            {
                czas_symulacji.wyswietl_czas(raport);
                if (miasto->baza.pojazdy[i]->typ==0)
                    raport << " Ambulans " << i+1 << " wrocil do bazy" << endl;
                else
                    raport << " Helikopter wrocil do bazy" << endl;
                miasto->baza.pojazdy_w_bazie++;
            }
        }
        czas_symulacji=czas_symulacji+1;
    }
    generuj_statystyki();
    raport.close();
}

void symulacja_typ::generuj_statystyki()
{
    if (mapa_zdarzen!=NULL)
    {
    for (int i=0; i<(miasto->n); i++)
        delete [] mapa_zdarzen[i];
    delete [] mapa_zdarzen;
    }
    mapa_zdarzen=new char*[miasto->n];
    for (int i=0; i<(miasto->n); i++)
        mapa_zdarzen[i]=new char[miasto->m];
    for (int i=0; i<(miasto->n); i++)
        for (int j=0; j<(miasto->m); j++)
            mapa_zdarzen[i][j]=miasto->plan_miasta[i][j];
    liczba_zgonow=0;
    czas_do_przybycia_pomocy=0;
    for (int i=0; i<zgloszenie.size(); i++)
    {
        if (zgloszenie[i].status==-1)
        {
            liczba_zgonow++;
            mapa_zdarzen[zgloszenie[i].lokalizacja.y][zgloszenie[i].lokalizacja.x]='X';
        }
        else
        {
            mapa_zdarzen[zgloszenie[i].lokalizacja.y][zgloszenie[i].lokalizacja.x]='!';
        }
        czas_do_przybycia_pomocy=czas_do_przybycia_pomocy+(zgloszenie[i].nadeszla_pomoc-zgloszenie[i].zdarzenie);

    }
    czas_do_przybycia_pomocy=czas_do_przybycia_pomocy/zgloszenie.size();
}

void symulacja_typ::wyswietl_statystyki()
{
    cout << "Liczba wezwan pomocy: " << zgloszenie.size() << endl;
    cout << "Liczba wezwan obsluzonych na czas: " << zgloszenie.size()-liczba_zgonow << endl;
    cout << "Liczba zgonow: " << liczba_zgonow << endl;
    cout << "Sredni czas oczekiwania na pomoc: " << fixed << setprecision(1) << czas_do_przybycia_pomocy << "min" << endl;
    for (int i=0; i<4; i++)
        cout << "Liczba interwencji ambulansu " << i+1 << ": " << liczba_interwencji[i] << endl;
    cout << "Liczba interwencji helikoptera: " << liczba_interwencji[4] << endl;
}

void symulacja_typ::wyswietl_raport()
{
    string linia;
    raport.open("raport.txt",ios::in);
    while (getline(raport,linia))
        cout << linia << endl;
    raport.close();
}

void symulacja_typ::wyswietl_zalozenia()
{
    cout << "Zalozenia symulacji:" << endl;
    cout << "1. Wezwania obslugiwane sa w kolejnosci zgloszen." << endl;
    cout << "2. 50% wezwan pochodzi z terenu o gestej zabudowie." << endl;
    cout << "3. Miasto posiada 1 szpital i 1 baze ratownictwa." << endl;
    cout << "4. Baza ratownictwa posiada 4 ambulanse i 1 helikopter." << endl;
    cout << "5. Do zgloszen priorytetowych w pierwszej kolejnosci wysylany jest helikopter." << endl;
    cout << "6. Do pozostalych zgloszen w pierwszej kolejnosci wysylany jest ambulans." << endl;
    cout << "7. Droga pojazdu ratowniczego przy obsludze wezwania:" << endl;
    cout << "   baza -> miejsce zdarzenia (udzelanie pomocy) -> szpital -> baza (serwis)" << endl;
    cout << "8. Ambulans z punktu (0,0) do punktu (x,y) jedzie x+y minut." << endl;
    cout << "9. Helikopter z punktu (0,0) do punktu (x,y) leci ((x^2+y^2)^(1/2))/2 minut." << endl;
}

void symulacja_typ::rysuj_mape_zdarzen()
{
    HANDLE hOut; //do kolorowania
    hOut = GetStdHandle( STD_OUTPUT_HANDLE );
    for (int i=0; i<(miasto->n); i++)
    {
        for (int j=0; j<(miasto->m); j++)
        {
            if (mapa_zdarzen[i][j]=='X') SetConsoleTextAttribute( hOut, FOREGROUND_RED | FOREGROUND_INTENSITY );
            if (mapa_zdarzen[i][j]=='!') SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
            if (mapa_zdarzen[i][j]=='S' || mapa_zdarzen[i][j]=='B') SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
            cout << mapa_zdarzen[i][j];
            SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
        }
        cout << endl;
    }
}
#endif
