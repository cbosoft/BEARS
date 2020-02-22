#pragma once
#include <vector>

struct BallConstructorData {
  double diameter;
  double mass;
  double roughness;
  std::vector<double> position;
  std::vector<double> orientation;
  std::vector<double> velocity;
  std::vector<double> angular_velocity;
  std::vector<double> force;
  std::vector<double> torque;
};

class Ball {

  //private:
  public:

    std::vector<double> position;
    std::vector<double> orientation;

    std::vector<double> velocity;
    std::vector<double> angular_velocity;

    std::vector<double> force;
    std::vector<double> torque;

    double roughness;
    double mass;
    double diameter;


    Ball(struct BallConstructorData);
    ~Ball();

};
