///*
// * $Revision: 2523 $
// *
// * last checkin:
// *   $Author: gutwenger $
// *   $Date: 2012-07-02 20:59:27 +0200 (Mo, 02. Jul 2012) $
// ***************************************************************/

///** \file
// * \brief Declaration of class Planarity which implements an
// *        energy function where the energy of a layout depends
// *        on the number of crossings.
// *
// * \author Rene Weiskircher
// *
// * \par License:
// * This file is part of the Open Graph Drawing Framework (OGDF).
// *
// * \par
// * Copyright (C)<br>
// * See README.txt in the root directory of the OGDF installation for details.
// *
// * \par
// * This program is free software; you can redistribute it and/or
// * modify it under the terms of the GNU General Public License
// * Version 2 or 3 as published by the Free Software Foundation;
// * see the file LICENSE.txt included in the packaging of this file
// * for details.
// *
// * \par
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * \par
// * You should have received a copy of the GNU General Public
// * License along with this program; if not, write to the Free
// * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// * Boston, MA 02110-1301, USA.
// *
// * \see  http://www.gnu.org/copyleft/gpl.html
// ***************************************************************/


//#ifdef _MSC_VER
//#pragma once
//#endif

//#ifndef OGDF_PLANARITYAPPROX_H
//#define OGDF_PLANARITYAPPROX_H


//#include "TSAPlanarity.h"
//#include <ogdf/basic/Array2D.h>


//namespace ogdf {


//class PlanarityApprox: public TSAPlanarity {
//public:
//	//! Initializes data structures to speed up later computations.
//    PlanarityApprox(GraphAttributes &GA, AccelerationStructure *accStruct);

//	~PlanarityApprox();

//    double computeCandidateEnergy(const Hashing<node, DPoint> &layoutChanges);
//private:

//    void calculateCloseNeighbours();

//#ifdef OGDF_DEBUG
//    virtual void printInternalData() const;
//#endif

//    static const int NEIGHBOUR_DEPTH = 3;
//    EdgeArray<List<edge>*> *m_closeNeighbours; //!< Stores a list of edges to be checked for intersection (reachable in maximum NEIGHBOUR_DEPTH hops)
//};


//}// namespace ogdf

//#endif
