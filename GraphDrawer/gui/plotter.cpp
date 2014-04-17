#include "plotter.h"

Plotter::Plotter(QCustomPlot *energyPlotter, QCustomPlot *neighbourhoodPlotter) : QObject()
{
    this->energyPlotter = energyPlotter;
    this->neighbourhoodPlotter = neighbourhoodPlotter;
    energy_iteration = 0;
    neighbourhood_iteration = 0;

    initEnergyPlotter();
    initNeighbourhoodPlotter();
}

void Plotter::initEnergyPlotter()
{
    energyPlotter->xAxis->setLabel("Iteration");
    energyPlotter->xAxis->setRangeLower(0);
    energyPlotter->yAxis->setLabel("Energy");
    energyPlotter->yAxis->setRangeLower(0);
    energyPlotter->yAxis2->setLabel("Temperature");
    energyPlotter->yAxis2->setVisible(true);
    energyPlotter->yAxis2->setRangeLower(0);
    energyPlotter->yAxis2->setRangeUpper(0);
    energyPlotter->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    energyPlotter->legend->setVisible(true);

    energyPlotter->addGraph(energyPlotter->xAxis, energyPlotter->yAxis);
    energyPlotter->addGraph(energyPlotter->xAxis, energyPlotter->yAxis2);

    energyPlotter->graph(0)->setPen(QPen(Qt::blue));
    energyPlotter->graph(0)->setName("Energy");
    energyPlotter->graph(1)->setName("Temperature");
    energyPlotter->graph(1)->setPen(QPen(Qt::red));

    energyPlotter->setNoAntialiasingOnDrag(true);

    energyPlotter->setInteraction(QCP::iRangeZoom, true);
    energyPlotter->setInteraction(QCP::iRangeDrag, true);
}

void Plotter::initNeighbourhoodPlotter()
{
    neighbourhoodPlotter->xAxis->setLabel("Iteration");
    neighbourhoodPlotter->xAxis->setRangeLower(0);
    neighbourhoodPlotter->yAxis->setLabel("Chance");
    neighbourhoodPlotter->yAxis->setRangeLower(0);
    neighbourhoodPlotter->yAxis->setRangeUpper(1);

    neighbourhoodPlotter->legend->setVisible(true);

    neighbourhoodPlotter->setInteraction(QCP::iRangeZoom, true);
    neighbourhoodPlotter->axisRect(0)->setRangeDrag(Qt::Horizontal);
    neighbourhoodPlotter->axisRect(0)->setRangeZoom(Qt::Horizontal);
    neighbourhoodPlotter->setInteraction(QCP::iRangeDrag, true);
}

void Plotter::neighbourhoodLegendEntry(int id, QString name)
{
    neighbourhoodPlotter->addGraph(neighbourhoodPlotter->xAxis, neighbourhoodPlotter->yAxis);
    QColor color;
    double hue = id / 10.0;
    color.setHsvF(hue, 1, 1);
    neighbourhoodPlotter->graph(id)->setPen(QPen(color));
    neighbourhoodPlotter->graph(id)->setName(name);
}

void Plotter::energyInfo(double energy, double temperature)
{
    energy_iteration++;
    energyPlotter->graph(0)->addData(energy_iteration, energy);
    energyPlotter->graph(1)->addData(energy_iteration, temperature);
    energyPlotter->yAxis->setRangeUpper(std::max(energyPlotter->yAxis->range().upper, energy));
    energyPlotter->yAxis2->setRangeUpper(std::max(energyPlotter->yAxis2->range().upper, temperature));
    energyPlotter->xAxis->setRangeUpper(energy_iteration);
    if(energy_iteration % 1000 == 0)
        energyPlotter->replot();
}

void Plotter::clear()
{
    neighbourhood_iteration = energy_iteration = 0;
    energyPlotter->yAxis->setRangeUpper(1);
    energyPlotter->yAxis2->setRangeUpper(1);
    energyPlotter->graph(0)->clearData();
    energyPlotter->graph(1)->clearData();

    neighbourhoodPlotter->clearGraphs();
}


void Plotter::neighbourhoodSelectionChanceInfo(int id, double chance)
{
    if(id == 0)
        neighbourhood_iteration++;

    neighbourhoodPlotter->graph(id)->addData(neighbourhood_iteration, chance);
    neighbourhoodPlotter->xAxis->setRangeUpper(neighbourhood_iteration);

    if(neighbourhood_iteration % 1000 == 0)
        neighbourhoodPlotter->replot();
}
