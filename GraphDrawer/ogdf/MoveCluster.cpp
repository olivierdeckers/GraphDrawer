#include "MoveCluster.h"

namespace ogdf {

MoveCluster::MoveCluster(ogdf::GraphAttributes &GA) : NeighbourhoodStructure(GA)
{
    List<node> nodes;
    m_G.allNodes(nodes);
    int clusterIdx = 0;

    while(!nodes.empty())
    {
        int r = randomNumber(0, nodes.size() - 1);
        ListIterator<node> n = nodes.get(r);

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
    nodes.del(nodes.get(nodes.search(start)));

    bool clusterExpanded = true;
    node lastNodeAdded = start;

    while(clusterExpanded)
    {
        clusterExpanded = false;

        node bestCandidate;
        double bestScore = 0;

        adjEntry adj;
        forall_adj(adj, lastNodeAdded) if(nodes.search(adj->twinNode()) != -1)
        {
            node candidate = adj->twinNode();
            double score = 0.0;

            ListIterator<node> it;
            for(it = c.nodes.begin(); it.valid(); it++)
            {
                if(m_G.searchEdge(candidate, *it) != 0) {
                    score++;
                }
            }
            score /= c.nodes.size();

            if(score > 0.5 && score > bestScore) {
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
        }
    }

}

void MoveCluster::generateNeighbouringLayout(double temp, Hashing<ogdf::node, ogdf::DPoint> &result)
{

}

}
