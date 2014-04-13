#ifndef MOVEEDGE_H
#define MOVEEDGE_H

#include <ogdf/basic/GraphAttributes.h>
#include "NeighbourhoodStructure.h"

namespace ogdf {

class MoveEdge : public NeighbourhoodStructure
{
public:
    MoveEdge(GraphAttributes &GA);
    ~MoveEdge();

    virtual void generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result);

private:
    List<edge> m_edges;
};

}
#endif // MOVEEDGE_H
