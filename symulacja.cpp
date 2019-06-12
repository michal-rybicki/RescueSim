#include <iostream>
#include <cstdlib> //do losowania
#include <ctime> //do losowania
#include <conio.h> //do sterowania klawiszami
#include <windows.h>//Potrzebne do koloru.
#include <stdlib.h>//Potrzebne do koloru.
#include "zegar.hpp"
#include "ratownictwo.hpp"

void koloruj(int menu,int wiersz,HANDLE hOut)
{
    if (menu==wiersz)
        SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE );
    else
        SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
}

using namespace std;
main()
{
    char wybor='Q';
    int wybrane_miasto=1;
    int menu=1, wyjscie=0, wybrany_widok=1;
    srand(time(NULL)); //do losowania
    HANDLE hOut; //do kolorowania
    hOut = GetStdHandle( STD_OUTPUT_HANDLE ); //do kolorowania
    koloruj(0,1,hOut); //kolorowanie
    miasto_bazowe * mapa_swiata[3];
    mapa_swiata[0]=new miasto_okragle;
    mapa_swiata[1]=new miasto_skosne;
    mapa_swiata[2]=new miasto_krzyzowe;

    //--------MENU--------------------------------------------------------------------------------------------------------------
    do
    {
        wybor='Q';
        menu=1;
        while (tolower(wybor)!='e')
        {
            system( "cls" ); //czyszczenie ekranu
            koloruj(menu,1,hOut);
            cout << "Rozpocznij symulacje" << endl;
            koloruj(menu,2,hOut);
            cout << "Zalozenia symulacji" << endl;
            koloruj(menu,3,hOut);
            cout << "Wyjdz" << endl;
            koloruj(0,1,hOut);

            wybor=getch();

            if ((wybor=='D' || wybor=='d' || wybor=='S' || wybor=='s') && menu<3)
                menu++;
            else if ((wybor=='A' || wybor=='a' || wybor=='W' || wybor=='w') && menu>1)
                menu--;
        }

        //----SYMULACJA------------------------------------------------------------------------------------------
        if (menu==1)

        {
        //----USTAWIANIE WARUNKOW SYMULACJI------------------------------------------------------------------------------------------
            wybor='Q';
            menu=1;
            while (tolower(wybor)!='e')
            {
                system( "cls" ); //czyszczenie ekranu
                cout << "Wybierz miasto: " << endl;
                mapa_swiata[wybrane_miasto-1]->rysuj_plan_miasta();
                cout << "Legenda:" << endl << "'+' - gesta zabudowa" << endl << "'-' - luzna zabudowa" << endl;

                wybor=getch();

                if ((wybor=='D' || wybor=='d' || wybor=='S' || wybor=='s') && wybrane_miasto<3)
                    wybrane_miasto++;
                else if ((wybor=='A' || wybor=='a' || wybor=='W' || wybor=='w') && wybrane_miasto>1)
                    wybrane_miasto--;
            }
            mapa_swiata[wybrane_miasto-1]->ustaw_szpital();
            mapa_swiata[wybrane_miasto-1]->ustaw_baze();
            symulacja_typ symulacja(mapa_swiata[wybrane_miasto-1]);

        //----WYKONYWANIE SYMULACJI--------------------------------------------------------------------------------------------

            symulacja.start();

        //----WYSWIETLANIE WYNIKOW--------------------------------------------------------------------------------------------

            menu=1, wybrany_widok=2;
            do
            {
                system( "cls" ); //czyszczenie ekranu

                if (wybrany_widok==2)
                {
                    cout << "Statystyki:" << endl;
                    symulacja.wyswietl_statystyki();
                }
                else if (wybrany_widok==3)
                {
                    cout << "Raport:" << endl;
                    symulacja.wyswietl_raport();
                }
                else if (wybrany_widok==4)
                {
                    cout << "Mapa zdarzen:" << endl;
                    symulacja.rysuj_mape_zdarzen();
                }
                cout << endl;
                koloruj(menu,1,hOut);
                cout << "Powtorz symulacje" << endl;
                koloruj(menu,2,hOut);
                cout << "Statystyki" << endl;
                koloruj(menu,3,hOut);
                cout << "Raport" << endl;
                koloruj(menu,4,hOut);
                cout << "Mapa zdarzen" << endl;
                koloruj(menu,5,hOut);
                cout << "Nowa symulacja" << endl;
                koloruj(menu,6,hOut);
                cout << "Wyjdz" << endl;
                koloruj(0,1,hOut);

                wybor=getch();

                if ((wybor=='D' || wybor=='d' || wybor=='S' || wybor=='s') && menu<6)
                    menu++;
                else if ((wybor=='A' || wybor=='a' || wybor=='W' || wybor=='w') && menu>1)
                    menu--;
                else if (tolower(wybor)=='e' && menu==1)
                    symulacja.start();
                else if (tolower(wybor)=='e' && menu>1)
                    wybrany_widok=menu;
            }
            while (wybrany_widok<5);
            if (wybrany_widok==6) wyjscie=1;
        }
        //----WYSWIETLANIE ZALOZEN SYMULACJI------------------------------------------------------------------------------------------
        else if (menu==2)
        {
            wybor='Q';
            menu=1;
            symulacja_typ symulacja(mapa_swiata[wybrane_miasto-1]);
            while (tolower(wybor)!='e')
            {
                system( "cls" ); //czyszczenie ekranu
                symulacja.wyswietl_zalozenia();
                cout << endl;
                koloruj(menu,1,hOut);
                cout << "Powrot" << endl;
                koloruj(menu,2,hOut);
                cout << "Wyjdz" << endl;
                koloruj(0,1,hOut);

                wybor=getch();

                if ((wybor=='D' || wybor=='d' || wybor=='S' || wybor=='s') && menu<2)
                    menu++;
                else if ((wybor=='A' || wybor=='a' || wybor=='W' || wybor=='w') && menu>1)
                    menu--;
            }
            if (menu==2) wyjscie=1;
        }
        else if (menu==3)
        {
            wyjscie=1;
        }
    }
    while (wyjscie==0);
    system( "cls" ); //czyszczenie ekranu
    return 0;
}

