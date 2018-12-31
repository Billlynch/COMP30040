#ifndef KERRROTATIONGRAPH_H
#define KERRROTATIONGRAPH_H

#include <QWidget>
#include <QChart>
#include <QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLineSeries>
#include <complex>
#include <map>

typedef std::pair<std::complex<double>, std::complex<double>> GraphItem; // p then s
typedef std::map<double, GraphItem> GraphMap;

using namespace QtCharts;

class kerrRotationGraph : public QChartView
{
    Q_OBJECT
public:
    explicit kerrRotationGraph(QWidget *parent = nullptr);

private:
    QLineSeries *m_series_p = new QLineSeries();
    QLineSeries *m_series_s = new QLineSeries();
    GraphMap m_graphMap;
    QChart *m_chart = new QChart();

    void addMapToSeries();
    void showChart();

signals:

public slots:
    void updateSeries(GraphMap graphMap);
};

#endif // KERRROTATIONGRAPH_H
