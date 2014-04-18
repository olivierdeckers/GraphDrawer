/** \file
 * \brief Implementation of class NodePairEnergy
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


#include <ogdf/TSANodePairEnergy.h>


#ifdef OGDF_SYSTEM_UNIX
#include <climits>
#endif

#ifdef OGDF_SYSTEM_WINDOWS
#include <limits>
#endif

using namespace std;

namespace ogdf {


TSANodePairEnergy::TSANodePairEnergy(const String energyname, GraphAttributes &AG) :
    TSAEnergyFunction(energyname,AG),
    m_shape(m_G),
    m_adjacentOracle(m_G)
{
    node v;
    double lengthSum = 0;
    forall_nodes(v,m_G) { //saving the shapes of the nodes in m_shape
        DPoint center(AG.x(v),AG.y(v));
        lengthSum += AG.width(v);
        lengthSum += AG.height(v);
        m_shape[v] = IntersectionRectangle(center,AG.width(v),AG.height(v));
    }
    m_G.allNodes(m_nonIsolated);
    ListIterator<node> it, itSucc;
    for(it = m_nonIsolated.begin(); it.valid(); it = itSucc) {
        itSucc = it.succ();
        if((*it)->degree() == 0) m_nonIsolated.del(it);
    }
    m_nodeNums = OGDF_NEW NodeArray<int>(m_G,0);
    int n_num = 1;
    for(it = m_nonIsolated.begin(); it.valid(); ++it) {
        (*m_nodeNums)[*it] = n_num;
        n_num++;
    }
    n_num--;
    m_pairEnergy = new Array2D<double> (1,n_num,1,n_num, 0);
    m_candPairEnergy = new Array2D<double> (1, n_num, 1, n_num, -1);
}


void TSANodePairEnergy::computeEnergy()
{
    int n_num = m_nonIsolated.size();
    double energySum = 0.0;
    Array<node> numNodes(1,n_num);

    ListIterator<node> it;
    for(it = m_nonIsolated.begin(); it.valid(); ++it) {
        numNodes[(*m_nodeNums)[*it]] = *it;
    }
    for(int i = 1; i <= n_num-1 ; i++) {
        for(int j = i+1; j <= n_num; j++) {
            double E = computeCoordEnergy(numNodes[i],numNodes[j]);
            (*m_pairEnergy)(i,j) = E;
            energySum += E;
        }
    }
    m_energy = energySum;
}


void TSANodePairEnergy::internalCandidateTaken() {
    int n_num = m_nonIsolated.size();
    double sum = 0;
    for(int i=1; i<=n_num-1; i++) {
        for(int j=i+1; j<=n_num; j++) {
            double E = (*m_candPairEnergy)(i, j);
            if(E != -1) {
                OGDF_ASSERT(E >= 0);
                (*m_pairEnergy)(i, j) = E;
            }
            sum += (*m_pairEnergy)(i,j);
        }
    }

    OGDF_ASSERT(abs(sum-m_energy) < 1e-10);
}


void TSANodePairEnergy::compCandEnergy()
{
    m_candPairEnergy->fill(-1);

    m_candidateEnergy = energy();
    HashConstIterator<node, DPoint> it;
    for(it = m_layoutChanges->begin(); it.valid(); ++it)
    {
        node v = it.key();
        int numv = (*m_nodeNums)[v];

        ListIterator<node> it;
        for(it = m_nonIsolated.begin(); it.valid(); ++ it) {
            int j = (*m_nodeNums)[*it];
            if(numv != j && (numv < j || !m_layoutChanges->member(*it))) {

                m_candidateEnergy -= (*m_pairEnergy)(min(j,numv),max(j,numv));
                double candEnergy = computeCoordEnergy(v,*it);
                m_candidateEnergy += candEnergy;
                (*m_candPairEnergy)(min(j,numv),max(j,numv)) = candEnergy;
            }
        }
    }
}


#ifdef OGDF_DEBUG
void TSANodePairEnergy::printInternalData() const {
    cout << "\nPair energies:";
    for(int i=1; i< m_nonIsolated.size(); i++)
        for(int j=i+1; j <= m_nonIsolated.size(); j++)
            if((*m_pairEnergy)(i,j) != 0.0)
                cout << "\nEnergy(" << i << ',' << j << ") = " << (*m_pairEnergy)(i,j);
}
#endif

} //namespace ogdf
