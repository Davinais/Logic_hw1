#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "QMNode.h"
#include "QMOperate.h"
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
    minterm_num <<= var_num;
    vector<int> minTerm;
    QMTable qm(var_num+1, vector<QMNode>());
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
    printInitial(&qm);
    qm = simplify(qm, var_num);
    QMTableP minTermCount = piChart(&qm, minTerm, var_num);
    vector<QMNode> select;
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
        for(auto& stat : minTermCount)
        {
            if(stat.size() == 1)
            {
                select.push_back(*stat[0]);
            }
        }
        for(auto& chosen : select)
        {
            for(auto it = minTermCount.begin(); it != minTermCount.end();)
            {
                bool exist = false;
                for(auto& term : *it)
                {
                    if(*term == chosen)
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                {
                    it = minTermCount.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
        map<string, QMNode> nodeName;
        map<string, int> nodeCount;
        for(auto& row : qm)
        {
            for(auto& term : row)
            {
                nodeName[term.getValue()] = term;
                nodeCount[term.getValue()] = 0;
            }
        }
        while(!minTermCount.empty())
        {
            for(auto& stat : minTermCount)
            {
                for(auto& term : stat)
                {
                    nodeCount[term->getValue()]++;
                }
            }
            string current;
            for(auto& term : nodeCount)
            {
                if(current == "")
                {
                    current = term.first;
                }
                else if(term.second > nodeCount[current])
                {
                    current = term.first;
                }
            }
            select.push_back(nodeName[current]);
            for(auto it = minTermCount.begin(); it != minTermCount.end();)
            {
                bool exist = false;
                for(auto& term : *it)
                {
                    if(*term == nodeName[current])
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                {
                    it = minTermCount.erase(it);
                }
                else
                {
                    it++;
                }
            }
            for(auto& term : nodeCount)
            {
                term.second = 0;
            }
        }
    }
    printFinal(select, var_num);
    return 0;
}
