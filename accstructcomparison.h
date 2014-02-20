#ifndef ACCSTRUCTCOMPARISON_H
#define ACCSTRUCTCOMPARISON_H

#include <QWidget>
#include "TSAPlanarity.h"
#include "TSAAttraction.h"
#include "TSARepulsion.h"

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

    ogdf::TSAPlanarity *m_planarity;
    ogdf::TSARepulsion *m_repulsion;
    ogdf::TSAAttraction *m_attraction;
};

#endif // ACCSTRUCTCOMPARISON_H
