#ifndef NEIGHBOURHOODSTRUCTURE_H
#define NEIGHBOURHOODSTRUCTURE_H

#include <ogdf/basic/NodeArray.h>
#include <ogdf/basic/geometry.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Graph.h>

namespace ogdf {

class NeighbourhoodStructure
{
public:
    NeighbourhoodStructure(GraphAttributes &GA, String name) : m_GA(GA), m_G(GA.constGraph()), m_name(name) {}

    virtual ~NeighbourhoodStructure() {}
    virtual void generateNeighbouringLayout(double temp, Hashing<node, DPoint> &result) = 0;

    String getName() { return m_name; }

protected:
    GraphAttributes &m_GA;
    const Graph &m_G;

    double diskRadius(double temp)
    {
        return min(1.0, 10*temp);
    }
private:
    const String m_name;
};

}

#endif // NEIGHBOURHOODSTRUCTURE_H
