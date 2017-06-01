#include "QMNode.h"
#include "QMOperate.h"
#include "QMio.h"
#include "QMstream.h"
using namespace std;

int main()
{
    QMTable qm;
    MinTerm minTerm;
    int var_num = readMinTerm(&qm, &minTerm, "input.txt");
    MinTerm dontCare = readDontCare(&qm, &minTerm, var_num, "input2.txt");
    SFout out("output.txt");
    printInput(&minTerm, &dontCare, out);
    printInitial(&qm, out);
    qm = simplify(qm, var_num, out);
    QMTableP minTermCount = piChart(&qm, minTerm, var_num, out);
    QMTable select = petrickMethod(qm, minTermCount, var_num, out);
    printFinal(select, var_num, out);
    return 0;
}
