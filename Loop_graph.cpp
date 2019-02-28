#include <utility>

#include "Loop_graph.h"
#include <iostream>
#include <QValueAxis>
#include <iostream>

Loop_graph::Loop_graph(QWidget *parent) : QChartView(parent) {
  auto chartView = new QChartView(m_chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  m_chart->setMargins(QMargins(0,0,0,0));
  m_chart->setBackgroundRoundness(0);
  this->setChart(m_chart);
  this->showChart();
}

Loop_graph::~Loop_graph()
{
        delete m_chart;
        delete m_coersivity;
        delete m_series_increase;
        delete m_series_decrease;
}

void Loop_graph::clear() {
  if (this->m_graphMap != nullptr) {
      delete this->m_graphMap;
  }

  m_chart->removeSeries(m_series_increase);
  m_chart->removeSeries(m_series_decrease);
  this->addMapToSeries();
}

void Loop_graph::addMapToSeries() {
//  this->m_chart->removeSeries(m_series_p);
//  this->m_chart->removeSeries(m_series_s);

    //this->m_chart->removeAllSeries();
  delete m_series_increase;
  delete m_series_decrease;

  m_series_increase = new QLineSeries();
  m_series_increase->setName("KerrSignal increase");
  m_series_decrease = new QLineSeries();
  m_series_decrease->setName("KerrSignal decrease");


  for (auto const &val : *m_graphMap) {
    double indx = val.first;
    QPointF point_s = QPointF(indx, val.second.first);
    QPointF point_p = QPointF(indx, val.second.second);

    m_series_increase->append(point_p); // p
    m_series_decrease->append(point_s); // s
  }

  this->showChart();
}

void Loop_graph::showChart() {
    m_chart->addSeries(this->m_series_increase);
    m_chart->addSeries(this->m_series_decrease);
    this->m_chart->createDefaultAxes();
    m_chart->axisX()->setTitleText("H");
    m_chart->axisY()->setTitleText("Kerr Signal");
    this->show();
}

void Loop_graph::updateCoersivity(double coersivity)
{
    this->m_coersivity = new double(coersivity);
}

void Loop_graph::updateSeries(const Eigen::Vector2cd Er)
{
    if (this->m_coersivity != nullptr && *this->m_coersivity != 0.0) {
        this->m_graphMap->clear();

        double step = *m_coersivity / 100.0;
        double hValue = std::pow(Er.norm(), 2.0);

        for (double i = -(*m_coersivity * 3); i <= (*m_coersivity * 3); i += step) {
            auto yValues = LoopGraphItem((hValue * std::tanh(i - *this->m_coersivity)),(hValue * std::tanh(i + *this->m_coersivity)));
            this->m_graphMap->insert(std::pair<double, LoopGraphItem>(i, yValues));
        }

        this->addMapToSeries();
     }
}

