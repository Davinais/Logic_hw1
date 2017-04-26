#ifndef _QMSTREAMIM_H
#define _QMSTREAMIM_H

#include <iostream>
#include <fstream>
#include "QMstream.h"

template <class T>
SFout& operator<<(SFout& fm, T const& var)
{
    std::cout << var;
    static_cast<std::ofstream&>(fm) << var;
    return fm;
}
SFout& operator<<(SFout& fm, std::ostream& (*pfun)(std::ostream&))
{
    pfun(fm);
    pfun(std::cout);
    return fm;
}
#endif