#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gui/layoutworker.h"
#include <ogdf/fileformats/GmlParser.h>
//#include <ogdf/fileformats/GraphIO.h>
#include "ogdf/TSALayout.h"
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/SpringEmbedderKK.h>
#include <iostream>
#include <QHash>
#include <QFileDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_G = new ogdf::Graph();
    m_GA = new ogdf::GraphAttributes(*m_G, ogdf::GraphAttributes::nodeGraphics |
       ogdf:: GraphAttributes::edgeGraphics | ogdf::GraphAttributes::nodeLabel );

    ui->graphCanvas->setGraph(*m_GA, *m_G);

    graphs = new QHash<QString, QString>();
    graphs->insert("20 nodes", "graphs/20.gml");
    graphs->insert("60 nodes", "graphs/60.gml");
    graphs->insert("K4", "graphs/K4.gml");
    graphs->insert("rose", "graphs/rose.gml");
    graphs->insert("cube", "graphs/cube.gml");
    graphs->insert("grid", "graphs/grid.gml");
    graphs->insert("tree", "graphs/tree.gml");
    graphs->insert("fb", "graphs/fb.gml");
    graphs->insert("dodecahedron", "graphs/dodecahedron.gml");
    graphs->insert("bipartite", "graphs/bipartite2.gml");
    graphs->insert("cuboid", "graphs/balk.gml");
    graphs->insert("cobweb", "graphs/cobweb.gml");
    graphs->insert("k33", "graphs/k33.gml");
    graphs->insert("ring", "graphs/ring.gml");
    QList<QString> keys = graphs->keys();
    qSort(keys);
    ui->graphFileInput->addItems(keys);

    plotter = new Plotter(ui->energyPlotWidget, ui->neighbourhoodPlotWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_GA;
    delete m_G;
    delete plotter;
}

void MainWindow::loadGraph(const std::string filename) {
    //ogdf::GraphIO::readGML(*m_GA, *m_G, filename);
    ogdf::GmlParser parser = ogdf::GmlParser(filename.c_str());
    parser.read(*m_G, *m_GA);

    ogdf::node v;
    forall_nodes(v, *m_G) {
        m_GA->width(v) = 0.05;
        m_GA->height(v) = 0.05;
    }

    double width = m_GA->boundingBox().width();
    forall_nodes(v, *m_G) {
        m_GA->x(v) /= width;
        m_GA->y(v) /= width;
    }

    ui->graphCanvas->update();
}

void MainWindow::layoutGraph()
{
    ogdf::LayoutModule *layout = 0;

    QString layoutName = ui->layoutSpinner->currentText();
    if(layoutName.compare("TSA") == 0) {
        ogdf::TSALayout *tLayout = new ogdf::TSALayout();

        tLayout->setPreferredEdgeLength(0);
        tLayout->setPreferredEdgeLengthMultiplier(ui->edgeLengthInput->value());
        tLayout->setAttractionWeight(ui->attractionWeight->value());
        tLayout->setRepulsionWeight(ui->repulsionWeight->value());
        tLayout->setPlanarityWeight(ui->planarityInput->value());
        tLayout->setAngularResolutionWeight(ui->angResWeight->value());
        tLayout->setEdgeLengthWeight(ui->edgeLengthWeight->value());
        tLayout->setQuality(ui->qualityInput->value());
        switch(ui->accStructBox->currentIndex()) {
        case 0:
            tLayout->setAccelerationStructureParameter(ogdf::TSALayout::AccelerationStructureType::none);
            break;
        case 1:
            tLayout->setAccelerationStructureParameter(ogdf::TSALayout::AccelerationStructureType::grid);
            break;
        case 2:
            tLayout->setAccelerationStructureParameter(ogdf::TSALayout::AccelerationStructureType::approximation);
            break;
        }

        layout = tLayout;
    }
    else if(layoutName.compare("DH") == 0) {
        ogdf::DavidsonHarelLayout *dLayout = new ogdf::DavidsonHarelLayout();
        dLayout->setSpeed(ogdf::DavidsonHarelLayout::sppFast);
        dLayout->fixSettings(ogdf::DavidsonHarelLayout::spPlanar);
        dLayout->setPreferredEdgeLengthMultiplier(ui->edgeLengthInput->value());
        layout = dLayout;
    }
    else if(layoutName.compare("FR") == 0) {
        ogdf::SpringEmbedderFR *frLayout = new ogdf::SpringEmbedderFR();
        layout = frLayout;
    }
    else if(layoutName.compare("KK") == 0) {
        ogdf::SpringEmbedderKK *kkLayout = new ogdf::SpringEmbedderKK();
        layout = kkLayout;
    }
    else if(layoutName.compare("FM3") == 0) {
        ogdf::FMMMLayout *fmLayout = new ogdf::FMMMLayout();
        layout = fmLayout;
    }

    QThread *thread = new QThread;
    LayoutWorker *worker = new LayoutWorker(layout, m_GA);
    connect(thread, SIGNAL(started()), worker, SLOT(run()));

    connect(worker, SIGNAL(finished(QString)), this, SLOT(layoutFinished(QString)));
    connect(worker, SIGNAL(finished(QString)), ui->graphCanvas, SLOT(update()), Qt::QueuedConnection);

    connect(worker, SIGNAL(energyInfoSignal(double,double)), plotter, SLOT(energyInfo(double,double)), Qt::QueuedConnection);
    connect(worker, SIGNAL(energyInfoSignal(double,double)), ui->graphCanvas, SLOT(update()), Qt::QueuedConnection);

    connect(worker, SIGNAL(neighbourhoodLegendEntrySignal(int,QString)), plotter, SLOT(neighbourhoodLegendEntry(int,QString)), Qt::QueuedConnection);
    connect(worker, SIGNAL(neighbourhoodSelectionChanceSignal(int,double)), plotter, SLOT(neighbourhoodSelectionChanceInfo(int,double)), Qt::QueuedConnection);

    worker->moveToThread(thread);
    thread->start();
}

void MainWindow::on_loadGraph_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open graph"), "graphs/", tr("*.gml"));
    if(filename.length() > 0) {
        loadGraph(filename.toStdString().c_str());
    }
}

void MainWindow::on_graphFileInput_currentIndexChanged(const QString &index)
{
    QString filename = QCoreApplication::applicationDirPath() + "/../GraphDrawer/GraphDrawer/" + graphs->value(index);
    loadGraph(filename.toStdString().c_str());
}

void MainWindow::on_layoutButton_clicked()
{
    layoutGraph();
    ui->graphCanvas->update();

    plotter->clear();
}

void MainWindow::on_randomLayoutButton_clicked()
{
    ogdf::node v;
    forall_nodes(v, *m_G) {
        m_GA->x(v) = (double) rand() / RAND_MAX;
        m_GA->y(v) = (double) rand() / RAND_MAX;
    }

    ui->graphCanvas->update();

    plotter->clear();
}

void MainWindow::layoutFinished(QString timingResult) {
    ui->layoutDuration->setText(timingResult);
}
