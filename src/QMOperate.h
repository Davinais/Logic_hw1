#ifndef _QM_OPERATE_H
#define _QM_OPERATE_H

#include <vector>
#include "QMNode.h"

typedef std::vector< std::vector<QMNode> > QMTable;
typedef std::vector< std::vector<QMNode*> > QMTableP;

void printInput(std::vector<int>*, std::vector<int>*);
void printInitial(QMTable*);
QMTable simplify(QMTable, int);
QMTableP piChart(QMTable*, std::vector<int>, int);
QMTable petrickMethod(QMTable, QMTableP, int);
void printFinal(QMTable, int);

#endif
