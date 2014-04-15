#include "MoveEdge.h"
#include <ogdf/basic/basic.h>
#include <ogdf/basic/Math.h>

#define randNum() ((double) rand()) / RAND_MAX

namespace ogdf {

MoveEdge::MoveEdge(GraphAttributes &GA) : NeighbourhoodStructure(GA)
{
    m_G.allEdges(m_edges);
}

MoveEdge::~MoveEdge() {}

void MoveEdge::generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result)
{
    int r = randomNumber(0, m_edges.size() - 1);
    edge e = *(m_edges.get(r));

    double randomAngle = randNum() * 2.0 * Math::pi;
    DPoint diff;
    diff.m_y = sin(randomAngle) * diskRadius(temp) * randNum();
    diff.m_x = cos(randomAngle) * diskRadius(temp) * randNum();

    node s = e->source();
    node t = e->target();

    DPoint newSPos;
    newSPos.m_x = m_GA.x(s) + diff.m_x;
    newSPos.m_y = m_GA.y(s) + diff.m_y;
    DPoint newTPos;
    newTPos.m_x = m_GA.x(t) + diff.m_x;
    newTPos.m_y = m_GA.y(t) + diff.m_y;

    result.insert(s, newSPos);
    result.insert(t, newTPos);
}

}
