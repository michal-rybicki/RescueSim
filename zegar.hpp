#ifndef zegar_hpp
#define zegar_hpp

#include <iostream>
#include <fstream> //do obslugi plikow

using namespace std;

class zegar
{
private:
    int liczba_dni[13];
public:
    //POLA:
    int dzien, miesiac, rok, godziny, minuty;

    //METODY:
    zegar(int godziny0, int minuty0, int dzien0, int miesiac0, int rok0); //konstruktor
    bool rok_przestepny(); //zwraca 1 dla roku przestepnego
    void ustaw_date(string data); //data moze byc w formacie mm-dd-rrr lub mm/dd/rrrr
    void ustaw_czas(string czas); //czas w formacie hh?mm lub h:mm
    void ustaw_czas(char czas[]); //czas w formacie hh?mm lub h:mm
    void ustaw_zegar(int godziny0, int minuty0, int dzien0, int miesiac0, int rok0);
    void wyswietl_czas(); //wyswietla na cout godzine w formacie hh:mm
    void wyswietl_czas(fstream& plik); //dopisuje do pliku godzine w formacie hh:mm
    void czas(); //wyswietla na cout godzine w formacie hh:mm
    void wyswietl_date(char separator); //wyswietla na cout date w formacie ddxmmxyyyy, gdzie x to separator (domyslnie '-')
    string dzien_tygodnia(); //oddaje dwuliterowy skrot od nazwy dnia tygodnia
    zegar operator+(int plus_minuty);
    bool operator<(zegar zegar2);
    bool operator>=(zegar zegar2);
    bool operator>(zegar zegar2);
    bool operator<=(zegar zegar2);
    bool operator==(zegar zegar2);
    int operator-(zegar zegar2);
    void dodaj_dzien();
};

zegar::zegar(int godziny0=0, int minuty0=0, int dzien0=1, int miesiac0=1, int rok0=2000)
{
    dzien=dzien0;
    miesiac=miesiac0;
    rok=rok0;
    godziny=godziny0;
    minuty=minuty0;
    int liczba_dni0[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    for (int i=0; i<13; i++) liczba_dni[i]=liczba_dni0[i];
}

bool zegar::rok_przestepny()
{
    if (rok%400==0 || (rok%4==0 && rok%100!=0))
        return 1;
    else
        return 0;
}

void zegar::ustaw_date(string data) //data moze byc w formacie mm-dd-rrr lub mm/dd/rrrr
    {
        char r[5], m[3], d[3];
        for (int i=6; i<10; i++)
        {
            r[i-6]=data[i];
        }
        r[5]='\0';
        m[2]='\0';
        d[2]='\0';
        for (int i=0; i<2; i++)
        {
            d[i]=data[i];
            m[i]=data[i+3];
        }
        rok=atoi(r), miesiac=atoi(m), dzien=atoi(d);
        return;
    }

void zegar::ustaw_czas(string czas)
{
    if (czas[2]==':' || czas[2]=='.')
    {
    char mm0[3], hh0[3];
    mm0[0]=czas[3];
    mm0[1]=czas[4];
    hh0[0]=czas[0];
    hh0[1]=czas[1];
    mm0[2]='\0';
    hh0[2]='\0';
    minuty=atoi(mm0);
    godziny=atoi(hh0);
    }
    else
    {
    char mm0[3], hh0[3];
    mm0[0]=czas[2];
    mm0[1]=czas[3];
    hh0[0]=czas[0];
    mm0[2]='\0';
    hh0[1]='\0';
    minuty=atoi(mm0);
    godziny=atoi(hh0);
    }
    return;
}

void zegar::ustaw_czas(char czas[])
{
    if (czas[2]==':' || czas[2]=='.')
    {
    char mm0[3], hh0[3];
    mm0[0]=czas[3];
    mm0[1]=czas[4];
    hh0[0]=czas[0];
    hh0[1]=czas[1];
    mm0[2]='\0';
    hh0[2]='\0';
    minuty=atoi(mm0);
    godziny=atoi(hh0);
    }
    else
    {
    char mm0[3], hh0[3];
    mm0[0]=czas[2];
    mm0[1]=czas[3];
    hh0[0]=czas[0];
    mm0[2]='\0';
    hh0[1]='\0';
    minuty=atoi(mm0);
    godziny=atoi(hh0);
    }
    return;
}

void zegar::ustaw_zegar(int godziny0=0, int minuty0=0, int dzien0=1, int miesiac0=1, int rok0=2000)
{
    dzien=dzien0;
    miesiac=miesiac0;
    rok=rok0;
    godziny=godziny0;
    minuty=minuty0;
}

void zegar::wyswietl_czas()
{
    if (godziny<10) cout << "0";
    cout << godziny << ":";
    if (minuty<10) cout << "0";
    cout << minuty;
    return;
}

void zegar::wyswietl_czas(fstream& plik)
{
    if (godziny<10) plik << "0";
    plik << godziny << ":";
    if (minuty<10) plik << "0";
    plik << minuty;
    return;
}

void zegar::wyswietl_date(char separator='-')
{
    if (dzien<10) cout << "0";
    cout << dzien << separator;
    if (miesiac<10) cout << "0";
    cout << miesiac << separator;
    if (rok<1000) cout << "0";
    if (rok<100) cout << "0";
    if (rok<10) cout << "0";
    cout << rok;
    return;
}

string zegar::dzien_tygodnia()
{
    string wynik="PN";
    int Y=(rok-1)%100;
    int C=(rok-1)-Y;
    int G=(Y+Y/4);
    int dzien_tygodnia_1_stycznia=(((((C/100)%4)*5)+G)%7);
    int liczba_dni_od_1_sty[13]={0,0,31,59,90,120,151,181,212,243,273,304,334};
    int dzien_roku=liczba_dni_od_1_sty[miesiac]+dzien;
    if (rok_przestepny() && miesiac>2) dzien_roku++;
    int dzien_tyg=(dzien_tygodnia_1_stycznia+dzien_roku-1)%7;
    if (dzien_tyg==1) wynik="WT";
    else if (dzien_tyg==2) wynik="SR";
    else if (dzien_tyg==3) wynik="CZ";
    else if (dzien_tyg==4) wynik="PT";
    else if (dzien_tyg==5) wynik="SB";
    else if (dzien_tyg==6) wynik="ND";
    return wynik;
}

zegar zegar::operator+(int plus_minuty)
{
    zegar zegar1(godziny,minuty,dzien,miesiac,rok);
    int M=plus_minuty, hh=godziny, mm=minuty;
    while (M>0)
    {
        if (M>1440)
            M=M-1440;
        else if (M>60)
        {
            if (hh<23)
                hh++;
            else
                {
                    hh=0;
                    zegar1.dodaj_dzien();
                }
            M=M-60;
        }
        else if (M>(60-mm))
        {
            if (hh<23)
                hh++;
            else
                {
                    hh=0;
                    zegar1.dodaj_dzien();
                }
            mm=M-(60-mm);
            M=0;
        }
        else
        {
            mm=mm+M;
            M=0;
        }
    }
    if (mm==60)
    {
        mm=0;
        if (hh<23)
            hh++;
        else
        {
            hh=0;
            zegar1.dodaj_dzien();
        }

    }
    zegar1.godziny=hh;
    zegar1.minuty=mm;
    return zegar1;
}

bool zegar::operator<(zegar zegar2)
{
         if (rok<zegar2.rok) return 1;
    else if (rok>zegar2.rok) return 0;
    else if (miesiac<zegar2.miesiac) return 1;
    else if (miesiac>zegar2.miesiac) return 0;
    else if (dzien<zegar2.dzien) return 1;
    else if (dzien>zegar2.dzien) return 0;
    else if (godziny<zegar2.godziny) return 1;
    else if (godziny>zegar2.godziny) return 0;
    else if (minuty<zegar2.minuty) return 1;
    else if (minuty>=zegar2.minuty) return 0;
}

bool zegar::operator>=(zegar zegar2)
{
    if (*this<zegar2) return 0;
    else return 1;
}

bool zegar::operator>(zegar zegar2)
{
         if (rok>zegar2.rok) return 1;
    else if (rok<zegar2.rok) return 0;
    else if (miesiac>zegar2.miesiac) return 1;
    else if (miesiac<zegar2.miesiac) return 0;
    else if (dzien>zegar2.dzien) return 1;
    else if (dzien<zegar2.dzien) return 0;
    else if (godziny>zegar2.godziny) return 1;
    else if (godziny<zegar2.godziny) return 0;
    else if (minuty>zegar2.minuty) return 1;
    else if (minuty<=zegar2.minuty) return 0;
}

bool zegar::operator<=(zegar zegar2)
{
    if (*this>zegar2) return 0;
    else return 1;
}

bool zegar::operator==(zegar zegar2)
{
    if (!(*this<zegar2) && !(*this>zegar2)) return 1;
    else return 0;
}

int zegar::operator-(zegar zegar2)
{
    int m1=minuty;
    int h1=godziny;
    int m0=zegar2.minuty;
    int h0=zegar2.godziny;
    int L=0;

    if ((h0*60+m0)<=(h1*60+m1))
    {
        L=h1*60+m1-h0*60-m0;
    }
    else
    {
        L=h1*60+m1+(1440-h0*60-m0);
    }

    return L;
}

void zegar::dodaj_dzien()
{
    if (rok_przestepny() && miesiac==2 && dzien==28) dzien++;
    else if (dzien<liczba_dni[miesiac]) dzien++;
    else if (miesiac<12)
    {
        miesiac++;
        dzien=1;
    }
    else
    {
        rok++;
        miesiac=1;
        dzien=1;
    }
    return;
}

class kolejka_zegarow
{
private:
    struct wezel{
    int nr;
    zegar czas;
    wezel * nast;
    };
    int ostatni_nr;

public:
    wezel * pierwszy;
    wezel * ostatni;
    int ilosc;

    kolejka_zegarow();
    bool pusta();
    void dodaj(zegar nowy);
    zegar pobierz();

};

kolejka_zegarow::kolejka_zegarow()
{
    pierwszy=NULL;
    ostatni=NULL;
    ostatni_nr=0;
    ilosc=0;
    return;
}

bool kolejka_zegarow::pusta()
{
    if (pierwszy==NULL)
        return 1;
    else
        return 0;
}

void kolejka_zegarow::dodaj(zegar nowy)
{
    ostatni_nr++;
    ilosc++;
    wezel * nowy_wpis;
    nowy_wpis=new wezel;
    nowy_wpis->czas=nowy;
    nowy_wpis->nast=NULL;
    nowy_wpis->nr=ostatni_nr;
    if (!pusta()) ostatni->nast=nowy_wpis;
    ostatni=nowy_wpis;
    if (pusta()) pierwszy=ostatni;
    return;
}

zegar kolejka_zegarow::pobierz()
{
    zegar pobrany;
    if (pusta()) return pobrany;
    ilosc--;
    pobrany=pierwszy->czas;
    delete pierwszy;
    pierwszy=pierwszy->nast;
    return pobrany;
}
#endif
