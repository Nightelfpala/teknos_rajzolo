#include <iostream>
#include <cmath>

#include "rajz.h"

using namespace std;

int teknos::futo_szam = 0;

teknos::teknos( string nev, int szel, int ma) :
    kep_sz(szel), kep_ma(ma), ablaknev(nev), ablak(NULL), rajzolo(NULL)
{
    init();
}

void teknos::init()
{
    if ( futo_szam == 0 )
    {
        if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
        {
            cerr << "Hiba tortent az SDL inicializalasakor" << endl
                << "hibauzenet: " << SDL_GetError() << endl;
        }
    }
    ++futo_szam;
    if ( futo_szam > 0 )
    {
        ablak = SDL_CreateWindow(ablaknev.c_str(), 200, 200, kep_sz, kep_ma, SDL_WINDOW_SHOWN);
        if ( ablak == NULL )
        {
            cerr << "Hiba tortent az ablak elkesziteseskor" << endl
                << "hibauzenet: " << SDL_GetError() << endl;
        }
        rajzolo = SDL_CreateRenderer( ablak, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        torolkep();
        frajz = true;
    }
}

bool teknos::toll()
{
    return toll_le = !toll_le;
}

void teknos::elore(double dist)
    // a rajzolas ugy van megvalositva, hogy amennyiben egy ponton kilep a kepernyobol, akkor a tuloldalon a visszajon
{
    if (toll_le)
    {
        double tar_x = poz_x + sin(toRad(irany)) * dist;
        double tar_y = poz_y - cos(toRad(irany)) * dist;
        SDL_RenderDrawLine( rajzolo, (int)poz_x, (int)poz_y, (int)tar_x, (int)tar_y);
        double np_x = poz_x;
        double np_y = poz_y;
        double nt_x = tar_x;
        double nt_y = tar_y;
        while ((nt_x > kep_sz) || (nt_x < 0) || (nt_y > kep_ma) || (nt_y < 0))
        {
            bool yelobb;
            double x_tav = 0;
            double y_tav = 0;
            if ((nt_y > kep_ma) && ((irany != 90) || (irany != 270)))
            {
                y_tav = (kep_ma - np_y) / cos( toRad(180 - irany));
            } else if ((nt_y < 0) && ((irany != 90) || (irany != 270)))
            {
                y_tav = np_y / cos( toRad(irany) );
            }
            if ((nt_x > kep_sz) && ((irany != 0) || (irany != 180)))
            {
                x_tav = (kep_sz - np_x) / sin( toRad(irany) );
            } else if ((nt_x < 0) && ((irany != 0) || (irany != 180)))
            {
                x_tav = np_x / sin( toRad(360 - irany) );
            }
            yelobb = x_tav < y_tav;
            //cout << x_tav << "\t" << y_tav << "\t" << xelobb << endl;
            //cout << np_x << " " << np_y << " " << nt_x << " " << nt_y << endl;

            if ((nt_x > kep_sz) && (y_tav == 0 || yelobb )  )
            {
                SDL_RenderDrawLine( rajzolo, (int) (np_x - kep_sz) , (int)np_y, (int) (nt_x - kep_sz) , (int) nt_y);
                nt_x -= kep_sz;
                np_x -= kep_sz;
            } else if ((nt_x < 0) && (y_tav == 0 || yelobb ) )
            {
                SDL_RenderDrawLine( rajzolo, (int) (np_x + kep_sz) , (int)np_y, (int) (nt_x + kep_sz) , (int) nt_y);
                nt_x += kep_sz;
                np_x += kep_sz;
            }
            if (nt_y > kep_ma)
            {
                SDL_RenderDrawLine( rajzolo, (int)np_x, (int) (np_y - kep_ma), (int)nt_x, (int) (nt_y - kep_ma));
                nt_y -= kep_ma;
                np_y -= kep_ma;
            } else if (nt_y < 0)
            {
                SDL_RenderDrawLine( rajzolo, (int)np_x, (int) (np_y + kep_ma), (int)nt_x, (int) (nt_y + kep_ma));
                nt_y += kep_ma;
                np_y += kep_ma;
            }
            //cout << np_x << " " << np_y << " " << nt_x << " " << nt_y << endl;
        }
        poz_x = nt_x;
        poz_y = nt_y;

        if (frajz) SDL_RenderPresent( rajzolo );
    } else
    {
        lep(dist);
    }
}

void teknos::lep(double dist)
{
    double tar_x = poz_x + sin(toRad(irany)) * dist;
    double tar_y = poz_y - cos(toRad(irany)) * dist;
    while (tar_x > kep_sz ||  tar_x < 0 || tar_y > kep_ma || tar_y < 0)
    {
        if (tar_x > kep_sz)
        {
            tar_x -= kep_sz;
        } else if (tar_x < 0)
        {
            tar_x += kep_sz;
        }
        if (tar_y > kep_ma)
        {
            tar_y -= kep_ma;
        } else if (tar_y < 0)
        {
            tar_y += kep_ma;
        }
    }
    poz_x = tar_x;
    poz_y = tar_y;
}

void teknos::hatra(double dist)
{
    elore( - dist );
}

void teknos::hatralep(double dist)
{
    lep( - dist );
}

void teknos::fordj(double deg)
{
    irany += deg;
    while ((irany > 360 ) || ( irany < 0 ) )
    {
        if (irany > 360)
        {
            irany -= 360;
        }
        if (irany < 0)
        {
            irany += 360;
        }
    }
}

void teknos::fordb(double deg)
{
    fordj( - deg );
}

void teknos::setpoz(double px, double py, double ir)
{
    poz_x = px;
    poz_y = py;
    irany = ir;
}

void teknos::torolkep()
{
    SDL_SetRenderDrawColor( rajzolo, 255, 255, 255, 255 );
    SDL_RenderClear( rajzolo );
    SDL_SetRenderDrawColor( rajzolo, 0, 0, 0, 255 );

    SDL_RenderPresent( rajzolo );

    setpoz(kep_sz / 2, kep_ma * 3 / 4, 0);
    toll_le = true;
}

void teknos::kirajz(std::string str, double indist, double indeg)
{
    double dist = indist;
    double deg = indeg;
    for (unsigned int i = 0; i < str.size(); ++i)
    {
        switch (str[i])
        {
        case 'F' :
            elore(dist);
            break;
        case 'f' :
            lep(dist);
            break;
        case 'B' :
            hatra(dist);
            break;
        case 'b':
            hatralep(dist);
            break;
        case '+' :
            fordj(deg);
            break;
        case '-' :
            fordb(deg);
            break;
        case '[' :
            {
                allapot ap(poz_x ,poz_y, irany);
                mentes.push(ap);
                break;
            }
        case ']' :
            {
                allapot ap = mentes.top();
                mentes.pop();
                setpoz(ap.x, ap.y, ap.ir);
                break;
            }
        default:
            break;
        }
    }
}

void teknos::kirajz(std::string str, double indist, double indeg, double hmul)
{
    double dist = indist;
    double deg = indeg;
    for (unsigned int i = 0; i < str.size(); ++i)
    {
        switch (str[i])
        {
        case 'F' :
            elore(dist);
            break;
        case 'f' :
            lep(dist);
            break;
        case 'B' :
            hatra(dist);
            break;
        case 'b':
            hatralep(dist);
            break;
        case '+' :
            fordj(deg);
            break;
        case '-' :
            fordb(deg);
            break;
        case '[' :
            {
                allapot ap(poz_x ,poz_y, irany);
                mentes.push(ap);
                break;
            }
        case ']' :
            {
                allapot ap = mentes.top();
                mentes.pop();
                setpoz(ap.x, ap.y, ap.ir);
                break;
            }
        case 'H' :
            {
                dist = dist * hmul;
                break;
            }
        case 'h' :
            {
                dist = dist / hmul;
                break;
            }
        default:
            break;
        }
    }
}

bool teknos::changeRajz()
{
    if ((frajz = (!frajz)))
    {
        SDL_RenderPresent( rajzolo );
    }
    return frajz;
}

bool teknos::changeRajz(bool b)
{
    return (frajz = b);
}

void teknos::forceRajz()
{
    SDL_RenderPresent( rajzolo );
}

double teknos::toRad(double alpha) const
{
    return (alpha / 180 * M_PI);
}

teknos::allapot::allapot(double a, double b, double c) : x(a), y(b), ir(c)
{
    //null
}

teknos::~teknos()
{
    SDL_DestroyWindow( ablak );
    SDL_DestroyRenderer( rajzolo );
    --futo_szam;
    if (futo_szam == 0)
    {
        SDL_Quit();
    }
}
