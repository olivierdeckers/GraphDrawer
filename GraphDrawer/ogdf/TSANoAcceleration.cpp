#include "TSANoAcceleration.h"

namespace ogdf{

TSANoAcceleration::TSANoAcceleration(const GraphAttributes &GA):
    m_G(GA.constGraph())
{
    m_G.allEdges(m_nonSelfLoops);

    ListIterator<edge> it, itSucc;
    for(it = m_nonSelfLoops.begin(); it.valid(); it = itSucc) {
        itSucc = it.succ();
        if((*it)->isSelfLoop()) m_nonSelfLoops.del(it);
    }
}

void TSANoAcceleration::updateNodePosition(const node, const DPoint &)
{
}

void TSANoAcceleration::possibleCrossingEdges(const DPoint &, const DPoint &, List<edge> &result) const
{
    ListConstIterator<edge> it = m_nonSelfLoops.begin();
    for(; it.valid(); it++)
    {
        result.pushBack(*it);
    }
}

void TSANoAcceleration::nearNodes(const DPoint &, const double , List<node> &) const
{
    //TODO
}

}
