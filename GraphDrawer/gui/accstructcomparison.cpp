//#include "accstructcomparison.h"
//#include "ui_accstructcomparison.h"
//#include "ogdf/TSALayout.h"
//#include <ogdf/basic/Graph.h>
//#include <ogdf/fileformats/GmlParser.h>

//AccStructComparison::AccStructComparison(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::AccStructComparison)
//{

//    srand(time(NULL));
//    ui->setupUi(this);

//    performLayouting();
//}

//AccStructComparison::~AccStructComparison()
//{
//    delete ui;
//}

//void loadGraph(const std::string filename, ogdf::Graph &G, ogdf::GraphAttributes &GA)
//{
//    ogdf::GmlParser parser = ogdf::GmlParser(filename.c_str());
//    parser.read(G, GA);

//    //GA.setDirected(false);

//    ogdf::node v;
//    forall_nodes(v, G) {
//        GA.width(v) = 10;
//        GA.height(v) = 10;
//    }
//}

//void randomLayout(ogdf::Graph &G, ogdf::GraphAttributes &GA)
//{
//    ogdf::node v;
//    forall_nodes(v, G) {
//        GA.x(v) = rand() % 1024;
//        GA.y(v) = rand() % 1024;
//    }
//}

//void AccStructComparison::performLayouting()
//{
//    ogdf::Graph G = ogdf::Graph();
//    ogdf::GraphAttributes GA = ogdf::GraphAttributes(G, ogdf::GraphAttributes::nodeGraphics |
//       ogdf:: GraphAttributes::edgeGraphics );

//    QString filename = QCoreApplication::applicationDirPath() + "/../../GraphDrawer/graphs/60.gml";
//    loadGraph(filename.toStdString().c_str(), G, GA);



//    ogdf::TSALayout layout = ogdf::TSALayout();
//    int samples = 10;
//    double progress = 0;
//    double progressDelta = 1.0/3.0*samples;

//    layout.fixSettings(ogdf::TSALayout::spPlanar);
//    layout.setAttractionWeight(1);
//    layout.setRepulsionWeight(1);
//    layout.setPlanarityWeight(3);
//    layout.setQuality(10);
//    layout.setPreferredEdgeLength(50);

//    m_repulsion = new ogdf::TSARepulsion(GA, 50);
//    m_attraction = new ogdf::TSAAttraction(GA, 50);
//    m_planarity = new ogdf::TSAPlanarity(GA);

//    for ( int accInt = ogdf::TSALayout::AccelerationStructureType::grid; accInt <= ogdf::TSALayout::AccelerationStructureType::none; accInt++ )
//    {
//       ogdf::TSALayout::AccelerationStructureType accStruct = static_cast<ogdf::TSALayout::AccelerationStructureType>(accInt);
//        layout.setAccelerationStructureParameter(accStruct);
//        double meanRepulsionEnergy = 0;
//        double meanAttractionEnergy = 0;
//        double meanPlanarityEnergy = 0;
//        double meanTime;
//        QTime time = QTime();
//        for(int i=0; i<samples; i++) {
//            randomLayout(G, GA);
//            time.start();
//            layout.call(GA);
//            meanTime += time.elapsed();

//            m_attraction->computeEnergy();
//            m_repulsion->computeEnergy();
//            m_planarity->computeEnergy();
//            meanAttractionEnergy += m_attraction->energy();
//            meanRepulsionEnergy += m_repulsion->energy();
//            meanPlanarityEnergy += m_planarity->energy();

//            progress += progressDelta;
//            cout << progress << endl;
//            //ui->progress->setValue(ceil(progress * 100));
//        }
//        meanAttractionEnergy /= 10.0;
//        meanRepulsionEnergy /= 10.0;
//        meanPlanarityEnergy /= 10.0;
//        meanTime /= 10.0;
//        cout << "none" << endl;
//        cout << "attraction: " << meanAttractionEnergy << endl;
//        cout << "repulsion: " << meanRepulsionEnergy << endl;
//        cout << "planarity: " << meanPlanarityEnergy << endl;
//        cout << "time: " << meanTime << endl;
//    }
//}
