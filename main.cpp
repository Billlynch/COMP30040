#include <QApplication>
#include <mokelasersim.h>
/*!
 * \brief main
 * \param argc - unused
 * \param argv - unused
 * \return the return code of the program.
 *
 * This just opens the window and the rest of the application is event based from the UI.
 */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MOKELaserSim w;

  w.show();

  return a.exec();
}
