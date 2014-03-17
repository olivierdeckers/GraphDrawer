#ifndef NEIGHBOURHOODSTRUCTURE_H
#define NEIGHBOURHOODSTRUCTURE_H

#include <ogdf/basic/NodeArray.h>
#include <ogdf/basic/geometry.h>

namespace ogdf {

struct LayoutChange {
    node n;
    DPoint newPos;
};

class NeighbourhoodStructure
{
public:
    virtual ~NeighbourhoodStructure() {}
    virtual List<LayoutChange> generateNeighbouringLayout() = 0;
};

}

#endif // NEIGHBOURHOODSTRUCTURE_H
