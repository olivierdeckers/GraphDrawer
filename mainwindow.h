#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_loadGraph_clicked();

    void on_graphFileInput_currentIndexChanged(const QString &arg1);

    void on_layoutButton_clicked();

    void on_pushButton_2_clicked();

    void layoutFinished(QString timingResult);

private:
    void loadGraph(const string filename);
    void layoutGraph();
    void drawGraph();

    Ui::MainWindow *ui;
    ogdf::Graph *m_G;
    ogdf::GraphAttributes *m_GA;
    QHash<QString, QString> *graphs;
};

#endif // MAINWINDOW_H
