#pragma once

#include <list>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"

class Sim {

  private:

    std::list<CollisionEvent *> events;
    std::list<Ball *> balls;
    double time;

    std::string trajectory_file_path;
    std::string configuration_file_path;

    void update_events();
    void clear_events();

  public:

    Sim() =default;
    Sim(std::string configuration_file_path);
    Sim(std::string configuration_file_path, std::string trajectory_file_path);
    ~Sim();

    void load_from_file(std::string path);
    void append_to_file(std::string path) const;
    void save_to_file(std::string path) const;

    void run(double end_time);

};
