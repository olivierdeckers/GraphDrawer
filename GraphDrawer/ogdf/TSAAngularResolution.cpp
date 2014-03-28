#include "TSAAngularResolution.h"

namespace ogdf {

TSAAngularResolution::TSAAngularResolution(GraphAttributes &GA) : TSAEnergyFunction("AngularResolution", GA),
    m_nodeEnergy(m_G, 0),
    m_candidateNodeEnergy()
{
}

TSAAngularResolution::~TSAAngularResolution()
{
}

void TSAAngularResolution::computeEnergy()
{
    m_energy = 0;

    node v;
    forall_nodes(v, m_G) {
        DPoint pos = currentPos(v);
        double angRes = computeAngularResolution(v);
        m_nodeEnergy[v] = angRes;
        m_energy += angRes;
    }
}

void TSAAngularResolution::compCandEnergy()
{
    m_candidateNodeEnergy.clear();
    m_candidateEnergy = energy();

    HashConstIterator<node, DPoint> it;
    for(it = m_layoutChanges->begin(); it.valid(); ++it)
    {
        node v = it.key();
        edge e;
        forall_adj_edges(e, v) {
            node opposite = e->opposite(v);

            m_candidateEnergy -= m_nodeEnergy[opposite];
            double energy = computeAngularResolution(opposite);
            NodeEnergyChange nec;
            nec.angRes = energy;
            nec.v = opposite;
            m_candidateNodeEnergy.pushBack(nec);
            m_candidateEnergy += energy;
        }

        m_candidateEnergy -= m_nodeEnergy[v];
        double energy = computeAngularResolution(v);
        NodeEnergyChange nec;
        nec.angRes = energy;
        nec.v = v;
        m_candidateNodeEnergy.pushBack(nec);
        m_candidateEnergy += energy;
    }
}

double TSAAngularResolution::computeAngularResolution(node v) const
{
    int degree = v->degree();
    double *angles = new double[degree];
    DPoint pos = newPos(v);
    int i=0;
    edge e;
    forall_adj_edges(e, v) {
        node opposite = e->opposite(v);
        DPoint pos2 = newPos(opposite);
        angles[i] = atan2(pos2.m_y - pos.m_y, pos2.m_x - pos.m_x);
        i++;
    }

    std::sort(angles, angles + degree);

    double minAngle = 2*M_PI;
    for(int i=1; i<degree; i++) {
        minAngle = min(minAngle, angles[i] - angles[i-1]);
    }
    minAngle = min(minAngle, angles[0] + 2*M_PI - angles[degree-1]);

    double idealAngle = 2*M_PI / (double) degree;

    return (idealAngle - minAngle) / idealAngle;
}

void TSAAngularResolution::internalCandidateTaken()
{
    ListIterator<NodeEnergyChange> it;
    for(it = m_candidateNodeEnergy.begin(); it.valid(); it++) {
        NodeEnergyChange nec = *it;
        m_nodeEnergy[nec.v] = nec.angRes;
    }
}

#ifdef OGDF_DEBUG
void TSAAngularResolution::printInternalData() const
{
}
#endif

}
