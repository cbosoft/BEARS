#pragma once
#include <string>

#include "vec.hpp"

struct BallConstructorData {
  int id;
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

    int id;

    Vec position;
    Vec orientation;

    Vec velocity;
    Vec angular_velocity;

    Vec force;
    Vec torque;

    double roughness;
    double mass;
    double diameter;


    Ball(const struct BallConstructorData &bcd);
    Ball(const std::string s);
    ~Ball();

    std::string to_tsv() const;
    std::string to_yaml() const;
    std::string to_yaml(int indent) const;
    std::string repr() const;

};
