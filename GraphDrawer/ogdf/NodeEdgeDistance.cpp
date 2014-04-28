#include "NodeEdgeDistance.h"

namespace ogdf {

NodeEdgeDistance::NodeEdgeDistance(GraphAttributes &GA, AccelerationStructure *accStruct) : TSAPlanarity(GA, accStruct)
{
}

bool NodeEdgeDistance::lowLevelIntersect(const DPoint &e1s, const DPoint &e1t, const DPoint &e2s, const DPoint &e2t, double &energy) const
{
    DPoint s1(e1s),t1(e1t),s2(e2s),t2(e2t);
    DLine l1(s1,t1), l2(s2,t2);
    DPoint inter;
    bool intersect = l1.intersection(l2,inter);
    if(intersect) {
        double dist1 = min(inter.distance(s1), inter.distance(t1));
        double dist2 = min(inter.distance(s2), inter.distance(t2));
        double dist = min(dist1, dist2);
        energy = 1.0/(dist*dist);
    }
    else {
        if(l1.slope() == l2.slope())
        {
            OGDF_ASSERT(false);
        }
        DRect r1 = DRect(l1);
        DRect r2 = DRect(l2);
        if(r1.contains(inter))
        {
            double dist = min(inter.distance(s2), inter.distance(t2));
            energy = 1.0/(dist*dist);
        }
        else if(r2.contains(inter))
        {
            double dist = min(inter.distance(s1), inter.distance(t1));
            energy = 1.0/(dist*dist);
        }
        else
        {
            energy = 0;
        }
    }
    return intersect;
}

}
