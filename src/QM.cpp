#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "QMNode.h"
#include "QMOperate.h"
#include "QMio.h"
using namespace std;

int main()
{
    QMTable qm;
    vector<int> minTerm;
    int var_num = readMinTerm(&qm, &minTerm, "input.txt");
    vector<int> dontCare = readDontCare(&qm, &minTerm, var_num, "input2.txt");
    ofstream outFile;
    outFile.open("output.txt");
    printInput(&minTerm, &dontCare);
    printInitial(&qm);
    qm = simplify(qm, var_num);
    QMTableP minTermCount = piChart(&qm, minTerm, var_num);
    QMTable select = petrickMethod(qm, minTermCount, var_num);
    printFinal(select, var_num);
    return 0;
}
