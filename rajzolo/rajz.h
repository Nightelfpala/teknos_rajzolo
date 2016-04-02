#ifndef RAJZ_H_INCLUDED
#define RAJZ_H_INCLUDED

#include <stack>

#include "SDL.h"

#include "l_rendszer.h"

class teknos
{
public:
    teknos( std::string nev = "Rajzablak", int szel = 800, int ma = 600 );

    bool toll();    //  vonalhuzas ki-bekapcsolasa - alapon be van kapcsolva
    void elore(double dist);	//	a kivalasztott iranyba elorehalad (es vonalat huz, ha be van kapcsolva)
    void lep(double dist);          // rajzolas nelkul
    void hatra(double dist);	// hatrafele halad
    void hatralep(double dist);     // rajzolas nelkul
    void fordj(double deg);	//	jobbra fordul a megadott fokszammal
    void fordb(double deg);	//	balra fordul a megadott fokszammal
    void setpoz(double px, double py, double ir);	// beallitja a teknos poziciojat es iranyat
    void torolkep();

    void kirajz(std::string str, double indist, double indeg);
    void kirajz(std::string str, double indist, double indeg, double hmul);

    bool changeRajz();  // rajzolas kesleltetes ki-bekapcsolasa - alapon ki van kapcsolva
    bool changeRajz(bool b);
    void forceRajz();   // kesleltetes eseten a jelenleg tarolt abra kirajzolasa

    ~teknos();
private:
    struct allapot
    {
        double x, y, ir;
        allapot(){}
        allapot(double a, double b, double c);
    };

    double toRad(double alpha) const;
	void init();

    const int kep_sz;
    const int kep_ma;
    static int futo_szam;
    std::string ablaknev;
    SDL_Window *ablak;  //  alap kepmeret
    SDL_Renderer *rajzolo;

    double poz_x;
    double poz_y;
    double irany;   // fokban tarolva
    bool toll_le;
    bool frajz;

    std::stack<allapot> mentes;
};

#endif // RAJZ_H_INCLUDED
