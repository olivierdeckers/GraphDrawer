#ifndef LAYOUTWORKER_H
#define LAYOUTWORKER_H

#include <QObject>
#include "ogdf/module/LayoutModule.h"
#include "ogdf/basic/GraphAttributes.h"

class LayoutWorker : public QObject
{
    Q_OBJECT
public:
    explicit LayoutWorker(ogdf::LayoutModule *layout, ogdf::GraphAttributes *GA);
    //~LayoutWorker();

    void energyInfo(double energy, double temperature);
    void neighbourhoodSelectionChance(int id, double chance);
    void neighbourhoodLegendEntry(int id, QString name);

signals:
    void finished(QString timingResult);
    void energyInfoSignal(double energy, double temperature);
    void neighbourhoodSelectionChanceSignal(int id, double chance);
    void neighbourhoodLegendEntrySignal(int id, QString name);

public slots:
    void run();

private:
    ogdf::LayoutModule *layout;
    ogdf::GraphAttributes *GA;

};

#endif // LAYOUTWORKER_H
