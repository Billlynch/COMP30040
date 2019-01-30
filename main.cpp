#include <QApplication>
#include <mokelasersim.h>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MOKELaserSim w;

  w.show();

  return a.exec();
}
