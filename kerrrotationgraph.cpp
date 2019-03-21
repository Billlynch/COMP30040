#include "kerrrotationgraph.h"

/*!
 * \brief kerrRotationGraph::kerrRotationGraph
 * \param parent (window to show in)
 *
 * This sets up the margins and chart type for the chart.
 */
kerrRotationGraph::kerrRotationGraph(QWidget *parent) : QChartView(parent) {
  auto chartView = new QChartView(m_chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  this->setChart(m_chart);
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setBackgroundRoundness(0);
  this->showChart();
}

/*!
 * \brief kerrRotationGraph::addMapToSeries
 * This takes the graph map object and adds them to two QT series objects to be
 * shown in the chart. It also sets the names for the series, then calls showChart()
 */
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

/*!
 * \brief kerrRotationGraph::showChart
 * This adds the series to the axis on the chart and shows it.
 */
void kerrRotationGraph::showChart() {
  m_chart->addSeries(m_series_p);
  m_chart->addSeries(m_series_s);

  m_chart->createDefaultAxes();
  m_chart->axisX()->setTitleText("Angle of Incidence (Deg.)");
  m_chart->axisY()->setTitleText("Kerr Rotation (Deg.)");

  this->show();
}

/*!
 * \brief kerrRotationGraph::updateSeries
 * \param graphMap - the data to be shown on the chart.
 *
 * This is called whenever a new ray collides with the sample.
 */

void kerrRotationGraph::updateSeries(GraphMap graphMap) {
  m_chart->removeSeries(m_series_p);
  m_chart->removeSeries(m_series_s);
  this->m_graphMap = std::move(graphMap);
  this->addMapToSeries();
}
