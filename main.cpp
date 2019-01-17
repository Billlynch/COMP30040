#include <mokelasersim.h>
#include <QApplication>

int main(int argc, char* argv[]) {
  glutInit( & argc, argv );
  QApplication a(argc, argv);
  MOKELaserSim w;

  w.show();

  return a.exec();
}
