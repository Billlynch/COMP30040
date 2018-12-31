#include "kerrrotationgraph.h"
#include <iostream>

kerrRotationGraph::kerrRotationGraph(QWidget *parent) : QChartView(parent)
{
    QChartView *chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->setChart(m_chart);
    this->showChart();
}

void kerrRotationGraph::addMapToSeries()
{
  m_series_p = new QSplineSeries();
  m_series_s = new QSplineSeries();


  for (auto const& val : m_graphMap)
  {
      m_series_p->append(val.first * (180.0/ M_PI), val.second.first.real() * (180.0/ M_PI)); // p
      m_series_s->append(val.first * (180.0/ M_PI), val.second.second.real()* (180.0/ M_PI)); // s
  }

  this->showChart();
}

void kerrRotationGraph::showChart()
{
    m_chart->addSeries(m_series_p);
    m_chart->addSeries(m_series_s);

    m_chart->createDefaultAxes();
    m_chart->setTitle("Kerr Rotation (Deg.) Longitudinal");

    this->show();
}

void kerrRotationGraph::updateSeries(GraphMap graphMap)
{
    m_chart->removeSeries(m_series_p);
    m_chart->removeSeries(m_series_s);
    std::cout << "got new graph map" << std::endl;
    this->m_graphMap = graphMap;
    this->addMapToSeries();
}
