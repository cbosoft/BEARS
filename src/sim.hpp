#pragma once

#include <list>
#include <array>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"

class Sim {

  private:

    std::list<CollisionEvent *> events;
    std::list<Ball *> balls;
    double time;

    void update_events();

  public:

    Sim() =default;
    ~Sim() =default;
    void load_from_file(std::string path);
    void append_to_file(std::string path) const;

    void run(double end_time);

};
