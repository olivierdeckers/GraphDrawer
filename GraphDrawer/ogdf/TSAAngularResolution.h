#ifndef TSAANGULARRESOLUTION_H
#define TSAANGULARRESOLUTION_H

#include <ogdf/TSAEnergyFunction.h>

namespace ogdf {

class TSAAngularResolution : public TSAEnergyFunction {
public:
    TSAAngularResolution(GraphAttributes &GA);
    ~TSAAngularResolution();

    //! Computes energy of initial layout and stores it in \a m_energy.
    void computeEnergy();

#ifdef OGDF_DEBUG
    void printInternalData() const;
#endif

protected:

    //! Changes internal data if candidate is taken.
    void internalCandidateTaken();

    void compCandEnergy();

    NodeArray<double> m_nodeEnergy;

    struct NodeEnergyChange {
        node v;
        double angRes;
    };
    List<NodeEnergyChange> m_candidateNodeEnergy;

private:
    double computeAngularResolution(node v) const;
};

}

#endif // TSAANGULARRESOLUTION_H
