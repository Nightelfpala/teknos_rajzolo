#include <iostream>
#include <cmath>    //pow
#include <limits>   //numeric_limits    -   ne zarjon be azonnal a konzolablak

#include "rajz.h"
#include "l_rendszer.h"

using namespace std;


void beKoch(int n, double d, teknos &tek);
void kiKoch(int n, double d, teknos &tek);
void Koch(int n, double d, teknos &tek, bool ir = true);
void Cantor(int n, double d, teknos &tek);
void Sajat(teknos &tek);

int main(int argc, char *argv[])
{

    cout << "Adja meg a kivalasztott program betujelet!" << endl;
    cout << "k\tKoch-gorbe haromszogon kifele" << endl;
    cout << "b\tKoch-gorbe haromszogon befele" << endl;
    cout << "l\tLindenmayer-modszerrel fraktalmegadas" << endl;

    cout << endl;
    char c;
    cin >> c;

    teknos A("rajzolo");
    A.changeRajz();
    int n; double d; double f = 0; double h = 0;
    int db; lrendszer L;
    char ch; string filenev; string allapot;

    switch (c)
    {
    case 'k': case 'b':
        cout << "Adja meg az iteracio szamot:\t";
        cin >> n;
        cout << "Adja meg a haromszog oldalhosszat:\t";
        cin >> d;
        if (c == 'b')
            beKoch(n, d, A);
        else
            kiKoch(n, d, A);
        break;
    case 'l':
        cout << "Utasitasok (kisbetu-nagybetu szamit):" << endl
            << "F\telorelepes rajzolassal" << endl
            << "f\telorelepes rajzolas nelkul" << endl
            << "B\thatralepes rajzolassal" << endl
            << "b\thatralepes rajzolas nelkul" << endl
            << "+\tjobbra fordulas" << endl
            << "-\tbalra fordulas" << endl
            << "[\taktualis pozicio eltarolasa a verembe" << endl
            << "]\tverembol pozicio betoltes" << endl
            << "H\tlepes hossz szorzasa a megadott arannyal" << endl
            << "h\tlepes hossz osztasa a megadott arannyal" << endl << endl;

        cout << "Filebol akar beolvasni? (Y/N)" << endl;
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            cout << "Adja meg a file nevet!" << endl;
            cin >> filenev;
            ifstream befile(filenev.c_str());
            if (befile.fail())
            {
                cout << "HIBA: A megadott fajl nem talalhato!" << endl
                    << "Nyomjon ENTERt kilepeshez!" << endl;
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin.get();
                return -1;
            }
            L.fromfile(befile);
            //formatum:
                // axioma
                // szabaly szam
                // szabalyszamszor ismetles:
                    // szabaly bal oldal
                    // szabaly jobb oldal
                // elfordulas szoge
                // hossz valtozas aranya
            befile >> f;
            befile >> h;
        } else
        {
            cout << "Adja meg az axiomat (szokozt nem tartalmazhat)!" << endl;
            string axiom;
            cin >> axiom;
            L.start(axiom);
            cout << "Adja meg a szabalyok szamat:\t" << endl;
            cin >> db;
            for (int i = 0; i < db; ++i)
            {
                cout << "Adja meg az " << i + 1 << ". szabaly kezdoszimbolumat!" << endl;
                char st;
                cin >> st;
                cout << "Adja meg az " << i + 1 << ". szabaly behelyettesiteset (szokozt nem tartalmazhat)!" << endl;
                string beh;
                cin >> beh;
                L.szabaly(st, beh);
            }
        }
        cout << "Adja meg az iteracio szamot:\t";
        cin >> n;
        L.helyett(n);
        allapot = L.allapot();
        cout << "Adja meg egy lepes hosszat:\t";
        cin >> d;
        if ( f == 0 && (allapot.find('+') != string::npos || allapot.find('-') != string::npos))
        {
            cout << "Adja meg az elfordulas merteket:\t";
            cin >> f;
        }
        if ( h == 0 && (allapot.find('h') != string::npos || allapot.find('H') != string::npos))
        {
            cout << "Adja meg a hossz valtoztatasi aranyt:\t";
            cin >> h;
        }
        if (h != 0)
        {
            A.kirajz(L.allapot(), d, f, h);
        } else
        {
            A.kirajz(L.allapot(), d, f);
        }
        A.forceRajz();
        break;
    }

    {
        teknos B("rand");   // ha ez nincs itt (ki van kommentezve), akkor nem rajzolodik ki az 'A' abraja sem
    }

    A.changeRajz();
    cout << "Kilepni a rajzoloablakon a 'q', illetve Escape gombok megnyomasaval lehet." << endl;

    bool quit = false;
    SDL_Event esemeny;

    while ( !quit )
    {
        while (SDL_PollEvent(&esemeny))
        {
            switch (esemeny.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch ( esemeny.key.keysym.sym )
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                //
                }
            }
        }
    }

    return 0;
}

void beKoch(int n, double d, teknos &tek)
{
    for (int i = 0; i < 3; ++i)
    {
        Koch(n, d, tek, false);
        tek.fordj(120);
    }
}

void kiKoch(int n, double d, teknos &tek)
{
    for (int i = 0; i < 3; ++i)
    {
        Koch(n, d, tek);
        tek.fordj(120);
    }
}

void Koch(int n, double d, teknos &tek, bool ir)
{
    double f;
    if (ir)
        f = 60;
    else
        f = -60;
    if (n == 1)
    {
        tek.elore(d);
    } else
    {
        Koch( n - 1, d / 3, tek, ir);
        tek.fordb( f );
        Koch( n - 1, d / 3, tek, ir);
        tek.fordj( 2 * f);
        Koch( n - 1, d / 3, tek, ir);
        tek.fordb( f );
        Koch( n - 1, d / 3, tek, ir);
    }
}

void Cantor(int n, double d, teknos &tek)
{
    if (n == 1)
    {
        tek.elore(d);
    } else
    {
        Cantor( n - 1, d / 3, tek);
        tek.toll();
        tek.elore(d / 3);
        tek.toll();
        Cantor( n - 1, d / 3, tek);
    }
}

void Sajat(teknos &tek)
{
    lrendszer L1("FX");
    L1.szabaly('X', "[+FX][-FX]");
    L1.helyett(4);
    tek.kirajz(L1.allapot(), 600 / pow(2.0, 4), 30);
}
