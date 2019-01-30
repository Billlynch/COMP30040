#ifndef RANDOMNOISECHARTVIEW_H
#define RANDOMNOISECHARTVIEW_H

#include <QWidget>
#include <QChart>
#include <QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QLineSeries>
#include <complex>
#include <map>
#include <QList>
#include <QPointF>
#include <iostream>
#include <string>
#include <random>
#include <cmath>
#include <map>
#include <QValueAxis>


using namespace QtCharts;

class RandomNoiseChartView : public QChartView
{
    Q_OBJECT
private:
    std::string name;
    QList<QPointF> *m_points;
    QSplineSeries *m_lineSeries;
    QChart *m_chart;
    QString m_chartTitle;

    void populateChart();
    void populateChart(std::normal_distribution<> d, std::mt19937 gen);

    void render();
public:
    RandomNoiseChartView(QWidget *parent = nullptr, const std::string& name = "");
    void setTitle(const std::string& title);

public slots:
    void newRandomGenerator(std::normal_distribution<>, std::mt19937);

};

#endif // RANDOMNOISECHARTVIEW_H
