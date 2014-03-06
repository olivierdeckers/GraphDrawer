#include "gtest/gtest.h"

#include "../GraphDrawer/ogdf/tsauniformgrid.h"
#include "../GraphDrawer/ogdf/TSAPlanarity.h"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GmlParser.h>
#include <ogdf/internal/energybased/UniformGrid.h>

class TSAUniformGridTest : public ::testing::Test {
protected:
    TSAUniformGridTest() {

    }

    virtual ~TSAUniformGridTest() {
        delete m_G;
        delete m_GA;
    }

    virtual void SetUp() {
        m_G = new ogdf::Graph();
        m_GA = new ogdf::GraphAttributes(*m_G, ogdf::GraphAttributes::nodeGraphics |
           ogdf:: GraphAttributes::edgeGraphics );

        ogdf::node a = m_G->newNode();
        ogdf::node b = m_G->newNode();
        ogdf::node c = m_G->newNode();
        ogdf::node d = m_G->newNode();
        m_G->newEdge(a, b);
        m_G->newEdge(a, d);
        m_G->newEdge(b,c);
        m_G->newEdge(b,d);
        m_G->newEdge(c,d);
        m_G->newEdge(a, c);

        setNodePos(a, 0, 0);
        setNodePos(b, 0.7, 0.1);
        setNodePos(c, 1, 1);
        setNodePos(d, 0, 1);
    }

    virtual void TearDown() {

    }

    ogdf::Graph* m_G;
    ogdf::GraphAttributes* m_GA;

    void setNodePos(ogdf::node n, double x, double y) {
        m_GA->x(n) = x;
        m_GA->y(n) = y;
    }
};

TEST_F(TSAUniformGridTest, InitialGridCorrectness) {
    ogdf::TSAUniformGrid grid = ogdf::TSAUniformGrid(*m_GA);

    ogdf::TSAPlanarity *planarity = new ogdf::TSAPlanarity(*m_GA);
    planarity->computeEnergy();

    ogdf::UniformGrid originalGrid = ogdf::UniformGrid(*m_GA);
    EXPECT_EQ(1, planarity->energy()) << "Planarity correctly detects one crossing";
    EXPECT_EQ(1, grid.numberOfCrossings()) << "Grid correctly detects one crossing";
    EXPECT_EQ(1, originalGrid.numberOfCrossings()) << "Original Grid correctly detects one crossing";

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
