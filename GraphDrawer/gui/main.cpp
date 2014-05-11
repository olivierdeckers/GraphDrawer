#include "mainwindow.h"
#include <ogdf/fileformats/GmlParser.h>
#include <ogdf/basic/Graph.h>
#include <ogdf/TSALayout.h>
#include <ogdf/AccelerationStructure.h>
#include <ogdf/TSAUniformGrid.h>
#include <ogdf/TSANoAcceleration.h>
#include <ogdf/TSARepulsion.h>
#include <ogdf/TSAAttraction.h>
#include <ogdf/TSAPlanarity.h>
#include <ogdf/TSAAngularResolution.h>
#include <QApplication>

void loadGraph(const std::string filename, ogdf::Graph &G, ogdf::GraphAttributes &GA)
{
    ogdf::GmlParser parser = ogdf::GmlParser(filename.c_str());
    parser.read(G, GA);

    ogdf::node v;
    forall_nodes(v, G) {
        GA.width(v) = 0.05;
        GA.height(v) = 0.05;
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

void compareAccStruct()
{
    ogdf::Graph G = ogdf::Graph();
    ogdf::GraphAttributes GA = ogdf::GraphAttributes(G, ogdf::GraphAttributes::nodeGraphics |
       ogdf::GraphAttributes::edgeGraphics | ogdf::GraphAttributes::nodeLabel);

    loadGraph("../GraphDrawer/GraphDrawer/graphs/grid.gml", G, GA);

    ogdf::TSALayout layout = ogdf::TSALayout();
    int samples = 10;

    layout.setAttractionWeight(1);
    layout.setRepulsionWeight(1);
    layout.setAngularResolutionWeight(1);
    layout.setPlanarityWeight(3);
    layout.setQuality(7);
    layout.setPreferredEdgeLength(5);
    layout.setAccelerationStructureParameter(ogdf::TSALayout::grid);

    double meanRepulsionEnergy = 0;
    double meanAttractionEnergy = 0;
    double meanPlanarityEnergy = 0;
    double meanAngResEnergy = 0;
    //double meanTime;
    //QTime time;
    for(int i=0; i<samples; i++) {
        randomLayout(G, GA);
        layout.call(GA);
        //ogdf::TSAUniformGrid accStruct(GA);
        //ogdf::TSAPlanarity planarity(GA, &accStruct);
        //time.start();
        //layout.call(GA);
        //cout << time.elapsed() << endl;

        ogdf::TSAUniformGrid accStruct(GA);
        ogdf::TSARepulsion repulsion(GA, 5);
        ogdf::TSAAttraction attraction(GA, 5);
        ogdf::TSAPlanarity planarity(GA, &accStruct);
        ogdf::TSAAngularResolution angres(GA);

        attraction.computeEnergy();
        repulsion.computeEnergy();
        planarity.computeEnergy();
        angres.computeEnergy();
        meanAttractionEnergy += attraction.energy();
        meanRepulsionEnergy += repulsion.energy();
        meanPlanarityEnergy += planarity.energy();
        meanAngResEnergy += angres.energy();
    }
    meanAttractionEnergy /= samples;
    meanRepulsionEnergy /= samples;
    meanPlanarityEnergy /= samples;
    meanAngResEnergy /= samples;
    cout << meanAttractionEnergy + meanRepulsionEnergy + meanAngResEnergy + meanPlanarityEnergy * 3 << endl;
    //meanTime /= samples;
    /*cout << "attraction: " << meanAttractionEnergy << endl;
    cout << "repulsion: " << meanRepulsionEnergy << endl;
    cout << "planarity: " << meanPlanarityEnergy << endl;
    cout << "angular res: " << meanAngResEnergy << endl;*/
    //cout << "time: " << meanTime << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();/**/

    /*compareAccStruct();

    return 0;/**/

}
