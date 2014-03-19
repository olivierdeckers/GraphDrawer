#ifndef TSANOACCELERATION_H
#define TSANOACCELERATION_H

#include <ogdf/AccelerationStructure.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph.h>

namespace ogdf {

class TSANoAcceleration : public AccelerationStructure
{
public:
    TSANoAcceleration(const GraphAttributes &);

    virtual void updateNodePosition(const node v, const DPoint& newPos);
    virtual void possibleCrossingEdges(const DPoint& sPos, const DPoint& tPos, List<edge>& result) const;
    virtual void nearNodes(const DPoint &pos, const double maxDist, List<node>& nodes) const;

private:
    const Graph &m_G;
};

}
#endif // TSANOACCELERATION_H
