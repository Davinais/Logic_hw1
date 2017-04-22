#ifndef _QMNODE_H_
#define _QMNODE_H_

#include <iostream>
#include <string>
#include <vector>
class QMNode
{
    public:
        QMNode(int, int);

        int count();
        int getPhase() {return phase;};
        bool isMerged() {return merged;};

        bool canMerge(QMNode);
        bool compare(QMNode);
        static QMNode mergeNode(QMNode*, QMNode*);

        friend std::ostream& operator <<(std::ostream&, const QMNode&);
    
    private:
        QMNode() {};
        int getMergePos(QMNode);
        int phase;
        std::vector<int> number;
        std::string value;
        bool merged;
};
#endif
