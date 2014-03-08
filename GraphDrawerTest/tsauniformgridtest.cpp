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
        b = m_G->newNode();
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
    ogdf::node b;

    void setNodePos(ogdf::node n, double x, double y) {
        m_GA->x(n) = x;
        m_GA->y(n) = y;
    }
};

TEST_F(TSAUniformGridTest, GridCorrectness) {
    ogdf::TSAUniformGrid* grid = new ogdf::TSAUniformGrid(*m_GA);
    EXPECT_EQ(1, grid->numberOfCrossings()) << "Grid correctly detects one crossing";

    const ogdf::DPoint newPos = ogdf::DPoint(0.1, 0.7);
    EXPECT_EQ(0, grid->calculateCandidateEnergy(b, newPos)) << "Grid calculates correct candidate energy";

    EXPECT_EQ(1, grid->numberOfCrossings()) << "Grid maintains energy while candidate is not accepted";

    EXPECT_EQ(false, grid->newGridNecessary(b, newPos)) << "No new grid is necessary";

    grid->updateNodePosition(b, newPos);
    EXPECT_EQ(0, grid->numberOfCrossings()) << "Grid correctly updates energy when candidate is accepted";

    delete grid;
}

TEST_F(TSAUniformGridTest, GridCorrectnessNewGridNecessary) {
    ogdf::TSAUniformGrid* grid = new ogdf::TSAUniformGrid(*m_GA);
    EXPECT_EQ(1, grid->numberOfCrossings()) << "Grid correctly detects one crossing";

    const ogdf::DPoint newPos = ogdf::DPoint(2,1.5);
    EXPECT_EQ(0, grid->calculateCandidateEnergy(b, newPos)) << "Grid calculates correct candidate energy";

    EXPECT_EQ(true, grid->newGridNecessary(b, newPos)) << "A new grid is necessary";

    EXPECT_EQ(1, grid->numberOfCrossings()) << "Grid maintains energy while candidate is not accepted";

    delete grid;
    grid = new ogdf::TSAUniformGrid(*m_GA, b, newPos);
    EXPECT_EQ(0, grid->numberOfCrossings()) << "Grid correctly updates energy when candidate is accepted";

    delete grid;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
