#include <sstream>
#include "QMNode.h"
QMNode::QMNode(int num, int size)
{
    number.push_back(num);
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
