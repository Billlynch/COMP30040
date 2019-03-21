#include "Loop_graph.h"

/*!
 * \brief kerrRotationGraph::kerrRotationGraph
 * \param parent (window to show in)
 *
 * This sets up the margins and chart type for the chart.
 */
Loop_graph::Loop_graph(QWidget *parent) : QChartView(parent) {
  auto chartView = new QChartView(m_chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setBackgroundRoundness(0);
  this->setChart(m_chart);
  this->showChart();
}

/*!
 * \brief Loop_graph::~Loop_graph
 * clear the memory on destruction of this object.
 */
Loop_graph::~Loop_graph() {
  delete m_chart;
  delete m_coersivity;
  delete m_series_increase;
  delete m_series_decrease;
}

/*!
 * \brief Loop_graph::addMapToSeries
 * this converts the data in the graph map into series for the QT chart
 * library to understand. It makes two series so that two lines are produced
 * on the same x position.
 */
void Loop_graph::addMapToSeries() {
  delete m_series_increase;
  delete m_series_decrease;

  m_series_increase = new QLineSeries();
  m_series_decrease = new QLineSeries();

  for (auto const &val : *m_graphMap) {
    double indx = val.first;
    QPointF point_s = QPointF(indx, val.second.first);
    QPointF point_p = QPointF(indx, val.second.second);

    m_series_increase->append(point_p); // p
    m_series_decrease->append(point_s); // s
  }

  this->showChart();
}

/*!
 * \brief Loop_graph::showChart
 * This adds the series to the chart and sets the axis. Plus the axis labels.
 */
void Loop_graph::showChart() {
  m_chart->addSeries(this->m_series_increase);
  m_chart->addSeries(this->m_series_decrease);
  m_chart->legend()->setVisible(false);
  this->m_chart->createDefaultAxes();
  m_chart->axisX()->setTitleText("H");
  m_chart->axisY()->setTitleText("Kerr Signal");
  this->show();
}

/*!
 * \brief Loop_graph::updateCoersivity
 * \param coersivity
 *
 * This is called whenever the coersivity is changed in the UI. This adjusts the
 * x offset for the tanh lines on the series, which is done on the next update of the series.
 */
void Loop_graph::updateCoersivity(double coersivity) {
  this->m_coersivity = new double(coersivity);
}


/*!
 * \brief Loop_graph::updateSeries
 * \param Er
 *
 * called everytime a new ray hits the analyser. This ray is the latest ray.
 *
 * This calculates the lines to plot on the chart. It uses the coersivity set from updateCoersivity.
 * It takes in the latest Er (polarisation in s and p planes) of the lastest ray.
 *
 * Works by setting the scale of the x axis to be -3 * coersivity to +3 * coersivity. Then steps through
 * this range and generates the value of tanh for both lines at this point. With the hValue as a multiplier,
 * hValue being the applied magnetic force. This information is then saved into a map which is converted to
 * line series in addMapToSeries().
 */
void Loop_graph::updateSeries(const Eigen::Vector2cd Er) {
  if (this->m_coersivity != nullptr && *this->m_coersivity != 0.0) {
    this->m_graphMap->clear();

    double step = *m_coersivity / 100.0;
    double hValue = std::pow(Er.norm(), 2.0);

    for (double i = -(*m_coersivity * 3); i <= (*m_coersivity * 3); i += step) {
      auto yValues =
          LoopGraphItem((hValue * std::tanh(i - *this->m_coersivity)),
                        (hValue * std::tanh(i + *this->m_coersivity)));
      this->m_graphMap->insert(std::pair<double, LoopGraphItem>(i, yValues));
    }

    this->addMapToSeries();
  }
}
