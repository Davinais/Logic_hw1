#ifndef _QMNODE_H_
#define _QMNODE_H_

#include <string>
#include <vector>
class QMNode
{
    public:
        QMNode() {};
        QMNode(int, int);

        int count();

        std::vector<int> number;
        std::string value;
        bool merged;
};
#endif
