#include "graphcanvas.h"
#include <QPainter>
#include <QTimer>

GraphCanvas::GraphCanvas(QWidget *parent) :
    QWidget(parent)
{
    m_GA = NULL;
    m_G = NULL;
}

GraphCanvas::~GraphCanvas()
{

}

void GraphCanvas::scalePoint(double &x, double &y)
{
    double factor = min(width() / (maxX - minX), height() / (maxY - minY));
    x = (x - minX) * factor;
    y = (y - minY) * factor;
}

void GraphCanvas::scaleDimension(double &w, double &h)
{
    double factor = min(width() / (maxX - minX), height() / (maxY - minY));
    w = w * factor;
    h = h * factor;
}

void GraphCanvas::calculateBBox(double margin)
{
    minX = 1e10; minY = 1e10;
    maxX = 0; maxY = 0;
    double x, y;
    ogdf::node v;
    forall_nodes(v, *m_G) {
        x = m_GA->x(v);
        y = m_GA->y(v);

        if(x>maxX)
            maxX = x;
        if(x<minX)
            minX = x;
        if(y>maxY)
            maxY = y;
        if(y<minY)
            minY = y;
    }

    maxX += margin;
    minX -= margin;
    maxY += margin;
    minY -= margin;
}

void GraphCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(m_GA != NULL && m_G != NULL)
    {
        double margin = 10;
        calculateBBox(margin);

        painter.setBrush(QBrush());
        painter.setPen(Qt::black);

        ogdf::edge e;
        forall_edges(e, *m_G) {
            double x1 = m_GA->x(e->source()), y1 = m_GA->y(e->source());
            double x2 = m_GA->x(e->target()), y2 = m_GA->y(e->target());
            scalePoint(x1, y1);
            scalePoint(x2, y2);
            painter.drawLine(x1, y1, x2, y2);
        }

        painter.setBrush(QBrush(QColor(255,77,77), Qt::SolidPattern));
        painter.setPen(QPen(QColor(182, 31, 31)));

        ogdf::node v;
        forall_nodes(v, *m_G) {
            double x = m_GA->x(v), y = m_GA->y(v);
            scalePoint(x, y);
            double w =  m_GA->width(v), h = m_GA->height(v);
            scaleDimension(w, h);
            x = x - w / 2.0;
            y = y - h / 2.0;

            painter.drawEllipse(x, y, w, h);
        }
    }
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);*/
    update();
}

void GraphCanvas::setGraph(ogdf::GraphAttributes &GA, ogdf::Graph &G)
{
    m_GA = &GA;
    m_G = &G;
}

