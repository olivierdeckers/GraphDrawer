#ifndef REMOVECROSSING_H
#define REMOVECROSSING_H

#include "NeighbourhoodStructure.h"
#include <ogdf/TSAPlanarity.h>

namespace ogdf {

class RemoveCrossing : public NeighbourhoodStructure
{
public:
    RemoveCrossing(GraphAttributes &GA, TSAPlanarity &planarity);
    ~RemoveCrossing();

    virtual void generateNeighbouringLayout(double temp, List<LayoutChange> &result);

private:
    TSAPlanarity &m_planarity;
};

}
#endif // REMOVECROSSING_H
