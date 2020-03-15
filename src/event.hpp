#pragma once

#include <list>

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

class EventCollection {

  private:

    std::list<CollisionEvent *> events;

  public:

    EventCollection() { }

    void add_event(CollisionEvent *event)
    {
      if (this->events.size() and (event->get_time() < events.front()->get_time())) {
        this->events.push_front(event);
      }
      else {
        this->events.push_back(event);
      }
    }

    const std::list<CollisionEvent *> &get_list() const
    {
      return this->events;
    }
};


inline bool event_compare_f(const CollisionEvent *a, const CollisionEvent *b)
{
  return a->get_time() < b->get_time();
}

