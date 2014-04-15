#ifndef ENERGYPLOTTER_H
#define ENERGYPLOTTER_H

#include <QObject>
#include <QHash>
#include "qcustomplot.h"

class Plotter : public QObject
{
    Q_OBJECT
public:
    Plotter(QCustomPlot *energyPlotter, QCustomPlot *neighbourhoodPlotter);

    void clear();

signals:

public slots:
    void energyInfo(double energy, double temperature);
    void neighbourhoodSelectionChanceInfo(int id, double chance);
    void neighbourhoodLegendEntry(int id, QString name);

private:
    QCustomPlot *energyPlotter;
    QCustomPlot *neighbourhoodPlotter;
    int energy_iteration, neighbourhood_iteration;

    void initEnergyPlotter();
    void initNeighbourhoodPlotter();
};

#endif // ENERGYPLOTTER_H
