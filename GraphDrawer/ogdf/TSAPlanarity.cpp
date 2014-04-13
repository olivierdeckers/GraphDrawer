/*
 * $Revision: 2565 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-07 17:14:54 +0200 (Sa, 07. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Implementation of class Planarity
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

#include "TSAPlanarity.h"

namespace ogdf {

    TSAPlanarity::~TSAPlanarity()
	{
		delete m_edgeNums;
        delete m_crossingMatrix;
	}


	// intializes number of edges and allocates memory for  crossingMatrix
    TSAPlanarity::TSAPlanarity(GraphAttributes &AG, AccelerationStructure *accStruct):
    TSAEnergyFunction("Planarity",AG),
      m_accStruct(accStruct)
	{
        m_edgeNums = OGDF_NEW EdgeArray<int>(m_G,0);
        m_G.allEdges(m_nonSelfLoops);
		ListIterator<edge> it, itSucc;
        for(it = m_nonSelfLoops.begin(); it.valid(); it = itSucc) {
			itSucc = it.succ();
            if((*it)->isSelfLoop()) m_nonSelfLoops.del(it);
		}
		int e_num = 1;
        for(it = m_nonSelfLoops.begin(); it.valid(); ++it) (*m_edgeNums)[*it] = e_num ++;
		e_num --;
        m_crossingMatrix = new Array2D<double> (1,e_num,1,e_num,0);
	}


	// computes energy of layout, stores it and sets the crossingMatrix
    void TSAPlanarity::computeEnergy()
    {
        double energySum = 0;

        ListIterator<edge> it;
        for(it = m_nonSelfLoops.begin(); it.valid(); it++) {
            edge e = *it;
            int i = (*m_edgeNums)[e];

            List<edge> possibleCrossings;
            m_accStruct->possibleCrossingEdges(currentPos(e->source()), currentPos(e->target()), possibleCrossings);
            for(ListIterator<edge> it2 = possibleCrossings.begin(); it2.valid(); it2++) {
                int j = (*m_edgeNums)[*it2];
                if(j > i) {
                    double energy = 0;
                    intersect(*it,*it2, energy);
                    (*m_crossingMatrix)(i,j) = energy;
                    energySum += energy;
                }
			}
		}
        m_energy = energySum;
    }

    TSAPlanarity::Crossing TSAPlanarity::getRandomCrossing() const
    {
        if(m_energy > 0)
        {
            List<Crossing> crossings;

            ListConstIterator<edge> it1;
            for(it1 = m_nonSelfLoops.begin(); it1.valid(); it1++)
            {
                ListConstIterator<edge> it2;
                for(it2 = m_nonSelfLoops.begin(); it2.valid(); it2++)
                {
                    int i = (*m_edgeNums)[*it1];
                    int j = (*m_edgeNums)[*it2];
                    if((*m_crossingMatrix)(min(i, j), max(i, j)) > 0)
                    {
                        Crossing c;
                        c.edge1 = *it1;
                        c.edge2 = *it2;
                        crossings.pushBack(c);
                    }
                }
            }

            int random = randomNumber(0, crossings.size() - 1);
            return *(crossings.get(random));
        }
        else
        {
            throw "There are no crossings to return";
        }
    }


	// tests if two edges cross
    bool TSAPlanarity::intersect(const edge e1, const edge e2, double &energy) const
	{
		node v1s = e1->source();
		node v1t = e1->target();
		node v2s = e2->source();
		node v2t = e2->target();

		bool cross = false;
		DPoint inter;
		if(v1s != v2s && v1s != v2t && v1t != v2s && v1t != v2t)
			cross = lowLevelIntersect(currentPos(v1s),currentPos(v1t), currentPos(v2s),currentPos(v2t), energy);
		return cross;
	}


	// tests if two lines given by four points cross
    bool TSAPlanarity::lowLevelIntersect(
		const DPoint &e1s,
		const DPoint &e1t,
		const DPoint &e2s,
		const DPoint &e2t,
		double &energy) const
	{
		DPoint s1(e1s),t1(e1t),s2(e2s),t2(e2t);
		DLine l1(s1,t1), l2(s2,t2);
		DPoint dummy;
		bool intersect = l1.intersection(l2,dummy);
        if(intersect) {
            energy = 1;
		}
		else {
			energy = 0;
		}
		return intersect;
	}


	// computes the energy if the node returned by testNode() is moved
	// to position testPos().
    void TSAPlanarity::compCandEnergy()
    {
        m_candidateEnergy = energy();
        m_crossingChanges.clear();

        HashConstIterator<node, DPoint> it;
        for(it = m_layoutChanges->begin(); it.valid(); ++it)
        {
            node v = it.key();

            edge e;
            forall_adj_edges(e,v) if(!e->isSelfLoop()) {
                // If oppsosite node is moved as well, make sure it is not handled twice!
                node opposite = e->opposite(v);
                if(m_layoutChanges->member(opposite) && opposite->index() < v->index())
                    continue;

                node s = e->source();
                node t = e->target();
                DPoint p1 = newPos(s);
                DPoint p2 = newPos(t);
                int e_num = (*m_edgeNums)[e];

                for(int i=1; i<=m_nonSelfLoops.size(); i++)  if(i != e_num) {
                    double crossingEnergy = (*m_crossingMatrix)(min(i, e_num), max(i, e_num));
                    if(crossingEnergy > 0) {
                        bool alreadyProcessed = false;
                        for(ListConstIterator<ChangedCrossing> it2 = m_crossingChanges.begin(); it2.valid(); it2++)
                        {
                            ChangedCrossing cc = *it2;
                            if(cc.edgeNum1 == min(i, e_num) && cc.edgeNum2 == max(i, e_num)) {
                                alreadyProcessed = true;
                                break;
                            }
                        }
                        if(!alreadyProcessed) {
                            m_candidateEnergy -= crossingEnergy;
                            ChangedCrossing cc;
                            cc.edgeNum1 = min(i, e_num);
                            cc.edgeNum2 = max(i, e_num);
                            cc.crossEnergy = 0;
                            m_crossingChanges.pushBack(cc);
                        }
                    }
                }

                ListIterator<edge> it;
                List<edge> possibleCrossings;
                m_accStruct->possibleCrossingEdges(p1, p2, possibleCrossings);
                for(it = possibleCrossings.begin(); it.valid(); ++it) if(*it != e) {
                    edge f = *it;
                    node s2 = f->source();
                    node t2 = f->target();
                    if(s2 != s && s2 != t && t2 != s && t2 != t && !m_layoutChanges->member(s2) && !m_layoutChanges->member(t2)) {
                        double intersectEnergy = 0;
                        lowLevelIntersect(p1,p2,newPos(s2),newPos(t2), intersectEnergy);
                        int f_num = (*m_edgeNums)[f];

                        if(intersectEnergy > 0) {
                            m_candidateEnergy += intersectEnergy; // produced a new intersection
                            ChangedCrossing cc;
                            cc.edgeNum1 = min(e_num,f_num);
                            cc.edgeNum2 = max(e_num,f_num);
                            cc.crossEnergy = intersectEnergy;
                            m_crossingChanges.pushBack(cc);
                        }
                    }
                }

                HashConstIterator<node, DPoint> it2;
                for(it2 = m_layoutChanges->begin(); it2.valid(); ++it2) if(it2.key() != v)
                {
                    node v2 = it2.key();
                    edge e2;
                    forall_adj_edges(e2, v2) if(!e->isSelfLoop()) {
                        node s2 = e2->source();
                        node t2 = e2->target();
                        if(s2 != s && s2 != t && t2 != s && t2 != t) {
                            double intersectEnergy = 0;
                            lowLevelIntersect(p1,p2,newPos(s2),newPos(t2), intersectEnergy);
                            int f_num = (*m_edgeNums)[e2];

                            if(intersectEnergy > 0) {
                                bool alreadyProcessed = false;
                                for(ListConstIterator<ChangedCrossing> it2 = m_crossingChanges.begin(); it2.valid(); it2++)
                                {
                                    ChangedCrossing cc = *it2;
                                    if(cc.edgeNum1 == min(e_num, f_num) && cc.edgeNum2 == max(e_num, f_num) && cc.crossEnergy > 0) {
                                        alreadyProcessed = true;
                                        break;
                                    }
                                }
                                if(!alreadyProcessed) {
                                    m_candidateEnergy += intersectEnergy; // produced a new intersection
                                    ChangedCrossing cc;
                                    cc.edgeNum1 = min(e_num,f_num);
                                    cc.edgeNum2 = max(e_num,f_num);
                                    cc.crossEnergy = intersectEnergy;
                                    m_crossingChanges.pushBack(cc);
                                }
                            }
                        }
                    }
                }
            }
        }

        if(m_candidateEnergy < 0) {
            OGDF_ASSERT(m_candidateEnergy > -1e-6);
            m_candidateEnergy = 0;
        }
	}


	// this functions sets the crossingMatrix according to candidateCrossings
    void TSAPlanarity::internalCandidateTaken() {
		ListIterator<ChangedCrossing> it;
		for(it = m_crossingChanges.begin(); it.valid(); ++ it) {
			ChangedCrossing cc = *(it);
			(*m_crossingMatrix)(cc.edgeNum1,cc.edgeNum2) = cc.crossEnergy;
        }
	}


#ifdef OGDF_DEBUG
void TSAPlanarity::printInternalData() const {
	cout << "\nCrossing Matrix:";
	int e_num = m_nonSelfLoops.size();
	for(int i = 1; i < e_num; i++) {
		cout << "\n Edge " << i << " crosses: ";
		for(int j = i+1; j <= e_num; j++)
			if((*m_crossingMatrix)(i,j)) cout << j << " ";
	}
    cout << "\nChanged crossings:";
    ListConstIterator<ChangedCrossing> it;
    for(it = m_crossingChanges.begin(); it.valid(); ++it) {
        ChangedCrossing cc = *(it);
        cout << " (" << cc.edgeNum1 << "," << cc.edgeNum2 << ")" << cc.crossEnergy;
    }

    cout << endl;
}
#endif

}
