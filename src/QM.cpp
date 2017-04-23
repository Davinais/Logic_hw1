#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
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
    if(var_num < 2 || var_num > 4)
    {
        cout << "目前只能處理2~4個變數喔！" << endl;
        return 2;
    }
    int minterm_num = 1;
    minterm_num <<= var_num;
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
    printInitial(&qm);
    qm = simplify(qm, var_num);
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
        for(auto& row : qm)
        {
            for(auto& implicant : row)
            {
                cout << setw(lspace) << implicant.varStr() << "|";
                string varSelect(minTerm.size()*3, ' ');
                for(auto& num : implicant.getNumber())
                {
                    int pos = -1;
                    int varSize = (int)minTerm.size();
                    for(int i = 0; i < varSize; i++)
                    {
                        if(minTerm[i] == num)
                        {
                            pos = i+1;
                            break;
                        }
                    }
                    if(pos > 0)
                    {
                        varSelect[pos*3-1] = 'x';
                    }
                }
                cout << varSelect << endl;
            }
        }
    }
    {

    }
    return 0;
}
