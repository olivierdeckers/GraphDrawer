#include "energyplotter.h"

EnergyPlotter::EnergyPlotter(QCustomPlot *plotter) : QObject()
{
    this->plotter = plotter;
    iteration = 0;

    plotter->xAxis->setLabel("Iteration");
    plotter->yAxis->setLabel("Energy");
    plotter->yAxis2->setLabel("Temperature");
    plotter->yAxis2->setVisible(true);
    plotter->legend->setVisible(true);

    plotter->addGraph(plotter->xAxis, plotter->yAxis);
    plotter->addGraph(plotter->xAxis, plotter->yAxis2);

    plotter->graph(0)->setPen(QPen(Qt::blue));
    plotter->graph(0)->setName("Energy");
    plotter->graph(1)->setName("Temperature");
    plotter->graph(1)->setPen(QPen(Qt::red));
}

void EnergyPlotter::energyInfoAvailable(double energy, double temperature)
{
    iteration++;
    plotter->graph(0)->addData(iteration, energy);
    plotter->graph(1)->addData(iteration, temperature);
    plotter->graph(0)->rescaleAxes();
    plotter->graph(1)->rescaleAxes(true);
    plotter->replot();
}
