#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "QMio.h"

using namespace std;

int readMinTerm(QMTable* qm, vector<int>* minTerm, string fileName)
{
    ifstream input;
    input.open(fileName);
    if(!input)
    {
        cerr << "無法讀取檔案！" << endl
             << "請確認檔案是否存在！" << endl;
        exit(1);
    }
    int var_num = 0;
    input >> var_num;
    if(var_num < 2 || var_num > 6)
    {
        cerr << "目前只能處理2~6個變數喔！" << endl;
        exit(2);
    }
    int minterm_num = 1;
    minterm_num <<= var_num;
    qm->assign(var_num+1, vector<QMNode>());
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
                exit(3);
            }
            minTerm->push_back(i);
            QMNode node(i, var_num);
            (*qm)[node.count()].push_back(node);
        }
    }
    input.close();
    return var_num;
}
vector<int> readDontCare(QMTable* qm, const vector<int>* minTerm, int var_num, string fileName)
{
    ifstream input;
    input.open(fileName);
    vector<int> dontCare;
    int minterm_num = 1;
    minterm_num <<= var_num;
    if(input)
    {
        int var_num2 = 0;
        input >> var_num2;
        if(var_num2 != var_num)
        {
            cerr << "請將minTerm與Don't Care變數數量調整成一樣之後，" << endl
                 << "再使用本程式，感謝配合！" << endl;
            exit(4);
        }
        else
        {
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
                        exit(3);
                    }
                    else if(find(minTerm->begin(), minTerm->end(), i) != minTerm->end())
                    {
                        cerr << "錯誤的輸入！minTerm重複出現在Don't Care！" << endl;
                        exit(3);
                    }
                    dontCare.push_back(i);
                    QMNode node(i, var_num);
                    (*qm)[node.count()].push_back(node);
                }
            }
        }
    }
    input.close();
    return dontCare;
}
