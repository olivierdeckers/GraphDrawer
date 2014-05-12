/*
 * $Revision: 2523 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-02 20:59:27 +0200 (Mo, 02. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Declares class Attraction.
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

#ifndef OGDF_TSAATTRACTION_H
#define OGDF_TSAATTRACTION_H


#include <ogdf/TSANodePairEnergy.h>

namespace ogdf {


//! Energy function for attraction between two adjacent vertices.
/**
 * Implements an energy function that simulates
 * attraction between two adjacent vertices. There is an optimum
 * distance where the energy is zero. The energy grows quadratic
 * with the difference to the optimum distance. The optimum
 * distance between two adjacent vertices depends on the size of
 * the two vertices.
 */
class TSAAttraction: public TSANodePairEnergy {
public:
		//Initializes data structures to speed up later computations
        TSAAttraction(GraphAttributes &AG);
        ~TSAAttraction() {}
#ifdef OGDF_DEBUG
		void printInternalData() const;
#endif
private:
	//! computes the energy contributed by the two nodes if they are placed at the two given positions
    double computeCoordEnergy(node,node);
};

}// namespace ogdf

#endif
