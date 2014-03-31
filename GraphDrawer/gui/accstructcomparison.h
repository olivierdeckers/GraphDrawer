#ifndef ACCSTRUCTCOMPARISON_H
#define ACCSTRUCTCOMPARISON_H

#include <QWidget>
#include "ogdf/TSAPlanarity.h"
#include "ogdf/TSAAttraction.h"
#include "ogdf/TSARepulsion.h"
#include <ogdf/TSAAngularResolution.h>

namespace Ui {
class AccStructComparison;
}

class AccStructComparison : public QWidget
{
    Q_OBJECT

public:
    explicit AccStructComparison(QWidget *parent = 0);
    ~AccStructComparison();

private:
    Ui::AccStructComparison *ui;
    void performLayouting();

    ogdf::AccelerationStructure *m_accStruct;

    ogdf::TSAPlanarity *m_planarity;
    ogdf::TSARepulsion *m_repulsion;
    ogdf::TSAAttraction *m_attraction;
    ogdf::TSAAngularResolution *m_angres;
};

#endif // ACCSTRUCTCOMPARISON_H
