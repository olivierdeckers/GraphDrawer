/** \file
 * \brief Declares class TSALayout, which is a front-end
 * for the TSA class.
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

#ifndef OGDF_TSA_LAYOUT_H
#define OGDF_TSA_LAYOUT_H


#include <ogdf/module/LayoutModule.h>
#include "ogdf/TSA.h"
#include <ogdf/AccelerationStructure.h>
#ifdef GRAPHDRAWER
#include "gui/layoutworker.h"
#endif


namespace ogdf {

class OGDF_EXPORT TSALayout : public LayoutModule
{
public:
	//! Easy way to set fixed costs
	enum SettingsParameter {spStandard, spRepulse, spPlanar}; //tuning of costs

    enum AccelerationStructureType {grid, approximation, none};

	//! Creates an instance of TSA layout.
    TSALayout();

	~TSALayout(){}

#ifdef GRAPHDRAWER
    void setWorker(LayoutWorker *worker);
#endif

    void setAccelerationStructureParameter(AccelerationStructureType as);

	//! Calls the layout algorithm for graph attributes \a GA.
    void call(GraphAttributes &GA);

	//! Fixes the cost values to special configurations.
	void fixSettings(SettingsParameter sp);

	void setQuality(double quality);

	//! Sets the preferred edge length multiplier for attraction.
	/**
	 * This is bad design, cause you dont need to have an attraction function,
	 * DH is purely modular and independent with its cost functions.
	 */
	void setPreferredEdgeLengthMultiplier(double multi) {m_multiplier = multi;}

	//! Sets the preferred edge length to \a elen
	void setPreferredEdgeLength(double elen) {m_prefEdgeLength = elen;}

	//! Sets the weight for the energy function \a Repulsion.
	void setRepulsionWeight(double w);

	//! Returns the weight for the energy function \a Repulsion.
	double getRepulsionWeight() const {return m_repulsionWeight;}

	//! Sets the weight for the energy function \a Attraction.
	void setAttractionWeight(double);

	//! Returns the weight for the energy function \a Attraction.
	double getAttractionWeight() const {return m_attractionWeight;}

	//! Sets the weight for the energy function \a NodeOverlap.
	void setNodeOverlapWeight(double);

	//! Returns the weight for the energy function \a NodeOverlap.
	double getNodeOverlapWeight() const {return m_nodeOverlapWeight;}

    void setAngularResolutionWeight(double);

    double getAngularResolutionWeight() const {return m_angResWeight;}

	//! Sets the weight for the energy function \a Planarity.
	void setPlanarityWeight(double);

	//! Returns the weight for the energy function \a Planarity.
	double getPlanarityWeight() const {return m_planarityWeight;}

	//! Sets the starting temperature to \a t.
	void setStartTemperature(int t);

	//! Returns the starting temperature.
	int getStartTemperature() const {return m_startTemperature;}

private:
    double calculatePreferredEdgeLength(const GraphAttributes &G, const double multiplier) const;

	double m_repulsionWeight;   //!< The weight for repulsion energy.
	double m_attractionWeight;  //!< The weight for attraction energy.
	double m_nodeOverlapWeight; //!< The weight for node overlap energy.
    double m_angResWeight;
	double m_planarityWeight;   //!< The weight for edge crossing energy.
    double m_startTemperature;     //!< The temperature at the start of the optimization.
	double m_multiplier;        //!< edge length multiplier
	double m_prefEdgeLength;    //!< Preferred edge length (abs value), only used if > 0
	bool m_crossings;           //!< Should crossings be computed?
	double m_quality;
    AccelerationStructureType m_accStruct;

#ifdef GRAPHDRAWER
    LayoutWorker *worker;
#endif
};

}
#endif
