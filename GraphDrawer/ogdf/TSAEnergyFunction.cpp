/*
 * $Revision: 2552 $
 *
 * last checkin:
 *   $Author: gutwenger $
 *   $Date: 2012-07-05 16:45:20 +0200 (Do, 05. Jul 2012) $
 ***************************************************************/

/** \file
 * \brief Implements class EnergyFunction.
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


#include <ogdf/TSAEnergyFunction.h>

namespace ogdf {

TSAEnergyFunction::TSAEnergyFunction(const String &funcname, GraphAttributes &AG) :
    m_G(AG.constGraph()),
    m_name(funcname),
    m_candidateEnergy(0),
    m_energy(0),
    m_AG(AG),
    m_layoutChanges(NULL) { }


void TSAEnergyFunction::candidateTaken()
{
    m_energy=m_candidateEnergy;
    m_candidateEnergy = 0.0;
    internalCandidateTaken();
}

double TSAEnergyFunction::computeCandidateEnergy(Hashing<node, DPoint> &layoutChanges)
{
    m_layoutChanges = &layoutChanges;
    compCandEnergy();
    OGDF_ASSERT(m_candidateEnergy >= 0.0);
    return m_candidateEnergy;
}

#ifdef OGDF_DEBUG
void TSAEnergyFunction::printStatus() const{
    cout << "\nEnergy function name: " << m_name;
    cout << "\nCurrent energy: " << m_energy;
    node v;
    cout << "\nPosition of nodes in current solution:";
    NodeArray<int> num(m_G);
    int count = 1;
    forall_nodes(v,m_G) num[v] = count ++;
    forall_nodes(v,m_G) {
        cout << "\nNode: " << num[v] << " Position: " << currentPos(v);
    }
    cout << "\nCandidate energy: " << m_candidateEnergy;
    printInternalData();
}
#endif

} //namespace
