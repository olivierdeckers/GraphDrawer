#ifndef RANDOMMOVE_H
#define RANDOMMOVE_H

#include "NeighbourhoodStructure.h"

namespace ogdf {

class RandomMove : public NeighbourhoodStructure
{
public:
    RandomMove(GraphAttributes&);
    ~RandomMove();

    virtual void generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result);

private:
    List<node> m_nonIsolatedNodes;
};

}
#endif // RANDOMMOVE_H
