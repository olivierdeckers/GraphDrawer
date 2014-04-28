#ifndef TSAEDGELENGTH_H
#define TSAEDGELENGTH_H

#include <ogdf/TSANodePairEnergy.h>

namespace ogdf {

class TSAEdgeLength : public TSANodePairEnergy
{
public:
    TSAEdgeLength(GraphAttributes &AG);

private:
    double computeCoordEnergy(node, node);

    virtual void internalCandidateTaken();

    edge findEdge(node a, node b, List<edge> &edges);

    struct EdgeLengthChange {
        node a, b;
        double length;
    };

    EdgeArray<double> m_edgeLengths;
    double m_sumOfEdgeLengths;
    int m_nbEdges;
    List<EdgeLengthChange> m_edgeLengthChanges;
};

}
#endif // TSAEDGELENGTH_H
