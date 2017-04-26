#ifndef _QMIO_H
#define _QMIO_H

#include <string>
#include <vector>
#include "QMOperate.h"

int readMinTerm(QMTable*, std::vector<int>*, std::string);
std::vector<int> readDontCare(QMTable*, const std::vector<int>*, int, std::string);
#endif