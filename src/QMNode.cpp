#include <sstream>
#include <iomanip>
#include "QMNode.h"
QMNode::QMNode(int num, int size)
{
    number.push_back(num);
    phase = 1;
    for(;num > 1;num/=2)
    {
        std::ostringstream temp;
        temp << num%2;
        value = temp.str() + value;
    }
    value = "1" + value;
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