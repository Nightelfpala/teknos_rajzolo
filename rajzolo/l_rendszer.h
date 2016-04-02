#ifndef L_RENDSZER_H_INCLUDED
#define L_RENDSZER_H_INCLUDED

#include <map>
#include <fstream>

class lrendszer
{
public:
    lrendszer(std::string start = "");
    ~lrendszer();

    void start(std::string st);
    void szabaly(const char &ch, const std::string &str);
    void fromfile(std::ifstream &infile);
        //formatum:
            // axioma
            // szabaly szam
            // szabalyszamszor:
                // szabaly bal oldal
                // szabaly jobb oldal
    void helyett(int n);
    const int& hany() const;

    const std::string& allapot() const;

private:
    std::map<char, std::string> behely;
    std::string allas;
    int darab;

    void iter();
};


#endif // L_RENDSZER_H_INCLUDED
