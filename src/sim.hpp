#pragma once

#include <vector>
#include <list>
#include <array>
#include <set>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"


class Sim {

  private:

    std::list<CollisionEvent *> events;
    std::vector<Ball *> balls;
    double time;
    int nthreads;
    bool periodic_boundaries;
    double side_length;
    double inv_side_length;

    std::string trajectory_file_path;
    std::string trajectory_file_extension;
    std::string config_file_path;

    void parallel_update_events(std::set<unsigned int> invalid_indices);
    void linear_update_events(std::set<unsigned int> invalid_indices);
    void update_events();

    void init_trajectory_tsv() const;
    void append_to_trajectory_tsv() const;

    void init_trajectory_yaml() const;
    void append_to_trajectory_yaml() const;

  public:

    std::array<Vec, 27> images;

    Sim();
    Sim(std::string config_file_path);
    Sim(std::string config_file_path, std::string trajectory_file_path);
    ~Sim();

    void load_from_file(std::string path);

    void init_trajectory();
    void append_to_trajectory() const;
    void save_to_file(std::string path) const;
    void show_config() const;

    void add_ball(const struct BallConstructorData &bcd);

    void run(double end_time);

    Ball *get_ball(int index) const;

    void set_side_length(double sl);
    void set_parallel(int nthreads);
    void set_config_file(std::string s);
    void set_trajectory_file(std::string s);
    Vec enforce_bounds(const Vec &p1, const Vec &p2) const;
    Vec enforce_bounds(const Vec &p) const;

    double get_kinetic_energy() const;

};
