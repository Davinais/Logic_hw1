#ifndef _QMNODE_H_
#define _QMNODE_H_

#include <iostream>
#include <string>
#include <vector>
class QMNode
{
    public:
        QMNode(int, int);
        QMNode() {merged = false;};

        int count();
        int getPhase() {return phase;};
        bool isMerged() {return merged;};
        const std::vector<int>& getNumber() const {return number;};

        bool canMerge(QMNode);
        bool compare(QMNode);
        static QMNode mergeNode(QMNode*, QMNode*);

        std::string varStr();
        std::string getValue() {return value;};

        friend std::ostream& operator <<(std::ostream&, const QMNode&);
        bool operator ==(const QMNode& a) const {return !(this->value.compare(a.value));};
        bool operator <(const QMNode& a) const {return (this->cmpValue < a.cmpValue);};
    
    private:
        int getMergePos(QMNode);
        int phase;
        void calcCmpValue();
        int cmpValue;
        std::vector<int> number;
        std::string value;
        bool merged;
};
#endif
