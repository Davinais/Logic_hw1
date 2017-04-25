#include <sstream>
#include <iomanip>
#include "QMNode.h"
QMNode::QMNode(int num, int size)
{
    number.push_back(num);
    phase = 1;
    //轉換為二進制
    for(;num > 0;num>>=1)
    {
        std::ostringstream temp;
        temp << num%2; //等同於(num&1)
        value = temp.str() + value;
    }
    while(value.size() < size)
    {
        value = "0" + value;
    }
    merged = false;
}
int QMNode::count()
{
    int count = 0;
    for(char& c : value)
    {
        if(c=='1')
            count++;
    }
    return count;
}
bool QMNode::canMerge(QMNode a)
{
    //檢查可否合併，即其value字串是否只有一個字元不相等
    int notEqual = 0;
    for(int i=0; i<value.size(); i++)
    {
        if(value[i] != a.value[i])
            notEqual++;
    }
    return (notEqual==1);
}
bool QMNode::compare(QMNode a)
{
    return (value.compare(a.value) == 0);
}
int QMNode::getMergePos(QMNode a)
{
    for(int i=0; i<value.size(); i++)
    {
        if(value[i] != a.value[i])
            return i;
    }
    return -1;
}
//合併用函數，將兩Node唯一不同的地方改為'-'，兩者蘊含數字合併後，存入新Node
QMNode QMNode::mergeNode(QMNode* a, QMNode* b)
{
    QMNode node;
    node.number.reserve(a->number.size() + b->number.size());
    node.number.insert(node.number.end(), a->number.begin(), a->number.end());
    node.number.insert(node.number.end(), b->number.begin(), b->number.end());
    a->merged = true;
    b->merged = true;
    node.phase = node.number.size();
    std::string temp = a->value;
    temp[a->getMergePos(*b)] = '-';
    node.value = temp;
    node.merged = false;
    return node;
}

std::string QMNode::varStr()
{
    std::string varString;
    //回傳以abc等變數為基礎的字串，若為1則直接印出該變數，0則在該變數後面再補上'，-則不印出
    char varName = 'a';
    for(auto& var : value)
    {
        if(var == '1')
        {
            varString += varName;
            varString += " ";
        }
        else if(var == '0')
        {
            varString += varName;
            varString += "'";
        }
        else{;}
        varName++;
    }
    return varString;
}

std::ostream& operator <<(std::ostream& out, const QMNode& node)
{
    out << "(";
    for(auto it = node.number.begin(); it != node.number.end();)
    {
        out << std::setw(2) << *it;
        if(++it != node.number.end())
            out << ",";
    }
    out << ")" << node.value;
    return out;
}
