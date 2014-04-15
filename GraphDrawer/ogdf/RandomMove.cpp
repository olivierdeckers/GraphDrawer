#include "RandomMove.h"

#include <ogdf/basic/basic.h>
#include <ogdf/basic/Math.h>

#define randNum() ((double) rand()) / RAND_MAX

namespace ogdf {

RandomMove::RandomMove(GraphAttributes &GA) : NeighbourhoodStructure(GA, "RandomMove")
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

void RandomMove::generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result)
{
    int randomPos = randomNumber(0, m_nonIsolatedNodes.size()-1);
    node v = *(m_nonIsolatedNodes.get(randomPos));
    double oldx = m_GA.x(v);
    double oldy = m_GA.y(v);
    double randomAngle = randNum() * 2.0 * Math::pi;
    DPoint newPos;
    newPos.m_y = oldy+sin(randomAngle) * diskRadius(temp) * randNum();
    newPos.m_x = oldx+cos(randomAngle) * diskRadius(temp) * randNum();

    result.insert(v, newPos);
}

}
