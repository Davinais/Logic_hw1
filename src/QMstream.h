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
#endif
