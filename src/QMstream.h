#ifndef _QMSTREAM_H
#define _QMSTREAM_H

#include <iostream>
#include <fstream>
struct SFout : std::ofstream
{
    SFout(const std::string& fileName)
       : std::ofstream(fileName)
       , fileName(fileName)
    {};

    const std::string fileName;
};
/*template <class T>
SFout& operator<<(SFout&, const T&);
SFout& operator<<(SFout&, std::ostream& (*pfun)(std::ostream&));*/
#endif