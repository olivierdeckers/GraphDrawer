#ifndef REMOVECROSSING_H
#define REMOVECROSSING_H

#include "NeighbourhoodStructure.h"

namespace ogdf {

class RemoveCrossing : public NeighbourhoodStructure
{
public:
    RemoveCrossing();
    ~RemoveCrossing();

    virtual List<LayoutChange> generateNeighbouringLayout();
};

}
#endif // REMOVECROSSING_H
