#pragma once

#include <vector>
#include <list>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"



class Sim {

  private:

    CollisionEvent * event;
    std::vector<Ball *> balls;
    double time;

    std::string trajectory_file_path;
    std::string configuration_file_path;

    void find_next_event();

  public:

    Sim() =default;
    Sim(std::string configuration_file_path);
    Sim(std::string configuration_file_path, std::string trajectory_file_path);
    ~Sim();

    void load_from_file(std::string path);
    void append_to_trajectory() const;
    void save_to_file(std::string path) const;
    void show_config() const;

    void add_ball(const struct BallConstructorData &bcd);

    void run(double end_time);

    Ball *get_ball(int index) const;

};
