#include "accstructcomparison.h"
#include "ui_accstructcomparison.h"
#include "ogdf/TSALayout.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GmlParser.h>
#include <ogdf/RandomMove.h>
#include <ogdf/RemoveCrossing.h>
#include <ogdf/TSANoAcceleration.h>

AccStructComparison::AccStructComparison(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccStructComparison)
{

    srand(time(NULL));
    ui->setupUi(this);

    performLayouting();
}

AccStructComparison::~AccStructComparison()
{
    delete ui;
}

void loadGraph(const std::string filename, ogdf::Graph &G, ogdf::GraphAttributes &GA)
{
    ogdf::GmlParser parser = ogdf::GmlParser(filename.c_str());
    parser.read(G, GA);

    ogdf::node v;
    forall_nodes(v, G) {
        GA.width(v) = 10;
        GA.height(v) = 10;
    }
}

void randomLayout(ogdf::Graph &G, ogdf::GraphAttributes &GA)
{
    ogdf::node v;
    forall_nodes(v, G) {
        GA.x(v) = (double) rand() / RAND_MAX;
        GA.y(v) = (double) rand() / RAND_MAX;
    }
}

void AccStructComparison::performLayouting()
{
    ogdf::Graph G = ogdf::Graph();
    ogdf::GraphAttributes GA = ogdf::GraphAttributes(G, ogdf::GraphAttributes::nodeGraphics |
       ogdf:: GraphAttributes::edgeGraphics );

    QString filename = QCoreApplication::applicationDirPath() + "/../GraphDrawer/GraphDrawer/graphs/60.gml";
    loadGraph(filename.toStdString().c_str(), G, GA);

    ogdf::TSALayout layout = ogdf::TSALayout();
    int samples = 10;

    layout.fixSettings(ogdf::TSALayout::spPlanar);
    layout.setAttractionWeight(1);
    layout.setRepulsionWeight(1);
    layout.setAngularResolutionWeight(1);
    layout.setPlanarityWeight(3);
    layout.setQuality(1000);
    layout.setPreferredEdgeLength(5);
    layout.setAccelerationStructureParameter(ogdf::TSALayout::none);

    m_accStruct = new ogdf::TSANoAcceleration(GA);
    m_repulsion = new ogdf::TSARepulsion(GA, 5);
    m_attraction = new ogdf::TSAAttraction(GA, 5);
    m_planarity = new ogdf::TSAPlanarity(GA, m_accStruct);
    m_angres = new ogdf::TSAAngularResolution(GA);

    double meanRepulsionEnergy = 0;
    double meanAttractionEnergy = 0;
    double meanPlanarityEnergy = 0;
    double meanAngResEnergy = 0;
    double meanTime;
    QTime time = QTime();
    for(int i=0; i<samples; i++) {
        randomLayout(G, GA);
        time.start();
        layout.call(GA);
        meanTime += time.elapsed();

        m_attraction->computeEnergy();
        m_repulsion->computeEnergy();
        m_planarity->computeEnergy();
        m_angres->computeEnergy();
        meanAttractionEnergy += m_attraction->energy();
        meanRepulsionEnergy += m_repulsion->energy();
        meanPlanarityEnergy += m_planarity->energy();
        meanAngResEnergy += m_angres->energy();
    }
    meanAttractionEnergy /= samples;
    meanRepulsionEnergy /= samples;
    meanPlanarityEnergy /= samples;
    meanAngResEnergy /= samples;
    meanTime /= samples;
    cout << "attraction: " << meanAttractionEnergy << endl;
    cout << "repulsion: " << meanRepulsionEnergy << endl;
    cout << "planarity: " << meanPlanarityEnergy << endl;
    cout << "angular res: " << meanAngResEnergy << endl;
    cout << "time: " << meanTime << endl;
}
