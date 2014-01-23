/*
 * $Revision: 2813 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-10-13 14:05:35 +0200 (Sa, 13. Okt 2012) $
 ***************************************************************/

/** \file
 * \brief Implementation of class Repulsion
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

#include "TSARepulsion.h"


namespace ogdf {

    TSARepulsion::TSARepulsion(GraphAttributes &AG, double prefEdgeLength) : NodePairEnergy("Repulsion",AG) {
        int nbNodes = AG.constGraph().numberOfNodes();
        m_scaleFactor = (ceil(sqrt(nbNodes)) - 1) * prefEdgeLength;
    }


    double TSARepulsion::computeCoordEnergy(
		node v1,
		node v2,
		const DPoint &p1,
		const DPoint &p2)
		const
	{
        IntersectionRectangle i1 = shape(v1);
        IntersectionRectangle i2 = shape(v2);
        i1.move(p1);
        i2.move(p2);
        double dist = i1.distance(i2) / m_scaleFactor;
        OGDF_ASSERT(dist >= 0.0);
        double div = (dist+1.0)*(dist+1.0);
        return 1.0/div;
	}

} //namespace ogdf
