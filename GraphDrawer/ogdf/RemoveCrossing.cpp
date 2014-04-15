#include "RemoveCrossing.h"
#include <ogdf/basic/geometry.h>

namespace ogdf {

RemoveCrossing::RemoveCrossing(GraphAttributes &GA, TSAPlanarity &planarity) :
    NeighbourhoodStructure(GA, "Crossing"),
    m_planarity(planarity)
{
}

RemoveCrossing::~RemoveCrossing()
{

}

void RemoveCrossing::generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result) {
    TSAPlanarity::Crossing c = m_planarity.getRandomCrossing();

    DLine e1 = DLine(m_GA.x(c.edge1->source()), m_GA.y(c.edge1->source()), m_GA.x(c.edge1->target()), m_GA.y(c.edge1->target()));
    DLine e2 = DLine(m_GA.x(c.edge2->source()), m_GA.y(c.edge2->source()), m_GA.x(c.edge2->target()), m_GA.y(c.edge2->target()));
    DPoint crossing;
    e1.intersection(e2, crossing);

    int nbNode = rand() % 4;
    node n;
    switch(nbNode) {
    case 0:
        n = c.edge1->source();
        break;
    case 1:
        n = c.edge1->target();
        break;
    case 2:
        n = c.edge2->source();
        break;
    case 3:
        n = c.edge2->target();
        break;
    }

    DPoint pos = DPoint(m_GA.x(n), m_GA.y(n));
    DPoint diff = crossing - pos;
    double alpha = randomDouble(1.001, 2);
    diff = DPoint(diff.m_x * alpha, diff.m_y * alpha);
    DPoint newPos = pos + diff;

    result.insert(n, newPos);
}

}
