#include "RandomMove.h"

#include <ogdf/basic/basic.h>
#include <ogdf/basic/Math.h>

#define randNum() ((double) rand()) / RAND_MAX

namespace ogdf {

RandomMove::RandomMove(GraphAttributes &GA) : NeighbourhoodStructure(GA)
{
    m_G.allNodes(m_nonIsolatedNodes);
    ListIterator<node> it,itSucc;
    for(it = m_nonIsolatedNodes.begin(); it.valid(); it = itSucc) {
        itSucc = it.succ();
        if((*it)->degree() == 0) m_nonIsolatedNodes.del(it);
    }
}

RandomMove::~RandomMove()
{
}

List<LayoutChange> RandomMove::generateNeighbouringLayout(double temp)
{
    double diskRadius = min(500.0, 1e4 * (temp - 1e-6));

    int randomPos = randomNumber(0, m_nonIsolatedNodes.size()-1);
    node v = *(m_nonIsolatedNodes.get(randomPos));
    double oldx = m_GA.x(v);
    double oldy = m_GA.y(v);
    double randomAngle = randNum() * 2.0 * Math::pi;
    DPoint newPos;
    newPos.m_y = oldy+sin(randomAngle)*diskRadius * randNum();
    newPos.m_x = oldx+cos(randomAngle)*diskRadius * randNum();

    LayoutChange lc;
    lc.n = v;
    lc.newPos = newPos;

    List<LayoutChange> result;
    result.pushBack(lc);

    return result;
}

}
