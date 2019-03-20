#include "randomnoisechartview.h"

/*!
 * \brief RandomNoiseChartView::populateChart
 * This adds the default curve into the bar chart, the overloaded method will
 * add useful points into it.
 */
void RandomNoiseChartView::populateChart() {
  m_points->append(QPointF(-5, 0));
  m_points->append(QPointF(0, 5));
  m_points->append(QPointF(5, 0));
  m_lineSeries->append(*m_points);
}

/*!
 * \brief RandomNoiseChartView::populateChart
 * \param d
 * \param gen
 * This is the overloaded version. It chooes 10000 random values and plot them
 * on the graph and put the distrobution of those into the points list.
 */
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

/*!
 * \brief RandomNoiseChartView::render
 * This adds the series to the chart and gets up the axis and lables.
 * We then show the chart. This is called internally not from external places.
 */
void RandomNoiseChartView::render() {
  m_chart->addSeries(m_lineSeries);
  m_chart->createDefaultAxes();
  m_chart->axisX()->setRange(-5, 5);
  m_chart->axisX()->setTitleText("Noise Multiplier");
  m_chart->axisY()->setTitleText("Prob... ");

  m_chart->legend()->hide();
  this->setChart(m_chart);
  this->show();
}

/*!
 * \brief RandomNoiseChartView::RandomNoiseChartView
 * \param parent
 * \param name
 * This is the construtor, it builds a default distrobution to display and sets up the
 * chart's display params.
 */
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

/*!
 * \brief RandomNoiseChartView::newRandomGenerator
 * \param d
 * \param gen
 *
 * This is called whenever a slider is changed on the UI relating to this instance of the
 * generator. We first clear the old point and then using the new generator and distrobution
 * plot a new chart and show that.
 */
void RandomNoiseChartView::newRandomGenerator(std::normal_distribution<> d,
                                              std::mt19937 gen) {
  this->m_lineSeries->clear();
  this->m_points->clear();
  this->m_chart->removeSeries(m_lineSeries);

  this->populateChart(d, gen);
  this->render();
}
