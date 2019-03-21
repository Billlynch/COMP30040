#include "mokelasersim.h"
#include "ui_mokelasersim.h"

/*!
 * \brief MOKELaserSim::MOKELaserSim
 * \param parent
 *
 * This sets up the main UI and event listener system for the application.
 */
MOKELaserSim::MOKELaserSim(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MOKELaserSim) {
  ui->setupUi(this);
  resultsWindow = new PolarisationWindow(ui->polarisationVisualisation);

  /*!
   * Allow the custom types defined in the system to be linked in the complier.
   */
  qRegisterMetaType<ListVector2cd>("ListVector2cd");
  qRegisterMetaType<ListMatrix4cd>("ListMatrix4cd");

  // connnect the completion of one ray from laser to analyser to the results window. (The output polarisation diagram).
  connect(&simThread, &SimulationThread::simComplete, resultsWindow,
          &PolarisationWindow::simResultsUpdated);

  // setup the event loop timer - this is the timer which sets off the rays roughly every 2ms. And link it to the fire ray method in the sim.
  eventLoopTimer = new QTimer(this);
  connect(eventLoopTimer, &QTimer::timeout, &simThread,
          &SimulationThread::fireNextRay);
  eventLoopTimer->setInterval(2);

  // setup the pem time timer. Every time this is incremented the 'time' in the PEM is also increased.
  pemTimer = new QTimer(this);
  connect(pemTimer, &QTimer::timeout, &simThread,
          &SimulationThread::incrementPEMTimeProgression);
  pemTimer->setInterval(1);

  // setup the noise generator for the laser.
  randomGenerator_laser = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator_laser, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->graphicsView, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator_laser, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &simThread, &SimulationThread::newLaserNoise); // connect thr noise output to the simulation thread.
  randomGenerator_laser->generate();

  // setup the noise generator for the PEM
  randomGenerator_pem = new RandomNoiseCalculator(0, 1);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          ui->noise_pem, &RandomNoiseChartView::newRandomGenerator);
  connect(randomGenerator_pem, &RandomNoiseCalculator::newRandomNoiseGeneration,
          &simThread, &SimulationThread::newPemNoise); // connect this to the simulation thread.
  randomGenerator_pem->generate();

  connect(this, &MOKELaserSim::newCameraLocation, ui->threeDVis,
          &ThreeDimentionalVisualisation::newCameraPostion); // connect the emission of a new camera location to the 3D view.
  connect(this, &MOKELaserSim::laserNoiseStateChanged, &simThread,
          &SimulationThread::newLaserNoiseState); // connect the emission of the laser state checkbox change to the simulation thread.
  connect(this, &MOKELaserSim::newPemState, &simThread,
          &SimulationThread::newPemState); // connect the emission of the pem enable/disabled state to teh simulation thread.
  connect(this, &MOKELaserSim::newPolariserState, &simThread,
          &SimulationThread::newPolariserState); // connect the emission of the polariser state to the simulation thread.
  connect(this, &MOKELaserSim::newPEMNoiseState, &simThread,
          &SimulationThread::newPemNoiseState); // connect the emission of a new PEM noise state to the simulation thread.

  connect(this, &MOKELaserSim::newPolariserState, ui->threeDVis,
          &ThreeDimentionalVisualisation::newPolariserState); // connect the enabled/disabled state of the polariser to the 3D view.

  connect(this, &MOKELaserSim::newPemState, ui->threeDVis,
          &ThreeDimentionalVisualisation::newPemState); // connect the enabled/disabled state of the PEM to the 3D view.

  connect(&simThread, &SimulationThread::emittedNewRayFromLaser, ui->threeDVis,
          &ThreeDimentionalVisualisation::newOutputFromLaser); // connect the emission of a 'emittedNewRayFromLaser' to the 3D view.

  connect(this, &MOKELaserSim::newHValue, &simThread,
          &SimulationThread::newHValue); // connect the change of the H value slider the the simulation thread.

  connect(this, &MOKELaserSim::newPolarisationTarget, &simThread,
          &SimulationThread::newPolarisationTarget); // connect the change of the target polarisation for the polariser to the simulation thread.

  connect(this, &MOKELaserSim::newCoersivity, ui->loop_graph,
          &Loop_graph::updateCoersivity); // connect the coersivity update to the hysteresis loop graph.
}

/*!
 * \brief MOKELaserSim::~MOKELaserSim
 * clear memory on class delete.
 */
MOKELaserSim::~MOKELaserSim() {
  delete resultsWindow;
  delete ui;
}

/*!
 * \brief MOKELaserSim::on_RunSimButton_clicked
 * This starts the timers for the event loop and pem time going and starts the simulation.
 */
void MOKELaserSim::on_RunSimButton_clicked() {
  simThread.simulate(ui->Q_r_input->value(), ui->Q_i_input->value(),
                  ui->n0_r_input->value(), ui->n0_i_input->value(),
                  *ui->kerrGraph, *ui->threeDVis, *ui->loop_graph);
  pemTimer->start();
  eventLoopTimer->start();
}

/*!
 * \brief MOKELaserSim::on_StopSimButton_clicked
 * This This stops the timers and the simulation thread.
 */
void MOKELaserSim::on_StopSimButton_clicked() {
  pemTimer->stop();
  eventLoopTimer->stop();
  simThread.safeAbort();
}

/*!
 * \brief MOKELaserSim::on_speedSlider_valueChanged
 * \param value - the value from the slider
 * This multiplies the base values for the time in ms to wait until triggering the time for the PEM and
 * the emission of the next ray by the value of the sider. slowing down the simulation, but not adjusting
 * the times in which the ray will collide with the PEM.
 */
void MOKELaserSim::on_speedSlider_valueChanged(int value) {
  pemTimer->setInterval(1 * value);
  eventLoopTimer->setInterval(2 * value);
}

/*!
 * \brief MOKELaserSim::on_angle_of_incidence_valueChanged
 * \param angle
 * sends the new angle of incidence to the simulation thread.
 */
void MOKELaserSim::on_angle_of_incidence_valueChanged(int angle) {
  if (simThread.isRunning()) {
    simThread.angleOfIncidenceChanged(static_cast<double>(angle));
    this->ui->currentAngle->display(angle);
  }
}

/*!
 * \brief MOKELaserSim::on_sample_mean_valueChanged
 * \param value
 * Set the mean for the noise of the laser based from the slider position.
 */
void MOKELaserSim::on_sample_mean_valueChanged(int value) {
  this->randomGenerator_laser->setMean(value / 100.0);
}

/*!
 * \brief MOKELaserSim::on_sample_deviation_valueChanged
 * \param value
 * Set the deviation of the noise for the laser based on the slider position.
 */
void MOKELaserSim::on_sample_deviation_valueChanged(int value) {
  this->randomGenerator_laser->setDeviation(value / 100.0);
}

/*!
 * \brief MOKELaserSim::on_noise_chk_stateChanged
 * \param state
 * Turn the noise on or off for the laser.
 */
void MOKELaserSim::on_noise_chk_stateChanged(int state) {
  emit laserNoiseStateChanged(state);
}

/*!
 * \brief MOKELaserSim::on_centre_view_clicked
 * Tell the 3D view to reset the 3D view to the centre.
 */
void MOKELaserSim::on_centre_view_clicked() {
  emit newCameraLocation(ObjectType::centre);
}

/*!
 * \brief MOKELaserSim::on_laser_view_clicked
 * Tell the 3D view to centre on the laser
 */
void MOKELaserSim::on_laser_view_clicked() {
  emit newCameraLocation(ObjectType::laser);
}

/*!
 * \brief MOKELaserSim::on_polariser_view_clicked
 * Tell the 3D view to centre on the polarising filter.
 */
void MOKELaserSim::on_polariser_view_clicked() {
  emit newCameraLocation(ObjectType::polarFilter);
}

/*!
 * \brief MOKELaserSim::on_sample_view_clicked
 * Tell the 3D view to centre on the sample.
 */
void MOKELaserSim::on_sample_view_clicked() {
  emit newCameraLocation(ObjectType::sample);
}

/*!
 * \brief MOKELaserSim::on_PEM_view_clicked
 * Tell the 3D view to cnetre on the PEM
 */
void MOKELaserSim::on_PEM_view_clicked() {
  emit newCameraLocation(ObjectType::pem);
}

/*!
 * \brief MOKELaserSim::on_Analysier_view_clicked
 * Tell the 3D view to centre on the analyser
 */
void MOKELaserSim::on_Analysier_view_clicked() {
  emit newCameraLocation(ObjectType::analyser);
}

/*!
 * \brief MOKELaserSim::on_threeDEnable_chk_stateChanged
 * \param state
 * Enable or disable the 3D view
 */
void MOKELaserSim::on_threeDEnable_chk_stateChanged(int state) {
  this->ui->threeDVis->setEnabledState(state);
}

/*!
 * \brief MOKELaserSim::on_polariserEnable_chk_stateChanged
 * \param state
 * Enable or disable the polariser diagram drawing.
 */
void MOKELaserSim::on_polariserDisplayEnable_chk_stateChanged(int state) {
  this->resultsWindow->setEnabledState(state);
}

/*!
 * \brief MOKELaserSim::on_pem_enabled_chk_stateChanged
 * \param state
 * Enable or disable the PEM.
 */
void MOKELaserSim::on_pem_enabled_chk_stateChanged(int state) {
  emit newPemState(state);
}

/*!
 * \brief MOKELaserSim::on_deviation_pem_valueChanged
 * \param value
 * Set the deviation for the PEM noise based on the position of the slider.
 */
void MOKELaserSim::on_deviation_pem_valueChanged(int value) {
  this->randomGenerator_pem->setDeviation(value / 100.0);
}

/*!
 * \brief MOKELaserSim::on_mean_pem_valueChanged
 * \param value
 * Set the mean of the noise for the PEM based on the position of the slider.
 */
void MOKELaserSim::on_mean_pem_valueChanged(int value) {
  this->randomGenerator_pem->setMean(value / 100.0);
}

/*!
 * \brief MOKELaserSim::on_polar_enabled_chk_stateChanged
 * \param state
 * Enable or disable the Polariser.
 */
void MOKELaserSim::on_polar_enabled_chk_stateChanged(int state) {
  emit newPolariserState(state);
}

/*!
 * \brief MOKELaserSim::on_H_slider_valueChanged
 * \param value
 * Set the new H value based on the slider.
 */
void MOKELaserSim::on_H_slider_valueChanged(int value) {
  emit newHValue(value / 10.0);
}

/*!
 * \brief MOKELaserSim::displayPolarVector
 * \param target - the Vector for the target angle of the polariser.
 * This takes the target vector and display the normalised s and p values in the UI.
 */
void MOKELaserSim::displayPolarVector(Eigen::Vector2d &target)
{
    target.normalize();
    std::stringstream ss;
    ss << target.x();
    this->ui->polar_s_normalised->setText(QString::fromStdString(ss.str()));
    ss.str(std::string());
    ss << target.y();
    this->ui->polar_p_normalised->setText(QString::fromStdString(ss.str()));
}

/*!
 * \brief MOKELaserSim::on_polar_direction_valueChanged
 * \param value - the value of the dial
 * For this we first need to convert the int value from the dial to an angle.
 * The value at the top centre of the dial is 0 degrees (visually but not in reality, in reality the 0 angle position is down).
 * The initial conversion is easy and is displayed on the ui at the top of this method.
 *
 * We however need this in vector form. To make the angle I am using the angle between the selected angle
 * on the dial and the down direction. To do this we use trigonometry. We emit this vector to the polariser.
 */
void MOKELaserSim::on_polar_direction_valueChanged(int value) {
  this->ui->polar_angle->setText(QString::fromStdString(std::to_string((value + 180) % 360)));
  auto angle = static_cast<double>(value * M_PI / 180.0);

  Eigen::Matrix<double, 2, 2> converter;
  converter << std::cos(angle), std::sin(angle), -std::sin(angle),
      std::cos(angle);

  auto downVector = Eigen::Vector2d(0, -1);

  Eigen::Vector2d target = converter * downVector;

  emit newPolarisationTarget(target);

  displayPolarVector(target);
}

/*!
 * \brief MOKELaserSim::on_doubleSpinBox_valueChanged
 * \param val
 * Tell the hysteresis loop graph the new coersivity based on the slider.
 */
void MOKELaserSim::on_doubleSpinBox_valueChanged(double val) {
  emit newCoersivity(val);
}
