#include "MoveCluster.h"
#include <ogdf/basic/Math.h>

namespace ogdf {

class NodeDegreeComparer {
public:
    bool less(node a, node b) const
    {
        return a->degree() > b->degree();
    }
};

MoveCluster::MoveCluster(ogdf::GraphAttributes &GA) : NeighbourhoodStructure(GA, "MoveCluster")
{
    List<node> nodes;
    m_G.allNodes(nodes);
    nodes.quicksort(NodeDegreeComparer());
    int clusterIdx = 0;

    while(!nodes.empty())
    {
        ListIterator<node> n = nodes.get(0);

        Cluster c;
        growCluster(*n, c, nodes, clusterIdx);
        clusterIdx ++;

        if(c.nodes.size() >= 4)
        {
            m_clusters.pushBack(c);
        }
    }
}

MoveCluster::~MoveCluster() {}

void MoveCluster::growCluster(node start, Cluster &c, List<node> &nodes, int clusterIdx)
{
    c.nodes.pushBack(start);
    m_GA.labelNode(start) = String(std::to_string(clusterIdx).data());
    m_GA.labelNode(start) += "s";
    nodes.del(nodes.get(nodes.search(start)));

    List<node> adjacentNodes; // List of nodes adjacent to a node in the cluster
    bool clusterExpanded = true;
    node lastNodeAdded = start;

    while(clusterExpanded)
    {
        clusterExpanded = false;

        adjEntry adj;
        forall_adj(adj, lastNodeAdded) if(nodes.search(adj->twinNode()) != -1 && adjacentNodes.search(adj->twinNode()) == -1)
        {
            adjacentNodes.pushBack(adj->twinNode());
        }

        node bestCandidate;
        double bestScore = 0;

        ListIterator<node> it = adjacentNodes.begin();
        for(; it.valid(); it++)
        {
            node candidate = *it;
            double score = 0.0;

            ListIterator<node> it2;
            for(it2 = c.nodes.begin(); it2.valid(); it2++)
            {
                if(m_G.searchEdge(candidate, *it2) != 0) {
                    score++;
                }
            }
            score /= c.nodes.size() + candidate->degree();

            if( (score >= 0.15 && score > bestScore) ||
                (bestScore == 0 && candidate->degree() == 1) ) {
                bestScore = score;
                bestCandidate = candidate;
            }
        }

        if(bestScore > 0)
        {
            clusterExpanded = true;
            c.nodes.pushBack(bestCandidate);
            m_GA.labelNode(bestCandidate) = String(std::to_string(clusterIdx).data());
            lastNodeAdded = bestCandidate;

            int nodeIdx = nodes.search(bestCandidate);
            OGDF_ASSERT(nodeIdx != -1);
            nodes.del(nodes.get(nodeIdx));

            nodeIdx = adjacentNodes.search(bestCandidate);
            OGDF_ASSERT(nodeIdx != -1);
            adjacentNodes.del(adjacentNodes.get(nodeIdx));
        }
    }

}

void MoveCluster::generateNeighbouringLayout(double temp, Hashing<ogdf::node, ogdf::DPoint> &result)
{
    if(m_clusters.size() <= 1)
        throw "There are no clusters to move";

    int r = randomNumber(0, m_clusters.size() - 1);
    Cluster c = *(m_clusters.get(r));

    double randomAngle = randomDouble(0, 1) * 2.0 * Math::pi;
    double diffX = cos(randomAngle) * diskRadius(temp);
    double diffY = sin(randomAngle) * diskRadius(temp);

    ListIterator<node> it = c.nodes.begin();
    for(; it.valid(); it++)
    {
        DPoint newPos;
        newPos.m_x = m_GA.x(*it) + diffX;
        newPos.m_y = m_GA.y(*it) + diffY;
        result.insert(*it, newPos);
    }
}

}
