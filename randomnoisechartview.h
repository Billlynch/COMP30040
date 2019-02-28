#ifndef RANDOMNOISECHARTVIEW_H
#define RANDOMNOISECHARTVIEW_H

#include <QChart>
#include <QLineSeries>
#include <QList>
#include <QPointF>
#include <QSplineSeries>
#include <QValueAxis>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <cmath>
#include <complex>
#include <iostream>
#include <map>
#include <map>
#include <random>
#include <string>

using namespace QtCharts;

class RandomNoiseChartView : public QChartView {
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
  RandomNoiseChartView(QWidget *parent = nullptr, const std::string &name = "");

public slots:
  void newRandomGenerator(std::normal_distribution<>, std::mt19937);
};

#endif // RANDOMNOISECHARTVIEW_H
