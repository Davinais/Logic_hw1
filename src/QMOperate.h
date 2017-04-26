#ifndef _QM_OPERATE_H
#define _QM_OPERATE_H

#include <vector>
#include "QMNode.h"
#include "QMstream.h"

typedef std::vector< std::vector<QMNode> > QMTable;
typedef std::vector< std::vector<QMNode*> > QMTableP;

void printInput(std::vector<int>*, std::vector<int>*, SFout&);
void printInitial(QMTable*, SFout&);
QMTable simplify(QMTable, int, SFout&);
QMTableP piChart(QMTable*, std::vector<int>, int, SFout&);
QMTable petrickMethod(QMTable, QMTableP, int);
void printFinal(QMTable, int, SFout&);

#endif
