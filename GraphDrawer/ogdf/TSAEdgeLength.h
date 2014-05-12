#ifndef TSAEDGELENGTH_H
#define TSAEDGELENGTH_H

#include <ogdf/TSANodePairEnergy.h>

namespace ogdf {

class TSAEdgeLength : public TSANodePairEnergy
{
public:
    TSAEdgeLength(GraphAttributes &AG, double preferredEdgeLength);

private:
    double computeCoordEnergy(node, node);

    double m_prefEdgeLength;
};

}
#endif // TSAEDGELENGTH_H
