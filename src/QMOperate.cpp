#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <deque>
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
QMTable petrickMethod(QMTable qm, QMTableP minTermCount, int var_num)
{
    vector< vector<string> > toFinal;
    for(auto& stat : minTermCount)
    {
        vector<string> temp;
        for(auto& term : stat)
        {
            temp.push_back(term->getValue());
        }
        toFinal.push_back(temp);
    }
    deque<string> finalAns;
    while(!toFinal.empty())
    {
        vector<string> current = toFinal.back();
        if(finalAns.empty())
        {
            for(auto& item : current)
            {
                finalAns.push_back(item);
            }
        }
        else
        {
            int size = 0, should = finalAns.size()*current.size();
            while(size < should)
            {
                string adding = finalAns.front();
                for(auto& added : current)
                {
                    if(adding.find(added) == string::npos)
                        finalAns.push_back(adding + " " + added);
                    else
                        finalAns.push_back(adding);
                    size++;
                }
                finalAns.pop_front();
            }
            for(auto it = finalAns.begin(); it != finalAns.end(); it++)
            {
                if(!(*it).compare("NONEED"))
                    continue;
                vector<string> subterm;
                {
                    istringstream sub(*it);
                    string temp;
                    while(getline(sub, temp, ' '))
                    {
                        subterm.push_back(temp);
                    }
                }
                for(auto check = finalAns.begin(); check != finalAns.end(); check++)
                {
                    if(check == it)
                    {
                        string temp;
                        for(auto& dupli : subterm)
                        {
                            if(temp.find(dupli) == string::npos)
                            {
                                temp = temp + dupli + " ";
                            }
                        }
                        temp.pop_back();
                        *check = temp;
                    }
                    else
                    {
                        int contain = 0;
                        for(auto& dupli : subterm)
                        {
                            if((*check).find(dupli) != string::npos)
                            {
                                contain++;
                            }
                        }
                        if(contain >= subterm.size())
                        {
                            *check = "NONEED";
                        }
                    }
                }
            }
            for(auto it = finalAns.begin(); it != finalAns.end();)
            {
                if(!(*it).compare("NONEED"))
                    it = finalAns.erase(it);
                else
                    it++;
            }
        }
        toFinal.pop_back();
    }
    int minSize = -1;
    for(auto& term : finalAns)
    {
        if(minSize == -1)
            minSize = term.size();
        else if(term.size() < minSize)
            minSize = term.size();
    }
    map<string, QMNode> nodeName;
    for(auto& row : qm)
    {
        for(auto& term : row)
        {
            nodeName[term.getValue()] = term;
        }
    }
    QMTable select;
    for(auto& ans : finalAns)
    {
        if(ans.size() <= minSize)
        {
            stringstream ansin(ans);
            string temp;
            vector<QMNode> answer;
            while(getline(ansin, temp, ' '))
            {
                answer.push_back(nodeName[temp]);
            }
            select.push_back(answer);
        }
    }
    return select;
}
void printFinal(QMTable select, int var_num)
{
    for(auto& ans : select)
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
        for(auto it = ans.rbegin(); it != ans.rend();)
        {
            string var = it->varStr();
            if(var == "")
            {
                cout << " 1 " << endl;
                break;
            }
            cout << it->varStr();
            it++;
            if(it != ans.rend())
                cout << "+";
            else
                cout << endl;
        }
    }
}
