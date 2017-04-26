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
        cerr << "�L�kŪ���ɮסI" << endl
             << "�нT�{�ɮ׬O�_�s�b�I" << endl;
        exit(1);
    }
    int var_num = 0;
    input >> var_num;
    if(var_num < 2 || var_num > 6)
    {
        cerr << "�ثe�u��B�z2~6���ܼƳ�I" << endl;
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
                cerr << "���~����J�I�W�X�ܼƽd��I" << endl;
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
            cerr << "�бNminTerm�PDon't Care�ܼƼƶq�վ㦨�@�ˤ���A" << endl
                 << "�A�ϥΥ��{���A�P�°t�X�I" << endl;
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
                        cerr << "���~����J�I�W�X�ܼƽd��I" << endl;
                        exit(3);
                    }
                    else if(find(minTerm->begin(), minTerm->end(), i) != minTerm->end())
                    {
                        cerr << "���~����J�IminTerm���ƥX�{�bDon't Care�I" << endl;
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
