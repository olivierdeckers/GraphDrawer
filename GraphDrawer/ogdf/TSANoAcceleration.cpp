#include "TSANoAcceleration.h"

namespace ogdf{

TSANoAcceleration::TSANoAcceleration(const GraphAttributes &GA):
    m_G(GA.constGraph())
{
}

void TSANoAcceleration::updateNodePosition(const node, const DPoint &)
{
}

void TSANoAcceleration::possibleCrossingEdges(const DPoint &, const DPoint &, List<edge> &result) const
{
    m_G.allEdges(result);
}

void TSANoAcceleration::nearNodes(const DPoint &, const double , List<node> &) const
{
    //TODO
}

}
