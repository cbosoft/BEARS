#pragma once

#include "vec.hpp"

class Ball;

class CollisionEvent {

  private:
    double time;
    Ball *balls[2];
    Vec image;

  public:

    CollisionEvent() =default;
    CollisionEvent(double t, Ball *a, Ball *b, Vec image)
    {
      this->time = t;
      this->balls[0] = a;
      this->balls[1] = b;
      this->image = image;
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

    Vec get_image() const
    {
      return this->image;
    }
};

typedef struct CollisionCheckResult {
  CollisionEvent *event;
  bool will_occur;
} CollisionCheckResult;
