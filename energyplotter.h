#ifndef ENERGYPLOTTER_H
#define ENERGYPLOTTER_H

#include <QObject>
#include "qcustomplot.h"

class EnergyPlotter : public QObject
{
    Q_OBJECT
public:
    EnergyPlotter(QCustomPlot * plot);

signals:

public slots:
    void energyInfoAvailable(double energy, double temperature);

private:
    QCustomPlot * plotter;
    int iteration;
};

#endif // ENERGYPLOTTER_H
