#ifndef _QM_OPERATE_H
#define _QM_OPERATE_H

#include <vector>
#include "QMNode.h"
#include "QMstream.h"

typedef std::vector<int> MinTerm;
typedef std::vector< std::vector<QMNode> > QMTable;
typedef std::vector< std::vector<QMNode*> > QMTableP;

void printInput(MinTerm*, MinTerm*, SFout&);
void printInitial(QMTable*, SFout&);
QMTable simplify(QMTable, int, SFout&);
QMTableP piChart(QMTable*, MinTerm, int, SFout&);
QMTable petrickMethod(QMTable, QMTableP, int, SFout&);
void printFinal(QMTable, int, SFout&);

#endif
