#ifndef REMOVECROSSING_H
#define REMOVECROSSING_H

#include "NeighbourhoodStructure.h"

namespace ogdf {

class RemoveCrossing : public NeighbourhoodStructure
{
public:
    RemoveCrossing(GraphAttributes &GA);
    ~RemoveCrossing();

    virtual List<LayoutChange> generateNeighbouringLayout(double temp);
};

}
#endif // REMOVECROSSING_H
