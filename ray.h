#include <eigen3/Eigen/Core>
#include <vector>
#include <utility>

#ifndef RAY_H
#define RAY_H
typedef Eigen::Matrix<std::complex<double>, 2, 1> Vector2cd;
typedef std::vector<Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2>>
    ListMatrix4cd;
typedef Eigen::Matrix<std::complex<double>, 2, 2, 0, 2, 2> Matrix4cd;

class Ray {
  Eigen::Vector3d m_origin;
  Eigen::Vector3d m_direction;
  Matrix4cd m_polarisation;
  Matrix4cd m_calculationMatrix;
  Eigen::Vector2d m_targetPolarisation;

public:
  Ray(Eigen::Vector3d origin, Eigen::Vector3d direction, Matrix4cd polarisation,
      Eigen::Vector2d targetPolarisation);

  ~Ray();

  Eigen::Vector3d getOrigin();
  Eigen::Vector3d getDirection();
  Matrix4cd getPolarisation();
  Matrix4cd getCalculationMatrix();
  double getWaveLength();

  void setOrigin(Eigen::Vector3d origin);
  void setDirection(Eigen::Vector3d direction);
  void setPolarisation(Matrix4cd polarisation);

  void calculationMatrixMultiplication(Matrix4cd &x);

  void flipPolarization();
};
#endif // RAY_H
