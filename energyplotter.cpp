#include "energyplotter.h"

EnergyPlotter::EnergyPlotter(QCustomPlot *plotter) : QObject()
{
    this->plotter = plotter;
    iteration = 0;

    plotter->xAxis->setLabel("Iteration");
    plotter->xAxis->setRangeLower(0);
    plotter->yAxis->setLabel("Energy");
    plotter->yAxis->setRangeLower(0);
    plotter->yAxis2->setLabel("Temperature");
    plotter->yAxis2->setVisible(true);
    plotter->yAxis2->setRangeLower(0);
    plotter->yAxis2->setRangeUpper(0);
    plotter->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    plotter->legend->setVisible(true);

    plotter->addGraph(plotter->xAxis, plotter->yAxis);
    plotter->addGraph(plotter->xAxis, plotter->yAxis2);

    plotter->graph(0)->setPen(QPen(Qt::blue));
    plotter->graph(0)->setName("Energy");
    plotter->graph(1)->setName("Temperature");
    plotter->graph(1)->setPen(QPen(Qt::red));

    plotter->setNoAntialiasingOnDrag(true);

    plotter->setInteraction(QCP::iRangeZoom, true);
    plotter->setInteraction(QCP::iRangeDrag, true);
}

void EnergyPlotter::energyInfoAvailable(double energy, double temperature)
{
    iteration++;
    plotter->graph(0)->addData(iteration, energy);
    plotter->graph(1)->addData(iteration, temperature);
    plotter->yAxis->setRangeUpper(std::max(plotter->yAxis->range().upper, energy));
    plotter->yAxis2->setRangeUpper(std::max(plotter->yAxis2->range().upper, temperature));
    plotter->xAxis->setRangeUpper(iteration);
    if(iteration%1000 == 0)
        plotter->replot();
}
