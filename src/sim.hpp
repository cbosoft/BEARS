#pragma once

#include <vector>
#include <list>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"



class Sim {

  private:

    std::list<CollisionEvent *> events;
    std::vector<Ball *> balls;
    double time;
    int nthreads;

    std::string trajectory_file_path;
    std::string configuration_file_path;

    void parallel_update_events();
    void linear_update_events();
    void update_events();
    void clear_events();

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

    void set_parallel(int nthreads);
};
