#ifndef _QM_OPERATE_H
#define _QM_OPERATE_H

#include <vector>
#include "QMNode.h"

typedef std::vector< std::vector<QMNode> > QMTable;

void printInitial(QMTable*);
QMTable simplify(QMTable, int);

#endif
