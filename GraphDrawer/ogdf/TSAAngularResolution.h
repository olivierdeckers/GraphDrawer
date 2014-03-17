#ifndef TSAANGULARRESOLUTION_H
#define TSAANGULARRESOLUTION_H

#include <ogdf/internal/energybased/EnergyFunction.h>

namespace ogdf {

class TSAAngularResolution : public EnergyFunction
{
public:
    TSAAngularResolution(GraphAttributes &GA);
    ~TSAAngularResolution();

    //! Computes energy of initial layout and stores it in \a m_energy.
    void computeEnergy();

#ifdef OGDF_DEBUG
    void printInternalData() const;
#endif

protected:

    //! Computes energy of candidate.
    void compCandEnergy();

    //! Changes internal data if candidate is taken.
    void internalCandidateTaken();

    NodeArray<double> m_nodeEnergy;

    struct NodeEnergyChange {
        node v;
        double angRes;
    };
    List<NodeEnergyChange> m_candidateNodeEnergy;

private:
    double computeAngularResolution(node v, node movedNode, DPoint& newPos) const;
};

}

#endif // TSAANGULARRESOLUTION_H
