#include "MoveCluster.h"

namespace ogdf {

class NodeDegreeComparer {
public:
    bool less(node a, node b) const
    {
        return a->degree() < b->degree();
    }
};

MoveCluster::MoveCluster(ogdf::GraphAttributes &GA) : NeighbourhoodStructure(GA)
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

        m_clusters.pushBack(c);
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
            score /= candidate->degree();

            if( (score >= 0.3 && score > bestScore) || (score == bestScore && candidate->degree() < bestCandidate->degree()) ) {
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

}

}
