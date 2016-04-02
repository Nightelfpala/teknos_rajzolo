#include <sstream>
#include "l_rendszer.h"

using namespace std;

lrendszer::lrendszer(std::string start) : allas(start), darab(0)
{
    //null
}

lrendszer::~lrendszer()
{
    //null
}

void lrendszer::start(std::string st)
{
    allas = st;
}

void lrendszer::szabaly(const char &ch, const std::string &str)
{
    behely[ch] = str;
}

void lrendszer::fromfile(std::ifstream &infile)
{
    string str;
    infile >> str;
    start(str);
    int sz;
    infile >> sz;
    for (int i = 0; i < sz; ++i)
    {
        char c;
        infile >> c;
        infile >> str;
        szabaly(c, str);
    }
}

void lrendszer::helyett(int n)
{
    for (int i = 0; i < n; ++i)
    {
        ++darab;
        iter();
    }
}

const int& lrendszer::hany() const
{
    return darab;
}

const std::string& lrendszer::allapot() const
{
    return allas;
}

void lrendszer::iter()
{
    stringstream ss;
    for (int i = 0; i < allas.size(); ++i)
    {
        if (behely.count(allas[i]) > 0)
        {
            ss << behely.at(allas[i]);
        } else
        {
            ss << allas[i];
        }
    }
    allas = ss.str();
}
