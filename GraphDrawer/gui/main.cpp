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
#include <ogdf/TSAEdgeLength.h>
#include <ogdf/NodeEdgeDistance.h>
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

    loadGraph("../GraphDrawer/GraphDrawer/graphs/60.gml", G, GA);

    ogdf::TSALayout layout = ogdf::TSALayout();
    int samples = 10;

    layout.setAttractionWeight(1);
    layout.setRepulsionWeight(1);
    layout.setAngularResolutionWeight(1);
    layout.setPlanarityWeight(3);
    layout.setEdgeLengthWeight(0);
    layout.setQuality(10);
    layout.setPreferredEdgeLength(5);
    layout.setAccelerationStructureParameter(ogdf::TSALayout::grid);

    //double meanRepulsionEnergy = 0;
    //double meanAttractionEnergy = 0;
    //double meanPlanarityEnergy = 0;
    //double meanAngResEnergy = 0;
    //double meanEnergy = 0.0;
    //double meanTime;
    QTime time;
    for(int i=0; i<samples; i++) {
        randomLayout(G, GA);
        time.start();
        layout.call(GA);
        cout << time.elapsed() << endl;
        //ogdf::TSAUniformGrid accStruct(GA);
        //ogdf::TSAPlanarity planarity(GA, &accStruct);
        //time.start();
        //layout.call(GA);
        //cout << time.elapsed() << endl;

        /*ogdf::TSAUniformGrid accStruct(GA);
        ogdf::TSARepulsion repulsion(GA);
        ogdf::TSAAttraction attraction(GA);
        ogdf::TSAPlanarity planarity(GA, &accStruct);
        ogdf::TSAAngularResolution angres(GA);
        ogdf::TSAEdgeLength edgeLength(GA, 5);
        ogdf::NodeEdgeDistance nodeEdgeDist(GA, &accStruct);*/

        /*attraction.computeEnergy();
        repulsion.computeEnergy();
        planarity.computeEnergy();
        angres.computeEnergy();
        edgeLength.computeEnergy();
        nodeEdgeDist.computeEnergy();
        meanEnergy += attraction.energy() + repulsion.energy() + 3*planarity.energy() + angres.energy() + edgeLength.energy() + nodeEdgeDist.energy();*/
        /*meanAttractionEnergy += attraction.energy();
        meanRepulsionEnergy += repulsion.energy();
        meanPlanarityEnergy += planarity.energy();
        meanAngResEnergy += angres.energy();*/
    }
    /*meanAttractionEnergy /= samples;
    meanRepulsionEnergy /= samples;
    meanPlanarityEnergy /= samples;
    meanAngResEnergy /= samples;*/
    //meanEnergy /= samples;
    //cout << meanEnergy << endl;
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
