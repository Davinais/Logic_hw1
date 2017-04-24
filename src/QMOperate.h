#ifndef _QM_OPERATE_H
#define _QM_OPERATE_H

#include <vector>
#include "QMNode.h"

typedef std::vector< std::vector<QMNode> > QMTable;
typedef std::vector< std::vector<QMNode*> > QMTableP;

void printInitial(QMTable*);
QMTable simplify(QMTable, int);
QMTableP piChart(QMTable*, std::vector<int>, int);
void printFinal(std::vector<QMNode>, int);

#endif
