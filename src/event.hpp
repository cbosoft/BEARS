#pragma once

#include <list>
#include <map>
#include <set>

#include "vec.hpp"

class Ball;

class CollisionEvent {

  private:
    double time;
    Ball *a, *b;
    Vec image;

  public:

    CollisionEvent() =default;
    CollisionEvent(double t, Ball *a, Ball *b)
    {
      this->time = t;
      this->a = a;
      this->b = b;
    }
    CollisionEvent(double t, Ball *a, Ball *b, Vec image) : CollisionEvent(t, a, b)
    {
      this->image = image;
    }

    double get_time() const
    {
      return this->time;
    }

    Ball *get_a() const
    {
      return this->a;
    }

    Ball *get_b() const
    {
      return this->b;
    }

    Vec get_image() const
    {
      return this->image;
    }

    std::pair<int, int> get_idpair() const;

};


inline bool event_compare_f(const CollisionEvent *a, const CollisionEvent *b)
{
  return a->get_time() < b->get_time();
}


class EventCollection {

  private:

    std::map<std::pair<int, int>, CollisionEvent *> events;

  public:

    EventCollection()
    {
    }

    ~EventCollection() 
    {
      for (auto kv : this->events) {
        delete kv.second;
      }
    }

    void push_back(CollisionEvent *event)
    {
      if (event == nullptr)
        return;

      auto pair = event->get_idpair();
      if (this->events.find(pair) != this->events.end())
        delete this->events[pair];
      this->events[pair] = event;
    }

    std::set<unsigned int> invalidate(CollisionEvent *ev);

    void merge(EventCollection &other)
    {
      this->events.merge(other.events);
    }

    std::list<CollisionEvent *> get_list() const
    {
      std::list<CollisionEvent *> rv;
      for (auto kv : this->events) {
        rv.push_back(kv.second);
      }
      return rv;
    }

    CollisionEvent *get_next_event() const
    {
      CollisionEvent *rv = nullptr;
      double event_time = -1;

      for (auto kv : this->events) {
        CollisionEvent *ev = kv.second;
        double t = ev->get_time();
        if ( (rv == nullptr) or (t < event_time) ) {
          rv = ev;
          event_time = t;
        }
      }

      return rv;
    }

    CollisionEvent *front()
    {
      auto rv = this->get_next_event();

      // remove from store
      if (rv)
        this->events.erase(this->events.find(rv->get_idpair()));

      // return value
      return rv;
    }

    size_t size() const
    {
      return this->events.size();
    }

};


