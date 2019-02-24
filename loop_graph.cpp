#include <utility>

#include "loop_graph.h"
#include <iostream>
#include <QValueAxis>
#include <iostream>

Loop_graph::Loop_graph(QWidget *parent) : QChartView(parent) {
  auto chartView = new QChartView(m_chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  this->setChart(m_chart);
  this->showChart();
}

Loop_graph::~Loop_graph()
{
    delete m_coersivity;
    delete m_series_p;
    delete m_series_s;
    delete m_chart;
}

void Loop_graph::clear() {
  if (this->m_graphMap != nullptr) {
      delete this->m_graphMap;
  }

  m_chart->removeSeries(m_series_p);
  m_chart->removeSeries(m_series_s);
  this->addMapToSeries();
}

void Loop_graph::addMapToSeries() {
  m_series_p = new QSplineSeries();
  m_series_s = new QSplineSeries();

  m_series_p->setName("Kerr Rotation P");
  m_series_s->setName("Kerr Rotation S");

  for (auto const &val : *m_graphMap) {
    double indx = val.first;
    QPointF point_s = QPointF(indx, val.second.first);
    QPointF point_p = QPointF(indx, val.second.second);

    if (m_series_p->points().indexOf(point_s) != -1) {
      m_series_p->remove(indx);
      m_series_s->remove(indx);
    }
    m_series_p->append(point_p); // p
    m_series_s->append(point_s); // s
  }
  this->showChart();
}

void Loop_graph::showChart() {
  m_chart->addSeries(m_series_p);
  m_chart->addSeries(m_series_s);

  m_chart->createDefaultAxes();
  m_chart->axisX()->setTitleText("H");
  m_chart->axisY()->setTitleText("Kerr Signal");

  m_chart->setTitle("Kerr Rotation (Deg.) Longitudinal");

  this->show();
}

void Loop_graph::updateCoersivity(double coersivity)
{
    this->m_coersivity = &coersivity;
}

void Loop_graph::updateSeries(Eigen::Vector2cd Er)
{
    if (this->m_coersivity != nullptr) {

        if (this->m_graphMap != nullptr) {
            delete this->m_graphMap;
        }
        this->m_graphMap = new LoopGraphMap(); // clear or initialise the graph map

        double step = *m_coersivity / 100.0;
        double hValue = std::pow(Er.norm(), 2.0);

        for (double i = -(*m_coersivity * 3); i <= (*m_coersivity * 3); i += step) {
            auto yValues = LoopGraphItem((std::tanh(hValue - *this->m_coersivity)), std::tanh(hValue + *this->m_coersivity));
            this->m_graphMap->at(i) = yValues;
        }

        this->addMapToSeries();
    }
}

//void loop_graph::updateSeries(GraphMap graphMap) {
//  m_chart->removeSeries(m_series_p);
//  m_chart->removeSeries(m_series_s);
//  this->m_graphMap = std::move(graphMap);
//  this->addMapToSeries();
//}
