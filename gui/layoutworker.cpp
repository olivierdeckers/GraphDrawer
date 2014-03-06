#include "gui/layoutworker.h"
#include "ogdf/TSALayout.h"
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

    ogdf::TSALayout* tsaLayout = dynamic_cast<ogdf::TSALayout*>(layout);
    if(tsaLayout != nullptr) {
        tsaLayout->setWorker(this);
    }

    layout->call(*GA);

    delete layout;

    QString time;
    time.sprintf("%i ms", timer.elapsed());
    emit finished(time);
}

void LayoutWorker::energyInfo(double energy, double temperature)
{
    emit energyInfoAvailable(energy, temperature);
}
