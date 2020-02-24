#pragma once
#include <string>
#include "vec.hpp"

struct BallConstructorData {
  double diameter;
  double mass;
  double roughness;
  Vec position;
  Vec orientation;
  Vec velocity;
  Vec angular_velocity;
  Vec force;
  Vec torque;
};

class Ball {

  //private:
  public:

    Vec position;
    Vec orientation;

    Vec velocity;
    Vec angular_velocity;

    Vec force;
    Vec torque;

    double roughness;
    double mass;
    double diameter;


    Ball() =default;
    Ball(struct BallConstructorData);
    Ball(const std::string s);
    ~Ball();

};
