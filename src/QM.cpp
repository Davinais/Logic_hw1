#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <vector>
#include "QMNode.h"
using namespace std;

int main()
{
    ifstream input;
    input.open("input.txt");
    if(!input)
    {
        cerr << "無法讀取檔案！" << endl
             << "請確認檔案是否存在！" << endl;
        return 1;
    }
    int var_num = 0;
    input >> var_num;
    if(var_num < 2 || var_num > 4)
    {
        cout << "目前只能處理2~4個變數喔！" << endl;
        return 2;
    }
    int minterm_num = 1;
    minterm_num = minterm_num << var_num;
    vector<int> minTerm;
    vector< vector<QMNode> > qm(var_num+1, vector<QMNode>());
    string line;
    while(getline(input, line))
    {
        stringstream s(line);
        string var;
        while(getline(s, var, ','))
        {
            stringstream vars(var);
            int i;
            vars >> i;
            if(i > minterm_num - 1)
            {
                cerr << "錯誤的輸入！超出變數範圍！" << endl;
                return 3;
            }
            minTerm.push_back(i);
            QMNode node(i, var_num);
            qm[node.count()].push_back(node);
        }
    }
    input.close();
    {
        cout << "Round 1" << endl
             << "==============================" << endl;
        for(auto& row : qm)
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
    int phase = 1;
    while(phase <= var_num)
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
            cout << "Round " << (phase/2)+1+1 << endl //第一個加一補的是phase/2，第二個補的是外面被算過的Round 1
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
        phase *= 2;
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
    {
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

    }
    return 0;
}
