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

protected:

    //! Computes energy of candidate.
    void compCandEnergy();

    //! Changes internal data if candidate is taken.
    void internalCandidateTaken();

    NodeArray<double> *m_nodeResolutions;
};

}

#endif // TSAANGULARRESOLUTION_H
