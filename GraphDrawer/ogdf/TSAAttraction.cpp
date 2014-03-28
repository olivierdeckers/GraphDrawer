/*
 * $Revision: 2552 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-05 16:45:20 +0200 (Do, 05. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Implements class Attraction.
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

#include "TSAAttraction.h"

namespace ogdf {

//initializes internal data, like name and layout
TSAAttraction::TSAAttraction(GraphAttributes &AG, double preferredEdgeLength) :
    TSANodePairEnergy("Attraction", AG) {
    int nbNodes = AG.constGraph().numberOfNodes();
    m_scaleFactor = ((ceil(sqrt(nbNodes)) - 1) * preferredEdgeLength);

}

double TSAAttraction::computeCoordEnergy(node v1, node v2)
const
{
	double energy = 0.0;
	if(adjacent(v1,v2)) {
		IntersectionRectangle i1(shape(v1)), i2(shape(v2));
        i1.move(newPos(v1));
        i2.move(newPos(v2));
        energy = i1.distance(i2) / m_scaleFactor;
        energy *= energy;
	}
	return energy;
}


#ifdef OGDF_DEBUG
void TSAAttraction::printInternalData() const {
    TSANodePairEnergy::printInternalData();
    cout << "\nScale factor: " << m_scaleFactor;
}
#endif

}// namespace ogdf
