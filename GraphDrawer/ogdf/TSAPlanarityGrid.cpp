/*
 * $Revision: 2552 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-05 16:45:20 +0200 (Do, 05. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Implementation of class PlanarityGrid
 *
 * The PlanarityGrid energy function counts the number of
 * crossings. It contains two UniformGrids: One for the
 * current layout and one for the candidate layout.
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


#include "TSAPlanarityGrid.h"

namespace ogdf {

    TSAPlanarityGrid::~TSAPlanarityGrid()
    {
    }

    TSAPlanarityGrid::TSAPlanarityGrid(GraphAttributes &AG, AccelerationStructure *accStruct):
    EnergyFunction("PlanarityGrid",AG),
      m_layout(AG),
      m_crossings(AG.constGraph())
    {
        m_accStruct = accStruct;
    }

    void TSAPlanarityGrid::computeEnergy()
    {
        List<edge> edges;
        m_G.allEdges(edges);
        node v = m_G.firstNode();
        computeCrossings(edges, v, currentPos(v));
    }

    void TSAPlanarityGrid::compCandEnergy()
    {
        node v = testNode();
        DPoint newPos = testPos();

        int crossings = m_energy;

        List<edge> incident;
        m_G.adjEdges(v,incident);

        ListIterator<edge> it1;
        for(it1 = incident.begin(); it1.valid(); ++it1) {
            edge& e = *it1;

            crossings -= m_crossings[e].size();
        }

        //now we compute all the remaining data of the class in one loop
        //going through all edges and storing them in the grid.
        ListConstIterator<edge> it;
        for(it = incident.begin(); it.valid(); ++it) {
            const edge& e = *it;
            DPoint sPos,tPos;
            const node& s = e->source();
            if(s != v) sPos = DPoint(m_layout.x(s),m_layout.y(s));
            else sPos = newPos;
            const node& t = e->target();
            if(t != v) tPos = DPoint(m_layout.x(t),m_layout.y(t));
            else tPos = newPos;

            List<edge> possibleCrossings;
            m_accStruct->possibleCrossingEdges(sPos, tPos, possibleCrossings);

            ListConstIterator<edge> it2;
            for(it2 = possibleCrossings.begin(); it2.valid(); ++it2) {
                if(crossingTest(e,*it2,v,newPos)) { //two edges cross in p
                    ++crossings;
                }
            }
        }

        m_candidateEnergy = crossings;
    }

    void TSAPlanarityGrid::internalCandidateTaken() {
    }

    void TSAPlanarityGrid::computeCrossings(
        const List<edge>& toInsert,
        const node moved,
        const DPoint& newPos)
    {
        m_energy = 0;
        //now we compute all the remaining data of the class in one loop
        //going through all edges and storing them in the grid.
        ListConstIterator<edge> it;
        for(it = toInsert.begin(); it.valid(); ++it) {
            const edge& e = *it;
            DPoint sPos,tPos;
            const node& s = e->source();
            if(s != moved) sPos = DPoint(m_layout.x(s),m_layout.y(s));
            else sPos = newPos;
            const node& t = e->target();
            if(t != moved) tPos = DPoint(m_layout.x(t),m_layout.y(t));
            else tPos = newPos;

            List<edge> possibleCrossings;
            m_accStruct->possibleCrossingEdges(sPos, tPos, possibleCrossings);

            ListConstIterator<edge> it2;
            for(it2 = possibleCrossings.begin(); it2.valid(); ++it2) {
                if(crossingTest(e,*it2,moved,newPos)) {
                    ++m_energy;
                    m_crossings[e].pushBack(*it2);
                    m_crossings[*it2].pushBack(e);
                }
            }
        }
    }


    //returns true if both edges are not adjacent and cross inside the given cell
    bool TSAPlanarityGrid::crossingTest(
        const edge e1,
        const edge e2,
        const node moved,
        const DPoint& newPos) const
    {
        node s1 = e1->source(), t1 = e1->target();
        node s2 = e2->source(), t2 = e2->target();
        if(s1 != s2 && s1 != t2 && t1 != s2 && t1 != t2) {//not adjacent
            DPoint ps1,pt1,ps2,pt2;
            if(s1 != moved) ps1 = DPoint(m_layout.x(s1),m_layout.y(s1));
            else ps1 = newPos;
            if(t1 != moved) pt1 = DPoint(m_layout.x(t1),m_layout.y(t1));
            else pt1 = newPos;
            if(s2 != moved) ps2 = DPoint(m_layout.x(s2),m_layout.y(s2));
            else ps2 = newPos;
            if(t2 != moved) pt2 = DPoint(m_layout.x(t2),m_layout.y(t2));
            else pt2 = newPos;
            DLine l1(ps1,pt1),l2(ps2,pt2);
            DPoint crossPoint;
            if(l1.intersection(l2,crossPoint)) {
                return true;
            }
        }
        return false;
    }

}
