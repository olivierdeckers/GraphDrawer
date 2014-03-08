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


#include "tsaplanaritygrid.h"

namespace ogdf {

    TSAPlanarityGrid::~TSAPlanarityGrid()
    {
        delete m_currentGrid;
    }


    // intialize m_currentLayout and m_candidateLayout
    TSAPlanarityGrid::TSAPlanarityGrid(GraphAttributes &AG):
    EnergyFunction("PlanarityGrid",AG),
      m_layout(AG),
      candidateNode(NULL),
      candidatePos()
    {
        m_currentGrid = new TSAUniformGrid(AG);
    }


    // computes energy of layout, stores it and sets the crossingMatrix
    void TSAPlanarityGrid::computeEnergy()
    {
        m_energy = m_currentGrid->numberOfCrossings();
    }


    // computes the energy if the node returned by testNode() is moved
    // to position testPos().
    void TSAPlanarityGrid::compCandEnergy()
    {
        candidateNode = testNode();
        candidatePos = testPos();

        m_candidateEnergy = m_currentGrid->calculateCandidateEnergy(candidateNode, candidatePos);
    }


    // this functions sets the currentGrid to the candidateGrid
    void TSAPlanarityGrid::internalCandidateTaken() {
        if(m_currentGrid->newGridNecessary(candidateNode, candidatePos)) {
            m_currentGrid = new TSAUniformGrid(m_layout, candidateNode, candidatePos);
        }
        else {
            m_currentGrid->updateNodePosition(candidateNode, candidatePos);
        }
        m_energy = m_currentGrid->numberOfCrossings();
    }


#ifdef OGDF_DEBUG
void TSAPlanarityGrid::printInternalData() const {
    cout << "\nCurrent grid: " << *m_currentGrid;
}
#endif

}
