#include "layoutworker.h"
#include <QTime>

LayoutWorker::LayoutWorker(ogdf::LayoutModule *layout, ogdf::GraphAttributes *GA)
{
    this->layout = layout;
    this->GA = GA;
}

void LayoutWorker::run()
{
    QTime timer;
    timer.start();

    layout->call(*GA);

    delete layout;

    QString time;
    time.sprintf("%i ms", timer.elapsed());
    emit finished(time);
}
