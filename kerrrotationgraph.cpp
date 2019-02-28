#include <utility>

#include "kerrrotationgraph.h"
#include <iostream>
#include <QValueAxis>

kerrRotationGraph::kerrRotationGraph(QWidget *parent) : QChartView(parent) {
  auto chartView = new QChartView(m_chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  this->setChart(m_chart);
  this->showChart();
}

void kerrRotationGraph::clear() {
  this->m_graphMap = GraphMap();
  m_chart->removeSeries(m_series_p);
  m_chart->removeSeries(m_series_s);
  this->addMapToSeries();
}

void kerrRotationGraph::addMapToSeries() {
  m_series_p = new QSplineSeries();
  m_series_s = new QSplineSeries();

  m_series_p->setName("Kerr Rotation P");
  m_series_s->setName("Kerr Rotation S");

  for (auto const &val : m_graphMap) {
    int indx = val.first * (180.0 / M_PI);
    double p = val.second.first.real() * (180.0 / M_PI);
    double s = val.second.second.real() * (180.0 / M_PI);
    QPointF point_s = QPointF(indx, s);
    QPointF point_p = QPointF(indx, p);

    if (m_series_p->points().indexOf(point_s) != -1) {
      m_series_p->remove(indx);
      m_series_s->remove(indx);
    }
    m_series_p->append(point_p); // p
    m_series_s->append(point_s); // s
  }
  this->showChart();
}

void kerrRotationGraph::showChart() {
  m_chart->addSeries(m_series_p);
  m_chart->addSeries(m_series_s);

  m_chart->createDefaultAxes();
  m_chart->axisX()->setTitleText("Angle of Incidence (Deg.)");
  m_chart->axisY()->setTitleText("Kerr Rotation (Deg.)");

  m_chart->setTitle("Kerr Rotation (Deg.) Longitudinal");

  this->show();
}

void kerrRotationGraph::updateSeries(GraphMap graphMap) {
  m_chart->removeSeries(m_series_p);
  m_chart->removeSeries(m_series_s);
  this->m_graphMap = std::move(graphMap);
  this->addMapToSeries();
}
