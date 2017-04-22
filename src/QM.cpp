#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
            QMNode* node = new QMNode(i, var_num);
            qm[node->count()].push_back(*node);
        }
    }
    input.close();
    return 0;
}
