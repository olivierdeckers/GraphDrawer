///*
// * $Revision: 2565 $
// *
// * last checkin:
// *   $Author: gutwenger $
// *   $Date: 2012-07-07 17:14:54 +0200 (Sa, 07. Jul 2012) $
// ***************************************************************/

///** \file
// * \brief Implementation of class Planarity
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

//#include "PlanarityApprox.h"
//#include <ogdf/graphalg/Dijkstra.h>

//namespace ogdf {

//    PlanarityApprox::~PlanarityApprox()
//    {
//        delete m_closeNeighbours;
//	}


//	// intializes number of edges and allocates memory for  crossingMatrix
//    PlanarityApprox::PlanarityApprox(GraphAttributes &GA, AccelerationStructure *accStruct):
//    TSAPlanarity(GA, accStruct)
//	{
//        m_closeNeighbours = OGDF_NEW EdgeArray<List<edge>*>(m_G);
//        calculateCloseNeighbours();
//    }


//	// computes the energy if the node returned by testNode() is moved
//	// to position testPos().
//    void PlanarityApprox::computeCandidateEnergy(const Hashing<node, DPoint> &layoutChanges)
//	{
//		node v = testNode();
//		m_candidateEnergy = energy();
//		edge e;
//		m_crossingChanges.clear();

//		forall_adj_edges(e,v) if(!e->isSelfLoop()) {
//			// first we compute the two endpoints of e if v is on its new position
//			node s = e->source();
//			node t = e->target();
//			DPoint p1 = testPos();
//			DPoint p2 = (s==v)? currentPos(t) : currentPos(s);
//			int e_num = (*m_edgeNums)[e];
//			edge f;
//            // now we compute the crossings of close neighbours other edges with e
//			ListIterator<edge> it;
//            for(it = (*((*m_closeNeighbours)[e])).begin(); it.valid(); ++it) if(*it != e) { //(*((*m_closeNeighbours)[e]))
//				f = *it;
//				node s2 = f->source();
//				node t2 = f->target();
//				if(s2 != s && s2 != t && t2 != s && t2 != t) {
//					double intersectEnergy = 0;
//                    lowLevelIntersect(p1,p2,currentPos(s2),currentPos(t2), intersectEnergy);
//					int f_num = (*m_edgeNums)[f];
//					double priorIntersectEnergy = (*m_crossingMatrix)(min(e_num,f_num),max(e_num,f_num));
					
//					if(priorIntersectEnergy != intersectEnergy) {
//						m_candidateEnergy -= priorIntersectEnergy; // this intersection was saved
//						m_candidateEnergy += intersectEnergy; // produced a new intersection
//						ChangedCrossing cc;
//						cc.edgeNum1 = min(e_num,f_num);
//						cc.edgeNum2 = max(e_num,f_num);
//						cc.crossEnergy = intersectEnergy;
//						m_crossingChanges.pushBack(cc);
//					}
//				}
//			}
//		}
//    }

//    void PlanarityApprox::calculateCloseNeighbours() {
//        List<edge> edges;
//        m_G.allEdges(edges);
//        cout << edges.size() << endl;
//        List<node> nodes;
//        m_G.allNodes(nodes);

//        Dijkstra<int> dijkstraS, dijkstraT;
//        const EdgeArray<int> weights = EdgeArray<int>(m_G, 1);
//        NodeArray<edge> predecessors = NodeArray<edge>(m_G);
//        NodeArray<int> distancesS = NodeArray<int>(m_G), distancesT = NodeArray<int>(m_G);

//        edge e;
//        forall_edges(e, m_G) {
//            (*m_closeNeighbours)[e] = new List<edge>();
//            dijkstraS.call(m_G, weights, e->source(), predecessors, distancesS);
//            dijkstraT.call(m_G, weights, e->target(), predecessors, distancesT);

//            node n;
//            forall_nodes(n, m_G) {
//                int distance = min(distancesS[n], distancesT[n]);
//                if(distance < NEIGHBOUR_DEPTH) {
//                    edge adjEdge;
//                    forall_adj_edges(adjEdge, n) {
//                        if((*(*m_closeNeighbours)[e]).search(adjEdge) == -1 && e != adjEdge) {
//                            (*(*m_closeNeighbours)[e]).pushBack(adjEdge);
//                        }
//                    }
//                }
//            }
//        }
//    }


//#ifdef OGDF_DEBUG
//void PlanarityApprox::printInternalData() const {
//    TSAPlanarity::printInternalData();
//    cout << "m_closeNeighbours: " << endl;
//    edge edge;
//    forall_edges(edge, m_G) {
//        cout << edge << ": " << (*m_closeNeighbours)[edge]->size() << endl;
//    }
//}
//#endif

//}
