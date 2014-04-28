#ifndef NODEEDGEDISTANCE_H
#define NODEEDGEDISTANCE_H

#include <ogdf/TSAPlanarity.h>
#include <ogdf/AccelerationStructure.h>

namespace ogdf {

class NodeEdgeDistance : public TSAPlanarity
{
public:
    NodeEdgeDistance(GraphAttributes &GA, AccelerationStructure *accStruct);

protected:
    virtual bool lowLevelIntersect(const DPoint &, const DPoint &, const DPoint &, const DPoint &, double &) const;

};

}

#endif // NODEEDGEDISTANCE_H
