#ifndef RANDOMMOVE_H
#define RANDOMMOVE_H

#include "NeighbourhoodStructure.h"

namespace ogdf {

class RandomMove : public NeighbourhoodStructure
{
public:
    RandomMove();
    ~RandomMove();

    virtual List<LayoutChange> generateNeighbouringLayout();
};

}
#endif // RANDOMMOVE_H
