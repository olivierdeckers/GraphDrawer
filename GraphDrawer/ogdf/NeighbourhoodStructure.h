#ifndef NEIGHBOURHOODSTRUCTURE_H
#define NEIGHBOURHOODSTRUCTURE_H

#include <ogdf/basic/NodeArray.h>
#include <ogdf/basic/geometry.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph.h>

namespace ogdf {

struct LayoutChange {
    node n;
    DPoint newPos;
};

class NeighbourhoodStructure
{
public:
    NeighbourhoodStructure(GraphAttributes &GA) : m_GA(GA), m_G(GA.constGraph()) {}

    virtual ~NeighbourhoodStructure() {}
    virtual List<LayoutChange> generateNeighbouringLayout(double temp) = 0;

protected:
    GraphAttributes &m_GA;
    const Graph &m_G;
};

}

#endif // NEIGHBOURHOODSTRUCTURE_H
