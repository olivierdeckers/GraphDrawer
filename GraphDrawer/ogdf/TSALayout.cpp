/** \file
 * \brief Implementation for TSALayout
 *
 * This is the frontend for the TSA optimization
 * function. It adds the energy functions to the problem and
 * sets their weights. It also contains functions for setting
 * and returning the parameters that influence the quality of
 * the solution and the speed of the optimization process.
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

#include "TSALayout.h"
#include "TSARepulsion.h"
#include "TSAAttraction.h"
#include <ogdf/internal/energybased/Overlap.h>
#include "PlanarityApprox.h"
#include "TSAPlanarity.h"
#include "tsaplanaritygrid.h"


#define DEFAULT_REPULSION_WEIGHT 1e6
#define DEFAULT_ATTRACTION_WEIGHT 1e2
#define DEFAULT_OVERLAP_WEIGHT 100
#define DEFAULT_PLANARITY_WEIGHT 500
#define DEFAULT_START_TEMPERATURE 500
#define DEFAULT_TSA_QUALITY 10

namespace ogdf {

struct InputValueInvalid : Exception { InputValueInvalid() : Exception(__FILE__, __LINE__) { } };

struct WeightLessThanZeroException : InputValueInvalid { };

struct IterationsNonPositive : InputValueInvalid { };

struct TemperatureNonPositive : InputValueInvalid { };

TSALayout::TSALayout()
{
	m_repulsionWeight = DEFAULT_REPULSION_WEIGHT;
	m_attractionWeight = DEFAULT_ATTRACTION_WEIGHT;
	m_nodeOverlapWeight = DEFAULT_OVERLAP_WEIGHT;
	m_planarityWeight = DEFAULT_PLANARITY_WEIGHT;
	m_startTemperature = DEFAULT_START_TEMPERATURE;
	m_multiplier = 2.0;
	m_prefEdgeLength = 0.0;
	m_crossings = false;
	m_quality = DEFAULT_TSA_QUALITY;
    m_accStruct = AccelerationStructure::none;
#ifdef GRAPHDRAWER
    worker = NULL;
#endif
}

#ifdef GRAPHDRAWER
void TSALayout::setWorker(LayoutWorker * worker)
{
    this->worker = worker;
}

#endif


void TSALayout::fixSettings(SettingsParameter sp)
{
	double r, a, p, o;
	switch (sp) {
	case spStandard:
		r = 900; a = 250; o = 1450; p = 300; m_crossings = false;
		break;
	case spRepulse:
		r = 9000; a = 250; o = 1450; p = 300; m_crossings = false;
		break;
	case spPlanar:
		r = 900; a = 250; o = 1450; p = 3000; m_crossings = true;
		break;
	default:
		OGDF_THROW_PARAM(AlgorithmFailureException, afcIllegalParameter);
	}//switch
	setRepulsionWeight(r);
	setAttractionWeight(a);
	setNodeOverlapWeight(o);
	setPlanarityWeight(p);
}//fixSettings

void TSALayout::setAccelerationStructureParameter(AccelerationStructure as)
{
    this->m_accStruct = as;
}

void TSALayout::setQuality(double quality)
{
	m_quality = quality;
}


void TSALayout::setRepulsionWeight(double w)
{
	if(w < 0) throw WeightLessThanZeroException();
	else m_repulsionWeight = w;
}


void TSALayout::setAttractionWeight(double w)
{
	if(w < 0) throw WeightLessThanZeroException();
	else m_attractionWeight = w;
}


void TSALayout::setNodeOverlapWeight(double w)
{
	if(w < 0) throw WeightLessThanZeroException();
	else m_nodeOverlapWeight = w;
}


void TSALayout::setPlanarityWeight(double w)
{
	if(w < 0) throw WeightLessThanZeroException();
	else m_planarityWeight = w;
}


void TSALayout::setStartTemperature (int w)
{
	if(w < 0) throw TemperatureNonPositive();
	else m_startTemperature = w;
}

double TSALayout::calculatePreferredEdgeLength(const GraphAttributes &AG, const double multiplier) const {
    double lengthSum(0.0);
    node v;
    forall_nodes(v,AG.constGraph()) {
        lengthSum += AG.width(v);
        lengthSum += AG.height(v);
    }
    lengthSum /= (2*AG.constGraph().numberOfNodes());
    // lengthSum is now the average of all lengths and widths
    return multiplier * lengthSum;
}

//this sets the parameters of the class TSA, adds the energy functions and
//starts the optimization process
void TSALayout::call(GraphAttributes &AG)
{
	// all edges straight-line
	AG.clearAllBends();

    double preferredEdgeLength =
            (DIsGreater(m_prefEdgeLength, 0.0)) ? m_prefEdgeLength : calculatePreferredEdgeLength(AG, m_multiplier);

	TSA dh;
    TSARepulsion rep(AG, preferredEdgeLength);
    TSAAttraction atr(AG, preferredEdgeLength);
    Overlap over(AG);
    EnergyFunction *planarity;
	//NodeIntersection ni(AG);

	dh.addEnergyFunction(&rep,m_repulsionWeight);
	dh.addEnergyFunction(&atr,m_attractionWeight);
	dh.addEnergyFunction(&over,m_nodeOverlapWeight);

    TSAUniformGrid *grid = new TSAUniformGrid(AG);

    if (m_crossings) {
        switch(m_accStruct) {
        case AccelerationStructure::none:
            planarity = new TSAPlanarity(AG);
            cout << "regular" << endl;
            break;
        case AccelerationStructure::approximation:
            planarity = new PlanarityApprox(AG);
            cout << "approx" << endl;
            break;
        case AccelerationStructure::grid:
            planarity = new TSAPlanarityGrid(AG, &grid);
            cout << "grid" << endl;
            break;
        }
        dh.addEnergyFunction(planarity, m_planarityWeight);
    }

	//dh.setNumberOfIterations(m_numberOfIterations);
    //dh.setStartTemperature(m_startTemperature);
	dh.setStartTemperature(m_startTemperature);
	dh.setQuality(m_quality);
#ifdef GRAPHDRAWER
    dh.call(AG, &grid, worker);
#else
    dh.call(AG, &grid);
#endif

    delete planarity;
    delete grid;
}

} // namespace ogdf