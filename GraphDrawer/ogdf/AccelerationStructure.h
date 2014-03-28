#ifndef ACCELERATIONSTRUCTURE_H
#define ACCELERATIONSTRUCTURE_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/geometry.h>

namespace ogdf {

class AccelerationStructure
{
public:
    virtual ~AccelerationStructure() {}

    //TODO: change to udateNodePosition(Hashing<node, DPoint> &layoutChanges
    virtual void updateNodePosition(const node n, const DPoint& newPos) = 0;

    virtual void possibleCrossingEdges(const DPoint& sPos, const DPoint& tPos, List<edge>& result) const = 0;
    virtual void nearNodes(const DPoint &pos, const double maxDist, List<node>& nodes) const = 0;
};

}
#endif // ACCELERATIONSTRUCTURE_H
