#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include <deque>
#include <queue>
#include "QMOperate.h"
#include "QMstream.h"
#include "QMstreamIm.h"
using namespace std;

void printInput(vector<int>* minTerm, vector<int>* dontCare, SFout& out)
{
    out << "Input:" << endl
         << "Min Term:" << endl
         << "  ";
    sort(minTerm->begin(), minTerm->end());
    for(auto& mt : *minTerm)
        out << mt << " ";
    out << endl;
    if(!dontCare->empty())
    {
        sort(dontCare->begin(), dontCare->end());
        out << "Don't Care:" << endl
             << "  ";
        for(auto& dc : *dontCare)
            out << dc << " ";
        out << endl;
    }
    out << endl;
}
void printInitial(QMTable* qm, SFout& out)
{
    out << "Round 1" << endl
             << "==============================" << endl;
        for(auto& row : *qm)
        {
            if(!row.empty())
            {
                for(auto& col : row)
                {
                    out << col << endl;
                }
                out << "------------------------------" << endl;
            }
        }
    out << endl;
}
QMTable simplify(QMTable qm, int var_num, SFout& out)
{
    int phase = 1;
    while(phase <= var_num*2) //忘了考慮最極限的情形，即所有位數都被化簡，故*2
    {
        int wave = 1;
        queue<QMNode> mergeQueue;
        for(auto& select : qm)
        {
            for(auto& current : select)
            {
                if(current.getPhase() == phase) //檢查是否為目前階段該合併的term，在此階段定義為合併過n次，則其階段數值為2^n
                {
                    int wavesize = qm[wave].size();
                    for(int i = 0; i < wavesize; i++)
                    {
                        QMNode* compare = &qm[wave][i];
                        if(compare->getPhase() == phase) //是否是同一個階段的term，在此階段定義為合併過n次，則其階段數值為2^n
                        {
                            if(current.canMerge(*compare))
                            {
                                mergeQueue.push(QMNode::mergeNode(&current, compare)); //加進之後要被合併的佇列中
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
            out << "Round " << (phase>>1)+1+1 << endl //第一個加一補的是phase/2，第二個補的是外面被算過的Round 1
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
                    if(iter.compare(*getIn)) //檢查是否重複
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
                        out << "------------------------------" << endl;
                        count = nowCount;
                    }
                    out << *getIn << endl;
                }
                mergeQueue.pop();
            }
            out << "------------------------------" << endl
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
QMTableP piChart(QMTable* qm, vector<int> minTerm, int var_num, SFout& out)
{
    QMTableP minTermCount(minTerm.size(), vector<QMNode*>());
    out << "Result:" << endl;
    int lspace = var_num * 2;
    out << setw(lspace) << "" << "|";
    //印出minTerm數字
    int columnWidth = 3;
    for(auto& need : minTerm)
    {
        out << setw(columnWidth) << need;
    }
    out << endl;
    //印出分隔線
    out << string(lspace, '-') << "|" << string(minTerm.size()*columnWidth, '-') << endl;
    //印出每個term以及其蘊含的數字
    for(auto& row : *qm)
    {
        for(auto& implicant : row)
        {
            out << setw(lspace) << implicant.varStr() << "|";
            string varSelect(minTerm.size()*columnWidth, ' ');
            for(auto& num : implicant.getNumber())
            {
                int pos = -1;
                int varSize = (int)minTerm.size();
                for(int i = 0; i < varSize; i++)
                {
                    if(minTerm[i] == num) //搜尋該數字在表中的哪裡
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
            out << varSelect << endl;
        }
    }
    out << endl;
    return minTermCount;
}
QMTable petrickMethod(QMTable qm, QMTableP minTermCount, int var_num)
{
    vector< vector<string> > toFinal;
    /*將QMNode物件矩陣轉為其value字串的矩陣以方便化簡
    在此，最外面的vector代表一整個式子，裡面的vector代表每一個小括號，or會分開存放成為vector的每一項，而and則是直接以字串合併的方式表現*/
    for(auto& stat : minTermCount)
    {
        vector<string> temp;
        for(auto& term : stat)
        {
            temp.push_back(term->getValue());
        }
        toFinal.push_back(temp);
    }
    /*使用deque，這是由於此處需要FIFO的結構做化簡，
    而C++的STL容器中，vector的結構FILO較快速，與stack較為接近，queue不可迭代與隨機存取，而deque可以迭代並且可以作FIFO或FILO*/
    deque<string> finalAns; //此deque為最後解答的容器，在運算結束後即會得到化簡解
    while(!toFinal.empty()) //若欲化簡之方程式還有括號未乘
    {
        vector<string> current = toFinal.back(); //取出最後面的括號做運算
        if(finalAns.empty()) //若是finalAns仍為空，直接將此括號丟入其中
        {
            for(auto& item : current)
            {
                finalAns.push_back(item);
            }
        }
        else
        {
            int size = 0, should = finalAns.size()*current.size(); //計算乘完之後的項數
            while(size < should) //兩括號相乘，乘到符合應該有的次數為止，如2項*3項，應當作6次乘法
            {
                string adding = finalAns.front(); //取出finalAns中的第一項做乘法
                for(auto& added : current) //取出待乘括號的每一項
                {
                    //檢查是否為其中含有XX的情形，若是則只輸出X，否則輸出XY至finalAns最後面，其中XY中間用空白分隔
                    if(adding.find(added) == string::npos)
                        finalAns.push_back(adding + " " + added);
                    else
                        finalAns.push_back(adding);
                    size++;
                }
                finalAns.pop_front(); //將第一項移除，代表已經乘完
            }
            for(auto it = finalAns.begin(); it != finalAns.end(); it++)
            {
                if(!(*it).compare("NONEED")) //檢查是否被標記為NONEED的項目，若是則跳過
                    continue;
                vector<string> subterm; //將每一項表示的乘積分開，以尋找是否可以化簡，如將原字串"X Y Z"拆成"X","Y","Z"
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
                    //檢查是否迭代到同一項，做不同處理
                    if(check == it)
                    {
                        //若是同一項，在檢查一次是否有XX=X的型式可以化簡
                        string temp;
                        for(auto& dupli : subterm)
                        {
                            if(temp.find(dupli) == string::npos)
                            {
                                temp = temp + dupli + " "; //先將每一項後面都加空白，最後在去除多餘的
                            }
                        }
                        temp.pop_back(); //去除最後多餘的空白
                        *check = temp;
                    }
                    else
                    {
                        //若是不同項，檢查是否有X+XY=X的型式可以化簡，若所有被分開的項，此乘積皆包含，代表有X+XX或是X+XY可以化簡
                        int contain = 0;
                        for(auto& dupli : subterm)
                        {
                            if((*check).find(dupli) != string::npos)
                            {
                                contain++;
                            }
                        }
                        //若是確認為X+XX或X+XY，將此XY或XX項標記為NONEED，之後再刪除
                        if(contain >= subterm.size())
                        {
                            *check = "NONEED";
                        }
                    }
                }
            }
            //刪除所有被標記為NONEED的項
            for(auto it = finalAns.begin(); it != finalAns.end();)
            {
                if(!(*it).compare("NONEED"))
                    it = finalAns.erase(it);
                else
                    it++;
            }
        }
        toFinal.pop_back(); //移除toFinal最後面的括號，也就是剛剛在做運算的括號，代表運算結束
    }
    //化簡完畢後，尋找相乘項數最少的項數數字為多少
    int minSize = -1;
    for(auto& term : finalAns)
    {
        if(minSize == -1)
            minSize = term.size();
        else if(term.size() < minSize)
            minSize = term.size();
    }
    //建立一map方便將字串換回QMNode物件
    map<string, QMNode> nodeName;
    for(auto& row : qm)
    {
        for(auto& term : row)
        {
            nodeName[term.getValue()] = term;
        }
    }
    //選出最少項數的項並放入select中，有可能有多種選擇，因此select是QMNode矩陣
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
void printFinal(QMTable select, int var_num, SFout& out)
{
    for(auto& ans : select)
    {
        //印出F(...)
        out << "F(";
        char var = 'A';
        for(int i=0; i<var_num;)
        {
            out << var;
            i++;
            var++;
            if(i < var_num)
                out << ",";
            else
                out << ") = ";
        }
        //印出最終合併的term，由於化簡時是從最後一個括號開始運算，此處反相印回來，其順序就會跟Prime Implicant表上至下順序相同
        for(auto it = ans.rbegin(); it != ans.rend();)
        {
            string var = it->varStr();
            if(var == "")
            {
                out << " 1 " << endl;
                break;
            }
            out << it->varStr();
            it++;
            if(it != ans.rend())
                out << "+";
            else
                out << endl;
        }
    }
}
