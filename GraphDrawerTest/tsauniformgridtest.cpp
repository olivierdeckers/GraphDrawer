#include "gtest/gtest.h"

#include <ogdf/basic/basic.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/Hashing.h>
#include <ogdf/fileformats/GmlParser.h>
#include <ogdf/internal/energybased/UniformGrid.h>
#include <ogdf/fileformats/GmlParser.h>

#include <ogdf/TSAPlanarity.h>
#include <ogdf/TSAAngularResolution.h>
#include <ogdf/TSAUniformGrid.h>
#include <ogdf/TSANoAcceleration.h>

#define randf() (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))

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

        a = m_G->newNode();
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
        setNodePos(b, 1, 0);
        setNodePos(c, 1, 1);
        setNodePos(d, 0, 1);
    }

    virtual void TearDown() {

    }

    ogdf::Graph* m_G;
    ogdf::GraphAttributes* m_GA;
    ogdf::node a,b;
    ogdf::Hashing<ogdf::node, ogdf::DPoint> layoutChanges;

    void setNodePos(ogdf::node n, double x, double y) {
        m_GA->x(n) = x;
        m_GA->y(n) = y;
    }

    double computeCandidateEnergy(ogdf::node n, ogdf::DPoint &newPos, ogdf::TSAEnergyFunction *planarity) {
        layoutChanges.clear();
        layoutChanges.insert(n, newPos);

        return planarity->computeCandidateEnergy(layoutChanges);
    }
};

TEST_F(TSAUniformGridTest, GridCorrectness) {
    ogdf::AccelerationStructure* grid = new ogdf::TSAUniformGrid(*m_GA);
    ogdf::TSAPlanarity* planarity = new ogdf::TSAPlanarity(*m_GA, grid);
    planarity->computeEnergy();

    EXPECT_EQ(1, planarity->energy()) << "Grid correctly detects one crossing";

    ogdf::DPoint newPos = ogdf::DPoint(0.1, 0.7);
    double candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(0, candEnergy) << "Grid calculates correct candidate energy";

    EXPECT_EQ(1, planarity->energy()) << "Grid maintains energy while candidate is not accepted";

    //EXPECT_EQ(false, grid->newGridNecessary(b, newPos)) << "No new grid is necessary";

    planarity->candidateTaken();
    EXPECT_EQ(0, planarity->energy()) << "Grid correctly updates energy when candidate is accepted";

    delete grid;
    delete planarity;
}

TEST_F(TSAUniformGridTest, GridCorrectnessMultipleCandidatesEvaluated) {
    ogdf::AccelerationStructure* grid = new ogdf::TSAUniformGrid(*m_GA);
    ogdf::TSAPlanarity* planarity = new ogdf::TSAPlanarity(*m_GA, grid);
    planarity->computeEnergy();

    EXPECT_EQ(1, planarity->energy()) << "Grid correctly detects one crossing";

    ogdf::DPoint newPos = ogdf::DPoint(0.1, 0.7);
    double candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(0, candEnergy) << "Grid calculates correct candidate energy";

    EXPECT_EQ(1, planarity->energy()) << "Grid maintains energy while candidate is not accepted";

    newPos = ogdf::DPoint(0.6, 0.4);
    candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(1, candEnergy) << "Grid calculates correct candidate energy";

    planarity->candidateTaken();
    EXPECT_EQ(1, planarity->energy()) << "Grid correctly updates energy when candidate is accepted";

    newPos = ogdf::DPoint(0.6, 0.4);
    candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(1, candEnergy) << "Grid calculates correct candidate energy";

    newPos = ogdf::DPoint(0.1, 0.7);
    candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(0, candEnergy) << "Grid calculates correct candidate energy";

    planarity->candidateTaken();

    EXPECT_EQ(0, planarity->energy()) << "Grid maintains energy while candidate is not accepted";

    delete grid;
    delete planarity;
}

TEST_F(TSAUniformGridTest, GridCorrectnessNewGridNecessary) {
    ogdf::AccelerationStructure* grid = new ogdf::TSAUniformGrid(*m_GA);
    ogdf::TSAPlanarity* planarity = new ogdf::TSAPlanarity(*m_GA, grid);
    planarity->computeEnergy();

    EXPECT_EQ(1, planarity->energy()) << "Grid correctly detects one crossing";

    ogdf::DPoint newPos = ogdf::DPoint(2,1.5);
    double candEnergy = computeCandidateEnergy(b, newPos, planarity);
    EXPECT_EQ(0, candEnergy) << "Grid calculates correct candidate energy";

    //EXPECT_EQ(true, grid->newGridNecessary(b, newPos)) << "A new grid is necessary";

    EXPECT_EQ(1, planarity->energy()) << "Grid maintains energy while candidate is not accepted";

    planarity->candidateTaken();
    EXPECT_EQ(0, planarity->energy());
    delete grid;
    delete planarity;
}

TEST_F(TSAUniformGridTest, GridCorrectnessPlanarityGrid) {
    unsigned int seed = std::time(NULL);
    srand(seed);
    cout << "seed: " << seed << endl;

    ogdf::Graph* G = new ogdf::Graph();
    ogdf::GraphAttributes* GA = new ogdf::GraphAttributes(*G, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);

    /*ogdf::GmlParser parser("../GraphDrawer/GraphDrawer/graphs/20.gml");
    parser.read(*G);
    int numNodes = 20;
    ogdf::node nodes [numNodes];
    ogdf::node n; int i=0;
    forall_nodes(n, *G) {
        nodes[i] = n;
        GA->x(n) = randf();
        GA->y(n) = randf();
        i++;
    }*/

    int numNodes = 20;
    ogdf::node nodes [numNodes];
    for(int i=0; i<numNodes; i++) {
        nodes[i] = G->newNode();
        GA->x(nodes[i]) = randf();
        GA->y(nodes[i]) = randf();
    }
    int nbEdges = 50;
    for(int i=0; i<nbEdges; i++) {
        int a = rand() % numNodes;
        int b = rand() % numNodes;
        G->newEdge(nodes[a], nodes[b]);
    }



    ogdf::AccelerationStructure* grid = new ogdf::TSAUniformGrid(*GA);
    ogdf::AccelerationStructure* none = new ogdf::TSANoAcceleration(*GA);
    ogdf::TSAPlanarity* planarityGrid = new ogdf::TSAPlanarity(*GA, grid);
    ogdf::TSAPlanarity* planarity = new ogdf::TSAPlanarity(*GA, none);
    planarityGrid->computeEnergy();
    planarity->computeEnergy();

    EXPECT_EQ(planarity->energy(), planarityGrid->energy()) << "Grid computes same initial energy";

    for(int i=0; i<100; i++) {
        ogdf::Hashing<ogdf::node, ogdf::DPoint> layoutChanges;
        int numMoved = ogdf::randomNumber(1, floor(numNodes/5));
        for(int i=0; i<numMoved; i++) {
            int n = rand() % numNodes;
            ogdf::DPoint newPos = ogdf::DPoint(randf(), randf());
            layoutChanges.insert(nodes[n], newPos);
        }

        double planCandEnergy = planarity->computeCandidateEnergy(layoutChanges);
        double gridCandEnergy = planarityGrid->computeCandidateEnergy(layoutChanges);
        EXPECT_EQ(planCandEnergy, gridCandEnergy) << "Grid computes same candidate energy";

        if(randf() < 0.5) {
            planarity->candidateTaken();
            planarityGrid->candidateTaken();

            ogdf::HashConstIterator<ogdf::node, ogdf::DPoint> it;
            for(it = layoutChanges.begin(); it.valid(); ++it)
            {
                grid->updateNodePosition(it.key(), it.info());
                GA->x(it.key()) = it.info().m_x;
                GA->y(it.key()) = it.info().m_y;
            }
        }

        EXPECT_EQ(planarity->energy(), planarityGrid->energy());
    }

    delete planarityGrid;
    delete planarity;
    delete grid;
    delete none;
    delete G;
    delete GA;
}

TEST_F(TSAUniformGridTest, GridCorrectnessMoveMultipleNodes) {
    ogdf::AccelerationStructure* grid = new ogdf::TSAUniformGrid(*m_GA);
    ogdf::TSAPlanarity* planarity = new ogdf::TSAPlanarity(*m_GA, grid);
    planarity->computeEnergy();

    EXPECT_EQ(1, planarity->energy()) << "Grid correctly detects one crossing";

    ogdf::Hashing<ogdf::node, ogdf::DPoint> layoutChanges;
    layoutChanges.insert(b, ogdf::DPoint(0.1, 0.7));
    layoutChanges.insert(a, ogdf::DPoint(0.05,0.05));
    double candEnergy = planarity->computeCandidateEnergy(layoutChanges);
    EXPECT_EQ(0, candEnergy) << "Grid calculates correct candidate energy";

    EXPECT_EQ(1, planarity->energy()) << "Grid maintains energy while candidate is not accepted";

    //EXPECT_EQ(false, grid->newGridNecessary(b, newPos)) << "No new grid is necessary";

    planarity->candidateTaken();
    EXPECT_EQ(0, planarity->energy()) << "Grid correctly updates energy when candidate is accepted";

    delete grid;
    delete planarity;
}

TEST_F(TSAUniformGridTest, AngRestInitialCorrectness) {
    ogdf::TSAAngularResolution *angres = new ogdf::TSAAngularResolution(*m_GA);
    angres->computeEnergy();
    double idealAngle = 2*M_PI/3.0;
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle * 4, angres->energy());

    m_GA->x(b) = 0.5;
    m_GA->y(b) = 0.5;

    angres->computeEnergy();
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle + (idealAngle-M_PI/2.0)/idealAngle + 2, angres->energy());
}

TEST_F(TSAUniformGridTest, AngRestCandidateEnergyCorrectness) {
    ogdf::TSAAngularResolution *angres = new ogdf::TSAAngularResolution(*m_GA);
    angres->computeEnergy();
    double idealAngle = 2*M_PI/3.0;

    ogdf::DPoint newPos = ogdf::DPoint(0.5, 0.5);
    double candEnergy = computeCandidateEnergy(b, newPos, angres);
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle + (idealAngle-M_PI/2.0)/idealAngle + 2, candEnergy) << "Candidate energy is correct";

    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle * 4, angres->energy()) << "Energy should not be updated yet";

    m_GA->x(b) = 0.5;
    m_GA->y(b) = 0.5;
    angres->candidateTaken();
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle + (idealAngle-M_PI/2.0)/idealAngle + 2, angres->energy()) << "Updated energy is correct";

    newPos = ogdf::DPoint(1, 0);
    candEnergy = computeCandidateEnergy(b, newPos, angres);
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle * 4, candEnergy) << "Candidate energy is correct (2)";

    m_GA->x(b) = 1;
    m_GA->y(b) = 0;
    angres->candidateTaken();
    EXPECT_DOUBLE_EQ((idealAngle-M_PI/4.0)/idealAngle * 4, angres->energy()) << "Updated energy is correct (2)";
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
