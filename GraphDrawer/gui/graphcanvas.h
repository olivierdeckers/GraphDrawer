#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QWidget>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class GraphCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit GraphCanvas(QWidget *parent = 0);
    ~GraphCanvas();

    void setGraph(ogdf::GraphAttributes &GA, ogdf::Graph &G);

protected:
    void paintEvent(QPaintEvent *);

private:
    ogdf::GraphAttributes *m_GA;
    ogdf::Graph *m_G;
    double minX, minY, maxX, maxY;

    void calculateBBox(double margin);
    void scalePoint(double &x, double &y);
    void scaleDimension(double &w, double &h);

signals:

public slots:

};

#endif // GRAPHCANVAS_H
