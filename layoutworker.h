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

signals:
    void finished(QString timingResult);

public slots:
    void run();

private:
    ogdf::LayoutModule *layout;
    ogdf::GraphAttributes *GA;

};

#endif // LAYOUTWORKER_H
