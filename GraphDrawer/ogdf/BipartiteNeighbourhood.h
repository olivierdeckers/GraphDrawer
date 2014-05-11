#ifndef BIPARTITENEIGHBOURHOOD_H
#define BIPARTITENEIGHBOURHOOD_H

#include <ogdf/NeighbourhoodStructure.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Hashing.h>

namespace ogdf {

class BipartiteNeighbourhood : public NeighbourhoodStructure
{
public:
    BipartiteNeighbourhood(GraphAttributes &GA);

    void generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result);

    void initialLayout();


private:
    List<node> left;
    List<node> right;

    void addNodes(List<node> &unassignedNodes, bool left, node n);
};

}

#endif // BIPARTITENEIGHBOURHOOD_H
