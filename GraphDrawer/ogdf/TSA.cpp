/** \file
 * \brief  Iimplementation of class TSA
 *
 * This class realizes the TSA Algorithm for
 * automtatic graph drawing. It minimizes the energy
 * of the drawing using thermodynamic simulated annealing. This file
 * contains the main simulated annealing algorithm and
 * the fnction for computing the next candidate layout
 * that should be considered.
 *
 * \author Olivier Deckers
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

#include "TSA.h"
#include <ogdf/basic/Math.h>
#include <time.h>

//TODO: in addition to the layout size, node sizes should be used in
//the initial radius computation in case of "all central" layouts with
//huge nodes
//the combinations for parameters should be checked: its only useful
//to have a slow shrinking if you have enough time to shrink down to
//small radius

namespace ogdf {

	const double TSA::m_startingTemp = 1e-2;
	//const int TSA::m_iterationMultiplier = 25;  //best//30;ori
	const double TSA::m_defaultEndTemperature = 1e-5;

	//initializes internal data and the random number generator
	TSA::TSA():
	m_temperature(m_startingTemp),
	m_energy(0.0),
	m_endTemperature(m_defaultEndTemperature),
	m_quality(1.0)
	//m_numberOfIterations(0)
	{
		
	}


	//allow resetting in between subsequent calls
	void TSA::initParameters()
    {
        m_energy = 0.0;

        unsigned int t = (unsigned) time(NULL);
		cout << "seed: " << t << endl;
		srand(t);
		//srand((unsigned int) 1385114936);
	}

	void TSA::setQuality(double quality) 
	{
		OGDF_ASSERT(quality >= 0);
		m_quality = quality;
	}

	void TSA::setStartTemperature(double startTemp)
	{
		OGDF_ASSERT(startTemp >= 0);
		m_temperature=startTemp;
	}

	/*void TSA::setNumberOfIterations(int steps)
	{
		OGDF_ASSERT(steps >= 0);
		m_numberOfIterations = steps;
	}*/

	//whenever an energy function is added, the initial energy of the new function
	//is computed and added to the initial energy of the layout
    void TSA::addEnergyFunction(TSAEnergyFunction *F, double weight)
	{
		m_energyFunctions.pushBack(F);
		OGDF_ASSERT(weight >= 0);
		m_weightsOfEnergyFunctions.pushBack(weight);
		F->computeEnergy();
		m_energy += F->energy();
	}

    void TSA::addNeighbourhoodStructure(NeighbourhoodStructure *ns)
    {
        m_neighbourhoodStructures.pushBack(ns);
        m_neighbourhoodImprovements.pushBack(0.0);
    }

    int TSA::chooseNeighbourhood() const
    {
        if(m_totalCostDiff == 0) {
            return randomNumber(0, m_neighbourhoodStructures.size() - 1);
        }

        double r = randNum();
        double acc = 0;
        int neighbourhood = 0;
        ListConstIterator<double> it;

        for(it = m_neighbourhoodImprovements.begin(); it.valid(); it++) {
            cout << "neighbourhood " << neighbourhood << ": " << *it << endl;
            neighbourhood ++;
        }
        neighbourhood = 0;

        for(it = m_neighbourhoodImprovements.begin(); it.valid(); it++)
        {
            acc += (*it) / m_totalCostDiff;
            OGDF_ASSERT(acc >= 0 && acc <= 1);
            if (r <= acc) {
                return neighbourhood;
            }
            neighbourhood++;
        }

        throw;
    }

    List<String> TSA::returnEnergyFunctionNames()
	{
        List<String> names;
        ListIterator<TSAEnergyFunction*> it;
		for(it = m_energyFunctions.begin(); it.valid(); it = it.succ())
			names.pushBack((*it)->getName());
		return names;
	}

	List<double> TSA::returnEnergyFunctionWeights()
	{
		List<double> weights;
		ListIterator<double> it;
		for(it = m_weightsOfEnergyFunctions.begin(); it.valid(); it = it.succ())
			weights.pushBack(*it);
		return weights;
	}

	//newVal is the energy value of a candidate layout. It is accepted if it is lower
	//than the previous energy of the layout or if m_fineTune is not tpFine and
	//the difference to the old energy divided by the temperature is smaller than a
	//random number between zero and one
	bool TSA::testEnergyValue(double newVal)
	{
		bool accepted = true;
		if(newVal > m_energy) {
			accepted = false;

			double testval = exp((m_energy-newVal)/ m_temperature);
			double compareVal = randNum(); // number between 0 and 1

			if(compareVal < testval)
				accepted = true;

		}
		return accepted;
	}

	//divides number returned by rand by RAND_MAX to get number between zero and one
	inline double TSA::randNum() const
	{
		double val = rand();
		val /= RAND_MAX;
		return val;
    }

	//steps through all energy functions and adds the initial energy computed by each
	//function for the start layout
	void TSA::computeInitialEnergy()
	{
		OGDF_ASSERT(!m_energyFunctions.empty());
        ListIterator<TSAEnergyFunction*> it;
		ListIterator<double> it2;
		it2 = m_weightsOfEnergyFunctions.begin();
		for(it = m_energyFunctions.begin(); it.valid() && it2.valid(); it=it.succ(), it2 = it2.succ())
			m_energy += (*it)->energy() * (*it2);
	}

	//the vertices with degree zero are placed below all other vertices on a horizontal
	// line centered with repect to the rest of the drawing
	void TSA::placeIsolatedNodes(GraphAttributes &AG) const {
		double minX = 0.0;
		double minY = 0.0;
		double maxX = 0.0;
		double maxY = 0.0;

        List<node> nonIsolatedNodes;
        AG.constGraph().allNodes(nonIsolatedNodes);

        if(!nonIsolatedNodes.empty()) {
			//compute a rectangle that includes all non-isolated vertices
            node v = nonIsolatedNodes.front();
			minX = AG.x(v);
			minY = AG.y(v);
			maxX = minX;
			maxY = minY;
			ListConstIterator<node> it;
            for(it = nonIsolatedNodes.begin(); it.valid(); ++it) {
				v = *it;
                if(v->degree() != 0)
                {
                    double xVal = AG.x(v);
                    double yVal = AG.y(v);
                    double halfHeight = AG.height(v) / 2.0;
                    double halfWidth = AG.width(v) / 2.0;
                    if(xVal - halfWidth < minX) minX = xVal - halfWidth;
                    if(xVal + halfWidth > maxX) maxX = xVal + halfWidth;
                    if(yVal - halfHeight < minY) minY = yVal - halfHeight;
                    if(yVal + halfHeight > maxY) maxY = yVal + halfHeight;
                }
			}
		}

		// compute the width and height of the largest isolated node
		List<node> isolated;
		node v;
		const Graph &G = AG.constGraph();
		double maxWidth = 0;
		double maxHeight = 0;
		forall_nodes(v,G) if(v->degree() == 0) {
			isolated.pushBack(v);
			if(AG.height(v) > maxHeight) maxHeight = AG.height(v);
			if(AG.width(v) > maxWidth) maxWidth = AG.width(v);
		}
		// The nodes are placed on a line in the middle under the non isolated vertices.
		// Each node gets a box sized 2 maxWidth.
		double boxWidth = 2.0*maxWidth;
		double commonYCoord = minY-(1.5*maxHeight);
		double XCenterOfDrawing = minX + ((maxX-minX)/2.0);
		double startXCoord = XCenterOfDrawing - 0.5*(isolated.size()*boxWidth);
		ListIterator<node> it;
		double xcoord = startXCoord;
		for(it = isolated.begin(); it.valid(); ++it) {
			v = *it;
			AG.x(v) = xcoord;
			AG.y(v) = commonYCoord;
			xcoord += boxWidth;
		}
	}

	//this is the main optimization routine with the loop that lowers the temperature
	//and the disk radius geometrically until the temperature is zero. For each
	//temperature, a certain number of new positions for a random vertex are tried
#ifdef GRAPHDRAWER
    void TSA::call(GraphAttributes &AG, AccelerationStructure *grid, LayoutWorker * worker)
#else
    void TSA::call(GraphAttributes &AG, AccelerationStructure *grid)
#endif
	{
		initParameters();

		time_t start = time(NULL);

		OGDF_ASSERT(!m_energyFunctions.empty());

        if(AG.constGraph().numberOfEdges() > 0) { //else only isolated nodes
			computeInitialEnergy();
			
            m_totalCostDiff = 0;
            m_totalEntropyDiff = 0;
			double costDiff;
			int i = 0;
			int iterationsSinceLastChange = 0;
			//this is the main optimization loop
            while((m_temperature > m_endTemperature || i < 20) && i < 1e5 /*&& m_diskRadius >= 1*/) {
                Hashing<node, DPoint> layoutChanges;
                int neighbourhood = (i > 1000) ? chooseNeighbourhood() : randomNumber(0, m_neighbourhoodStructures.size() - 1);

                try {
                    (*(m_neighbourhoodStructures.get(neighbourhood)))->generateNeighbouringLayout(m_temperature, layoutChanges);
                }
                catch(const char*) { //fall back to randomMove when other structure is not applicable
                    neighbourhood = 0;
                    m_neighbourhoodStructures.front()->generateNeighbouringLayout(m_temperature, layoutChanges);
                }
                cout << "neighbourhood used: " << neighbourhood << endl;

				//compute candidate energy and decide if new layout is chosen
                ListIterator<TSAEnergyFunction*> it;
				ListIterator<double> it2 = m_weightsOfEnergyFunctions.begin();
				double newEnergy = 0.0;
				for(it = m_energyFunctions.begin(); it.valid(); it = it.succ()) {
                    newEnergy += (*it)->computeCandidateEnergy(layoutChanges) * (*it2);
					it2 = it2.succ();
				}
				OGDF_ASSERT(newEnergy >= 0.0);

				costDiff = newEnergy - m_energy;

				//this tests if the new layout is accepted. If this is the case,
				//all energy functions are informed that the new layout is accepted
				if(testEnergyValue(newEnergy)) {
                    m_totalCostDiff += costDiff;
                    (*(m_neighbourhoodImprovements.get(neighbourhood))) += costDiff;

					for(it = m_energyFunctions.begin(); it.valid(); it = it.succ())
						(*it)->candidateTaken();

                    HashConstIterator<node, DPoint> it;
                    for(it = layoutChanges.begin(); it.valid(); ++it)
                    {
                        AG.x(it.key()) = it.info().m_x;
                        AG.y(it.key()) = it.info().m_y;
                        grid->updateNodePosition(it.key(), it.info());
                    }

                    m_energy = newEnergy;

                    if(costDiff < -1e-4)
                        iterationsSinceLastChange = 0;
                }

				if(costDiff > 0) {
                    m_totalEntropyDiff -= costDiff / m_temperature;
				}
				
                if(m_totalCostDiff >= 0 || m_totalEntropyDiff == 0) {
					m_temperature = m_startingTemp;
				}
				else {
                    m_temperature = m_quality * (m_totalCostDiff / m_totalEntropyDiff);
				}

#ifdef GRAPHDRAWER
                if(worker != NULL)
                    worker->energyInfo(m_energy, m_temperature);
#endif
				/*cout << "temperature: " << m_temperature << endl;
				cout << "diskradius: " << m_diskRadius << endl;
				cout << "energy: " << m_energy << endl;
				cout << "iteration: " << i << endl;*/
                iterationsSinceLastChange ++;

                if(iterationsSinceLastChange > 100)
					break;

                i ++;
			}
			
			cout << "energy: " << std::fixed << m_energy << endl;
			cout << "iterations: " << std::fixed << i << endl;
		}
        //TODO: consider zero degree vertices
		//if there are zero degree vertices, they are placed using placeIsolatedNodes
        //if(m_nonIsolatedNodes.size() != G.numberOfNodes())
        //	placeIsolatedNodes(AG);
		cout << "Time: " << (time(NULL) - start) << endl;
    }
} //namespace
