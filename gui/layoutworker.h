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

signals:
    void finished(QString timingResult);
    void energyInfoAvailable(double energy, double temperature);

public slots:
    void run();

private:
    ogdf::LayoutModule *layout;
    ogdf::GraphAttributes *GA;

};

#endif // LAYOUTWORKER_H
