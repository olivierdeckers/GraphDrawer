#include "tsaangularresolution.h"

namespace ogdf {

TSAAngularResolution::TSAAngularResolution(GraphAttributes &GA) : EnergyFunction("AngularResolution", GA)
{
}

TSAAngularResolution::~TSAAngularResolution() {}

void TSAAngularResolution::computeEnergy()
{
    m_energy = 0;

    node v;
    forall_nodes(v, m_G) {
        int degree = v->degree();
        double *angles = new double[degree];
        int i=0;
        edge e;
        forall_adj_edges(e, v) {
            DPoint pos = currentPos(e->opposite(v));
            angles[i] = atan2(pos.m_y, pos.m_x);
        }

        std::sort(angles, angles + degree);

        double minAngle = 2*M_PI;
        for(int i=1; i<degree; i++) {
            minAngle = min(minAngle, angles[i] - angles[i-1]);
        }
        minAngle = min(minAngle, angles[0] + 2*M_PI - angles[degree-1]);

        m_energy += 2*M_PI / (double) degree - minAngle;
    }
}

void TSAAngularResolution::compCandEnergy()
{
}

void TSAAngularResolution::internalCandidateTaken()
{
}

}
