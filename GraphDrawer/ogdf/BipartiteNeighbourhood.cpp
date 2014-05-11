#include "BipartiteNeighbourhood.h"
#include <ogdf/basic/Math.h>

namespace ogdf {

BipartiteNeighbourhood::BipartiteNeighbourhood(GraphAttributes &GA) : NeighbourhoodStructure(GA, "Bipartite")
{
}

// swap two nodes form the same set
void BipartiteNeighbourhood::generateNeighbouringLayout(double, Hashing<node, DPoint> &result)
{
    List<node> &set = (randomNumber(0, 1) == 0) ? left : right;

    int a = randomNumber(0, set.size()-1);
    int b = randomNumber(0, set.size()-1);

    node n1 = *(set.get(a));
    node n2 = *(set.get(b));

    DPoint p1(m_GA.x(n1), m_GA.y(n1));
    DPoint p2(m_GA.x(n2), m_GA.y(n2));

    result.insert(n1, p2);
    result.insert(n2, p1);
}

void BipartiteNeighbourhood::initialLayout()
{
    List<node> unassignedNodes;
    m_G.allNodes(unassignedNodes);
    int nbNodes = unassignedNodes.size();

    while(!unassignedNodes.empty()) {
        node n = unassignedNodes.front();

        addNodes(unassignedNodes, true, n);
    }

    OGDF_ASSERT(left.size() + right.size() == nbNodes);

    ListIterator<node> it;
    double dy = 1.0 / left.size();
    double y = 0.0;
    for(it = left.begin(); it.valid(); it++) {
        m_GA.x(*it) = 0;
        m_GA.y(*it) = y;
        y += dy;
    }

    dy = 1.0 / right.size();
    y = 0.0;
    for(it = right.begin(); it.valid(); it++) {
        m_GA.x(*it) = 1;
        m_GA.y(*it) = y;
        y += dy;
    }
}

void BipartiteNeighbourhood::addNodes(List<node> &unassignedNodes, bool useLeft, node n) {
    List<node> &set = useLeft ? left : right;

    set.pushBack(n);

    int index = unassignedNodes.search(n);
    OGDF_ASSERT(index >= 0);
    unassignedNodes.del(unassignedNodes.get(index));

    adjEntry adj;
    forall_adj(adj, n) {
        node neighbour = adj->twinNode();
        if(unassignedNodes.search(neighbour) >= 0) {
            addNodes(unassignedNodes, !useLeft, neighbour);
        }
    }

}

}
