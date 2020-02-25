#pragma once
#include <string>

#include "vec.hpp"
#include "event.hpp"


struct BallConstructorData {
  int id;
  double diameter;
  double mass;
  double inertia;
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
    double inertia;
    double diameter;


    Ball(const struct BallConstructorData &bcd);
    Ball(const std::string s);
    ~Ball();

    std::string to_tsv() const;
    std::string to_yaml() const;
    std::string to_yaml(int indent) const;
    std::string repr() const;

    void collide(Ball *other);
    void timejump(double time);

    CollisionCheckResult *check_will_collide(Ball *other) const;
};
