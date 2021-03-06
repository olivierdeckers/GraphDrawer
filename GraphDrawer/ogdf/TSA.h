/*
 * $Revision: 2641 $
 *
 * last checkin:
 *   $Author: Olivier Deckers $
 *   $Date: 2013-11-15 11:30:36 +0200 $
 ***************************************************************/

/** \file
 * \brief Declares class TSA which implements the
 * thermodynamic simulated annealing approach for drawing graphs.
 *
 * \author Olivier Deckers
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#ifdef _MSC_VER
#pragma once
#endif

#ifndef OGDF_TSA_H
#define OGDF_TSA_H


#include <ogdf/TSAEnergyFunction.h>
#include <ogdf/NeighbourhoodStructure.h>
#include <ogdf/TSAUniformGrid.h>
#ifdef GRAPHDRAWER
#include "gui/layoutworker.h"
#endif


namespace ogdf {


//! The TSA approach for drawing graphs.
class OGDF_EXPORT TSA
{
public:

	//! Creates an instance of Davidsen-Harel base class.
	TSA();

	~TSA() { }

	//! Sets the start temperature to \a startTemp.
	void setStartTemperature(double startTemp);

    void setQuality(double quality);

	//! Adds an energy function \a F with a certain weight.
    void addEnergyFunction(TSAEnergyFunction *F, double weight);

    void addNeighbourhoodStructure(NeighbourhoodStructure *ns, double weight);

	//! Returns a list of the names of the energy functions.
    List<String> returnEnergyFunctionNames();

	//! Returns a list of the weights of the energy functions.
	List<double> returnEnergyFunctionWeights();

	//! Calls the TSA method for graph \a GA.
#ifdef GRAPHDRAWER
    void call(GraphAttributes &AG, AccelerationStructure *grid, LayoutWorker *worker);
#else
    void call(GraphAttributes &AG, AccelerationStructure *grid);
#endif

private:
	//! The default starting temperature.
    double m_startingTemp;
	//! The default end temperature
	const static double m_defaultEndTemperature;

	double m_temperature;          //!< The temperature during the annealing process.
    double m_energy;            //!< The current energy of the system.
    double m_candEnergy;
	double m_quality;			//!< The quality/runtime tradeoff parameter: higher values means higher quality and higher runtime
	double m_endTemperature;	//!< The stop condition for temperature
    double m_totalCostDiff;
    double m_totalEntropyDiff;

    List<TSAEnergyFunction*> m_energyFunctions; //!< The list of the energy functions.
    List<double> m_weightsOfEnergyFunctions; //!< The list of the weights for the energy functions.
    List<NeighbourhoodStructure*> m_neighbourhoodStructures;
    List<double> m_neighbourhoodSelectionChances;
    double m_rewardParameter, m_penaltyParameter;

	//! Resets the parameters for subsequent runs.
	void initParameters();

	//! Tests if new energy value satisfies annealing property (only better if m_fineTune).
	bool testEnergyValue(double newVal);

    double compCandEnergy(Hashing<node,DPoint> &layoutChanges);

	//! Computes a random number between zero and one
	double randNum() const;

	//! Computes the energy of the initial layout and stores it in \a m_energy.
	void computeInitialEnergy();

	//! Computes positions for the vertices of degree zero.
	void placeIsolatedNodes(GraphAttributes &AG) const;

    int chooseNeighbourhood(int nbIterations) const;

    void updateNeighbourhoodChances(double costDiff, int neighbourhoodUsed);

    void acceptChanges(GraphAttributes &AG, Hashing<node,DPoint> &layoutChanges, AccelerationStructure *grid);

    void postProcessingPhase(GraphAttributes &AG, AccelerationStructure *grid);

	//! Fake assignment operator (dummy to avoid copying)
	TSA& operator=(const TSA &dh);
	//! Fake copy constructor (dummy to avoid copying)
	TSA(const TSA &) { }
};

} //end namespace
#endif
