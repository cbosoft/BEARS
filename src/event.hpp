#pragma once

#include "vec.hpp"

class Ball;

class CollisionEvent {

  private:
    double time;
    Vec positions[2];
    Ball *balls[2];

  public:

    CollisionEvent() =default;
    CollisionEvent(double t, Vec p1, Vec p2, Ball *a, Ball *b)
    {
      this->time = t;
      this->positions[0] = p1;
      this->positions[1] = p2;
      this->balls[0] = a;
      this->balls[1] = b;
    }

    double get_time() const
    {
      return this->time;
    }

    Ball *get_a() const
    {
      return this->balls[0];
    }

    Ball *get_b() const
    {
      return this->balls[1];
    }
};

typedef struct CollisionCheckResult {
  CollisionEvent *event;
  bool will_occur;
} CollisionCheckResult;
