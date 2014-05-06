#include "TSAEdgeLength.h"

namespace ogdf {


TSAEdgeLength::TSAEdgeLength(GraphAttributes &GA) : TSANodePairEnergy("EdgeLength", GA)
{
    m_edgeLengths = EdgeArray<double>(GA.constGraph());

    edge e;
    m_sumOfEdgeLengths = 0;
    m_nbEdges = 0;
    forall_edges(e, GA.constGraph()) {
        node a = e->source();
        node b = e->target();

        IntersectionRectangle i1(shape(a)), i2(shape(b));
        i1.move(newPos(a));
        i2.move(newPos(b));
        double length = i1.distance(i2);
        m_edgeLengths[e] = length;
        m_sumOfEdgeLengths += length;
        m_nbEdges++;
    }
}

double TSAEdgeLength::computeCoordEnergy(node a, node b)
{
    double energy = 0.0;
    if(adjacent(a,b)) {
        IntersectionRectangle i1(shape(a)), i2(shape(b));
        i1.move(newPos(a));
        i2.move(newPos(b));
        double length = i1.distance(i2);

        EdgeLengthChange elc;
        elc.a = a; elc.b = b;
        elc.length = length;
        m_edgeLengthChanges.pushBack(elc);

        energy = length - m_sumOfEdgeLengths / m_nbEdges;
        energy *= energy;
    }

    return energy;
}

void TSAEdgeLength::internalCandidateTaken()
{
    TSANodePairEnergy::internalCandidateTaken();

    List<edge> edges;
    m_G.allEdges(edges);

    ListIterator<EdgeLengthChange> it = m_edgeLengthChanges.begin();
    for(;it.valid(); it++)
    {
        EdgeLengthChange elc = *it;

        edge e = findEdge(elc.a, elc.b, edges);
        m_sumOfEdgeLengths -= m_edgeLengths[e];
        m_sumOfEdgeLengths += elc.length;
        m_edgeLengths[e] = elc.length;
    }

    m_edgeLengthChanges.clear();
}

edge TSAEdgeLength::findEdge(node a, node b, List<edge> &edges)
{
    ListIterator<edge> it = edges.begin();
    for(; it.valid(); it++) {
        edge e = *it;
        if ((e->source() == a && e->target() == b) || (e->source() == b && e->target() == a))
            return e;
    }

    throw "No edge found between nodes.";
}

}
