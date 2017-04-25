#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
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
    if(var_num < 2 || var_num > 6)
    {
        cerr << "目前只能處理2~6個變數喔！" << endl;
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
    input.open("input2.txt");
    vector<int> dontCare;
    if(input)
    {
        int var_num2 = 0;
        input >> var_num2;
        if(var_num2 != var_num)
        {
            cerr << "請將minTerm與Don't Care變數數量調整成一樣之後，" << endl
                 << "再使用本程式，感謝配合！" << endl;
            return 4;
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
                        return 3;
                    }
                    else if(find(minTerm.begin(), minTerm.end(), i) != minTerm.end())
                    {
                        cerr << "錯誤的輸入！minTerm重複出現在Don't Care！" << endl;
                        return 3;
                    }
                    dontCare.push_back(i);
                    QMNode node(i, var_num);
                    qm[node.count()].push_back(node);
                }
            }
        }
    }
    cout << "輸入：" << endl
         << "Min Term:" << endl
         << "  ";
    sort(minTerm.begin(), minTerm.end());
    for(auto& mt : minTerm)
        cout << mt << " ";
    cout << endl;
    if(!dontCare.empty())
    {
        sort(dontCare.begin(), dontCare.end());
        cout << "Don't Care:" << endl
             << "  ";
        for(auto& dc : dontCare)
            cout << dc << " ";
        cout << endl;
    }
    cout << endl;
    printInitial(&qm);
    qm = simplify(qm, var_num);
    QMTableP minTermCount = piChart(&qm, minTerm, var_num);
    QMTable select = petrickMethod(qm, minTermCount, var_num);
    printFinal(select, var_num);
    return 0;
}
