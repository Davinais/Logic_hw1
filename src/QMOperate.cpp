#include <iostream>
#include <iomanip>
#include <queue>
#include "QMOperate.h"
using namespace std;

void printInitial(QMTable* qm)
{
    cout << "Round 1" << endl
             << "==============================" << endl;
        for(auto& row : *qm)
        {
            if(!row.empty())
            {
                for(auto& col : row)
                {
                    cout << col << endl;
                }
                cout << "------------------------------" << endl;
            }
        }
    cout << endl;
}
QMTable simplify(QMTable qm, int var_num)
{
    int phase = 1;
    while(phase <= var_num*2) //忘了考慮最極限的情形
    {
        int wave = 1;
        queue<QMNode> mergeQueue;
        for(auto& select : qm)
        {
            for(auto& current : select)
            {
                if(current.getPhase() == phase)
                {
                    int wavesize = qm[wave].size();
                    for(int i = 0; i < wavesize; i++)
                    {
                        QMNode* compare = &qm[wave][i];
                        if(compare->getPhase() == phase)
                        {
                            if(current.canMerge(*compare))
                            {
                                mergeQueue.push(QMNode::mergeNode(&current, compare));
                            }
                        }
                    }
                }
            }
            if(++wave > qm.size()-1)
                break;
        }
        if(!mergeQueue.empty())
        {
            int count = -1;
            cout << "Round " << (phase>>1)+1+1 << endl //第一個加一補的是phase/2，第二個補的是外面被算過的Round 1
                 << "==============================" << endl;
            while(!mergeQueue.empty())
            {
                QMNode* getIn = &mergeQueue.front();
                int nowCount = getIn->count();
                if(count == -1) //初始化count
                    count = nowCount;
                bool duplicate = false;
                for(auto& iter : qm[nowCount])
                {
                    if(iter.compare(*getIn))
                    {
                        duplicate = true;
                        break;
                    }
                }
                if(!duplicate)
                {
                    qm[nowCount].push_back(*getIn);
                    if(nowCount != count)
                    {
                        cout << "------------------------------" << endl;
                        count = nowCount;
                    }
                    cout << *getIn << endl;
                }
                mergeQueue.pop();
            }
            cout << "------------------------------" << endl
                 << endl;
        }
        else
        {
            break;
        }
        phase <<= 1;
    }
    //移除不必要元素，即是已經被合併過的
    {
        for(auto& row : qm)
        {
            for(auto it = row.begin(); it != row.end();)
            {
                if(it->isMerged())
                {
                    it = row.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
    }
    return qm;
}
QMTableP piChart(QMTable* qm, vector<int> minTerm, int var_num)
{
    QMTableP minTermCount(minTerm.size(), vector<QMNode*>());
    cout << "Result" << endl
            << "==============================" << endl;
    int lspace = var_num * 2;
    cout << setw(lspace) << "" << "|";
    int columnWidth = 3;
    for(auto& need : minTerm)
    {
        cout << setw(columnWidth) << need;
    }
    cout << endl;
    cout << string(lspace, '-') << "|" << string(minTerm.size()*columnWidth, '-') << endl;
    for(auto& row : *qm)
    {
        for(auto& implicant : row)
        {
            cout << setw(lspace) << implicant.varStr() << "|";
            string varSelect(minTerm.size()*columnWidth, ' ');
            for(auto& num : implicant.getNumber())
            {
                int pos = -1;
                int varSize = (int)minTerm.size();
                for(int i = 0; i < varSize; i++)
                {
                    if(minTerm[i] == num)
                    {
                        pos = i+1;
                        minTermCount[i].push_back(&implicant);
                        break;
                    }
                }
                if(pos > 0)
                {
                    varSelect[pos*columnWidth-1] = 'x';
                }
            }
            cout << varSelect << endl;
        }
    }
    cout << endl;
    return minTermCount;
}
void printFinal(vector<QMNode> select, int var_num)
{
    cout << "F(";
    char var = 'A';
    for(int i=0; i<var_num;)
    {
        cout << var;
        i++;
        var++;
        if(i < var_num)
            cout << ",";
        else
            cout << ") = ";
    }
    for(auto it = select.begin(); it != select.end(); )
    {
        string var = it->varStr();
        if(var == "")
        {
            cout << " 1 " << endl;
            break;
        }
        cout << it->varStr();
        it++;
        if(it != select.end())
            cout << "+";
        else
            cout << endl;
    }
}
