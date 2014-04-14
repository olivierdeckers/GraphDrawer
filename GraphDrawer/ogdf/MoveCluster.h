#ifndef MOVECLUSTER_H
#define MOVECLUSTER_H

#include <ogdf/NeighbourhoodStructure.h>

namespace ogdf {

class MoveCluster : public NeighbourhoodStructure
{
public:
    MoveCluster(GraphAttributes &GA);
    ~MoveCluster();

    virtual void generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result);

    struct Cluster {
        List<node> nodes;
    };

private:
    List<Cluster> m_clusters;

    void growCluster(node start, Cluster &c, List<node> &nodes, int idx);
};

}
#endif // MOVECLUSTER_H
