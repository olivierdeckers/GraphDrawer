/*
 * $Revision: 2523 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-02 20:59:27 +0200 (Mo, 02. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Declaration of class Planarity which implements an
 *        energy function where the energy of a layout depends
 *        on the number of crossings.
 *
 * \author Rene Weiskircher
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

#ifndef OGDF_TSAPLANARITY_H
#define OGDF_TSAPLANARITY_H


#include <ogdf/TSAEnergyFunction.h>
#include <ogdf/basic/Array2D.h>
#include <ogdf/AccelerationStructure.h>
#include <ogdf/basic/Graph.h>


namespace ogdf {


class TSAPlanarity: public TSAEnergyFunction {
public:
	//! Initializes data structures to speed up later computations.
    TSAPlanarity(GraphAttributes &AG, AccelerationStructure *accStruct);

    ~TSAPlanarity();

	//! Computes energy of initial layout and stores it in \a m_energy.
    void computeEnergy();

    struct Crossing {
        edge edge1;
        edge edge2;
    };

    Crossing getRandomCrossing() const;

protected:
	struct ChangedCrossing {
		int edgeNum1;
		int edgeNum2;
		double crossEnergy;
	};

	//! Returns 1 if edges cross else 0.
    bool intersect(const edge, const edge, double&) const;

	//! Changes internal data if candidate is taken.
	void internalCandidateTaken();

	//! Releases memory allocated for \a m_candidateCrossings.
	void clearCandidateCrossings();

	//! Tests if two lines given by four points intersect.
    virtual bool lowLevelIntersect( const DPoint&, const DPoint&, const DPoint&,
		 const DPoint&, double&) const;

    void compCandEnergy();

#ifdef OGDF_DEBUG
    virtual void printInternalData() const;
#endif

	EdgeArray<int> *m_edgeNums; //!< Maps an edge to the index used in the crossingsMatrix
	Array2D<double> *m_crossingMatrix; //!< stores for each pair of edges if they cross

	/**
	 * stores for all edges incident to the test node
	 * an array with the crossings that change if the candidate position is chosen
	 */
    List<ChangedCrossing> m_crossingChanges;

    List<edge> m_nonSelfLoops; //!< list of edges that are not slef loops

    AccelerationStructure *m_accStruct;
}; // class Planarity


}// namespace ogdf

#endif
