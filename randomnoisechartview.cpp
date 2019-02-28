#include "randomnoisechartview.h"
#include <iostream>

void RandomNoiseChartView::populateChart() {
  m_points->append(QPointF(-5, 0));
  m_points->append(QPointF(0, 5));
  m_points->append(QPointF(5, 0));
  m_lineSeries->append(*m_points);
}

void RandomNoiseChartView::populateChart(std::normal_distribution<> d,
                                         std::mt19937 gen) {

  std::map<int, int> hist{};
  for (int n = 0; n < 10000; ++n) {
    ++hist[std::round(d(gen))];
  }
  for (auto p : hist) {
    m_points->append(QPointF(p.first, (p.second / 200.0)));
  }

  m_lineSeries->append(*m_points);
}

void RandomNoiseChartView::render() {
  m_chart->addSeries(m_lineSeries);
  m_chart->createDefaultAxes();
  // m_chart->axisY()->setMax(20);
  m_chart->axisX()->setRange(-5, 5);

  m_chart->legend()->hide();
  this->setChart(m_chart);
  //  this->chart()->setPlotArea(area);
  this->show();
}

RandomNoiseChartView::RandomNoiseChartView(QWidget *parent,
                                           const std::string &name)
    : QChartView(parent) {
  m_chart = new QChart();
  m_lineSeries = new QSplineSeries();
  m_points = new QList<QPointF>();

  this->setRenderHint(QPainter::Antialiasing);
  m_chart->setMargins(QMargins(0,0,0,0));
  m_chart->setBackgroundRoundness(0);
  QRect area = QRect(0, 0, 256, 128);

  this->setSceneRect(area);
}


void RandomNoiseChartView::newRandomGenerator(std::normal_distribution<> d,
                                              std::mt19937 gen) {
  this->m_lineSeries->clear();
  this->m_points->clear();
  this->m_chart->removeSeries(m_lineSeries);

  this->populateChart(d, gen);
  this->render();
}
