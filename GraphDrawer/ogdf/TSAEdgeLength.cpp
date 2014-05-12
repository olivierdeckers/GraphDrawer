#include "TSAEdgeLength.h"

namespace ogdf {


TSAEdgeLength::TSAEdgeLength(GraphAttributes &GA, double prefferredEdgeLength) : TSANodePairEnergy("EdgeLength", GA),
    m_prefEdgeLength(prefferredEdgeLength)
{

}

double TSAEdgeLength::computeCoordEnergy(node a, node b)
{
    double energy = 0.0;
    if(adjacent(a,b)) {
        IntersectionRectangle i1(shape(a)), i2(shape(b));
        i1.move(newPos(a));
        i2.move(newPos(b));
        double length = i1.distance(i2);

        energy = length - m_prefEdgeLength;
        energy *= energy;
    }

    return energy;
}

}
