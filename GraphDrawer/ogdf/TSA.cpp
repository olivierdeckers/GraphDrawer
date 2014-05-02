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

#include <ogdf/TSAEdgeLength.h>
#include <ogdf/NodeEdgeDistance.h>

//TODO: in addition to the layout size, node sizes should be used in
//the initial radius computation in case of "all central" layouts with
//huge nodes
//the combinations for parameters should be checked: its only useful
//to have a slow shrinking if you have enough time to shrink down to
//small radius

namespace ogdf {

    const double TSA::m_startingTemp = 1e-1;
	//const int TSA::m_iterationMultiplier = 25;  //best//30;ori
	const double TSA::m_defaultEndTemperature = 1e-5;

	//initializes internal data and the random number generator
    TSA::TSA():
        m_temperature(m_startingTemp),
        m_energy(0.0),
        m_quality(1.0),
        m_endTemperature(m_defaultEndTemperature),
        m_penaltyParameter(0.01),
        m_rewardParameter(0.1)
	{
		
	}


	//allow resetting in between subsequent calls
	void TSA::initParameters()
    {
        m_energy = 0.0;

        ListIterator<double> it = m_neighbourhoodSelectionChances.begin();
        double sum = 0;
        for(;it.valid(); it++)
        {
            sum += *it;
        }
        it = m_neighbourhoodSelectionChances.begin();
        for(; it.valid(); it++)
        {
            *it /= sum;
        }

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

    void TSA::addNeighbourhoodStructure(NeighbourhoodStructure *ns, double weight)
    {
        m_neighbourhoodStructures.pushBack(ns);
        m_neighbourhoodSelectionChances.pushBack(weight);
    }

    int TSA::chooseNeighbourhood(int nbIterations) const
    {
        /*int r = randomNumber(0, 99);
        if(r < 20)
            return 1;
        return 0;/**/

        //if(nbIterations > 3000)
        //    return 0;

        if(m_neighbourhoodStructures.size() == 1)
            return 0;

        double r = randNum();
        double acc = 0;
        int neighbourhood = 0;
        ListConstIterator<double> it = m_neighbourhoodSelectionChances.begin();
        for(; it.valid(); it++)
        {
            acc += *it;
            OGDF_ASSERT(acc >= -1e-6 && acc <= 1 + 1e-6);
            if (r <= acc) {
                return neighbourhood;
            }
            neighbourhood++;
        }

        throw;/**/
    }

    void TSA::updateNeighbourhoodChances(double costDiff, int neighbourhoodUsed)
    {
        int reward = (costDiff < 0) ? 1 : 0;
        int nbNeighbours = m_neighbourhoodStructures.size();
        ListIterator<double> chanceIt = m_neighbourhoodSelectionChances.begin();
        for(int i=0; chanceIt.valid(); chanceIt++, i++)
        {
            if(i == neighbourhoodUsed)
            {
                *chanceIt += m_rewardParameter * reward * (1-*chanceIt)
                        - m_penaltyParameter * (1-reward) * *chanceIt;
            }
            else
            {
                *chanceIt += -m_rewardParameter * reward * *chanceIt
                        + m_penaltyParameter * (1-reward) * ((1.0 / (nbNeighbours - 1)) - *chanceIt);
            }
        }
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
        cout << "initial energy components: " << endl;
        for(it = m_energyFunctions.begin(); it.valid() && it2.valid(); it=it.succ(), it2 = it2.succ()) {
			m_energy += (*it)->energy() * (*it2);
            cout << (*it)->getName() << ": " << (*it)->energy() << endl;
        }
        cout << "Initial energy" << m_energy << endl;
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

    double TSA::compCandEnergy(Hashing<node,DPoint> &layoutChanges)
    {
        ListIterator<TSAEnergyFunction*> it;
        ListIterator<double> it2 = m_weightsOfEnergyFunctions.begin();
        double newEnergy = 0.0;
        for(it = m_energyFunctions.begin(); it.valid(); it = it.succ()) {
            newEnergy += (*it)->computeCandidateEnergy(layoutChanges) * (*it2);
            it2 = it2.succ();
        }
        OGDF_ASSERT(newEnergy >= 0.0);
        return newEnergy;
    }

    void TSA::acceptChanges(GraphAttributes &AG, Hashing<node,DPoint> &layoutChanges, AccelerationStructure *grid)
    {
        ListIterator<TSAEnergyFunction*> energyFunctionIt;
        for(energyFunctionIt = m_energyFunctions.begin(); energyFunctionIt.valid(); energyFunctionIt = energyFunctionIt.succ())
            (*energyFunctionIt)->candidateTaken();

        HashConstIterator<node, DPoint> it;
        for(it = layoutChanges.begin(); it.valid(); ++it)
        {
            AG.x(it.key()) = it.info().m_x;
            AG.y(it.key()) = it.info().m_y;
            grid->updateNodePosition(it.key(), it.info());
        }

        m_energy = m_candEnergy;
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

#ifdef GRAPHDRAWER
        ListIterator<NeighbourhoodStructure*> it = m_neighbourhoodStructures.begin();
        int i=0;
        for(; it.valid(); it++, i++)
        {
            worker->neighbourhoodLegendEntry(i, QString((*it)->getName().cstr()));
        }
#endif

		time_t start = time(NULL);

		OGDF_ASSERT(!m_energyFunctions.empty());

        if(AG.constGraph().numberOfEdges() > 0) { //else only isolated nodes
			computeInitialEnergy();
			
            m_totalCostDiff = 0;
            m_totalEntropyDiff = 0;
            double costDiff;
            double entropyDiffSinceLastUpdate = 0;
            double costDiffSinceLastUpdate = 0;
			int i = 0;
            double costDiffPrediction = 0;
            double alpha = 0.9;
            bool updateTemp = true;
            while((m_temperature > m_endTemperature || i < 20) && i < 1e5) {
                Hashing<node, DPoint> layoutChanges;
                int neighbourhood = chooseNeighbourhood(i);

                try {
                    (*(m_neighbourhoodStructures.get(neighbourhood)))->generateNeighbouringLayout(m_temperature, layoutChanges);
                }
                catch(const char*) { //fall back to randomMove when other structure is not applicable
                    neighbourhood = 0;
                    m_neighbourhoodStructures.front()->generateNeighbouringLayout(m_temperature, layoutChanges);
                }
                //cout << "neighbourhood used: " << neighbourhood << ", iteration: " << i << endl;

				//compute candidate energy and decide if new layout is chosen
                m_candEnergy = compCandEnergy(layoutChanges);

                costDiff = m_candEnergy - m_energy;

                updateNeighbourhoodChances(costDiff, neighbourhood);

				//this tests if the new layout is accepted. If this is the case,
				//all energy functions are informed that the new layout is accepted
                if(testEnergyValue(m_candEnergy)) {
                    acceptChanges(AG, layoutChanges, grid);

                    if(costDiffSinceLastUpdate == 0)
                        costDiffSinceLastUpdate = costDiff;
                    else
                        costDiffSinceLastUpdate = min(costDiffSinceLastUpdate, costDiff);
                }

                if(costDiff > 0) {
                    double currentEntropyDiff = costDiff / m_temperature;
                    entropyDiffSinceLastUpdate = max(entropyDiffSinceLastUpdate, currentEntropyDiff);
				}
				
                if(updateTemp)
                {
                    m_totalCostDiff += costDiffSinceLastUpdate;
                    m_totalEntropyDiff -= entropyDiffSinceLastUpdate;
                    costDiffSinceLastUpdate = 0;
                    entropyDiffSinceLastUpdate = 0;
                }

                if(m_totalCostDiff >= 0 || m_totalEntropyDiff == 0) {
					m_temperature = m_startingTemp;
				}
                else if(updateTemp) {
                    m_temperature = m_quality * (m_totalCostDiff / m_totalEntropyDiff);
				}


                updateTemp = i%1 == 0;
#ifdef GRAPHDRAWER
                if(worker != NULL) {
                    worker->energyInfo(m_energy, m_temperature);

                    ListIterator<double> it = m_neighbourhoodSelectionChances.begin();
                    for(int j = 0; it.valid(); it++, j++)
                    {
                        worker->neighbourhoodSelectionChance(j, *it);
                    }
                }
#endif
				/*cout << "temperature: " << m_temperature << endl;
				cout << "diskradius: " << m_diskRadius << endl;
				cout << "energy: " << m_energy << endl;
				cout << "iteration: " << i << endl;*/

                if(costDiff <= 0)
                    costDiffPrediction = alpha * costDiff + (1-alpha) * costDiffPrediction;

                if(costDiffPrediction > -1e-4 && i > 1000)
                    break;

                i ++;
			}

            postProcessingPhase(AG, grid);
		}
        //TODO: consider zero degree vertices
		//if there are zero degree vertices, they are placed using placeIsolatedNodes
        //if(m_nonIsolatedNodes.size() != G.numberOfNodes())
        //	placeIsolatedNodes(AG);
		cout << "Time: " << (time(NULL) - start) << endl;
        cout << "iterations: " << std::fixed << i << endl;
    }

    void TSA::postProcessingPhase(GraphAttributes &AG, AccelerationStructure *grid)
    {
        cout << "PostProcessing... Current energy: " << m_energy << endl;
        TSAEdgeLength edgeLength(AG);
        NodeEdgeDistance nodeEdgeDistance(AG, grid);
        addEnergyFunction(&edgeLength, 100);
        addEnergyFunction(&nodeEdgeDistance, 100);
        computeInitialEnergy();
        cout << "Energy with extra energy functions: " << m_energy << endl;

        DRect rect = AG.boundingBox();
        double size = max(rect.height(), rect.width());
        double diskRadius = 0.0001*size;

        node n;
        forall_nodes(n, AG.constGraph())
        {
            for(int i=0; i<10; i++) {
                Hashing<node, DPoint> layoutChanges;
                double oldx = AG.x(n);
                double oldy = AG.y(n);
                double randomAngle = randNum() * 2.0 * Math::pi;
                DPoint newPos;
                newPos.m_y = oldy + sin(randomAngle) * diskRadius * randNum();
                newPos.m_x = oldx + cos(randomAngle) * diskRadius * randNum();
                layoutChanges.insert(n, newPos);

                m_candEnergy = compCandEnergy(layoutChanges);
                if(m_candEnergy < m_energy) {
                    acceptChanges(AG, layoutChanges, grid);
                    i = 0;
                }
            }
        }

        cout << "Finished postprocessing. New Energy: " << m_energy << endl;
        cout << "energy: " << std::fixed << m_energy << endl;
        cout << "Improvement: " << std::fixed << m_totalCostDiff << endl;
        cout << "energy components: " << endl;
        ListIterator<TSAEnergyFunction*> it = m_energyFunctions.begin();
        for(;it.valid(); it++) {
            cout << (*it)->getName() << ": " << (*it)->energy() << endl;
        }
    }
} //namespace
