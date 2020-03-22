#pragma once
#include <string>
#include <vector>

#include "vec.hpp"
#include "event.hpp"


struct BallConstructorData {
  uint32_t id;
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

class Sim;
class Ball {

  private:

    uint32_t id;

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

    EventCollection events;

    const Sim *parent;


  public:

    Ball(const struct BallConstructorData &bcd);
    Ball(const std::string s);
    ~Ball();

    std::string to_tsv() const;
    uint32_t *to_bin() const;
    std::string repr() const;

    void collide(Ball *other);
    void timejump(double time);

    void set_parent(Sim *parent);
    void set_image(const Vec &image);
    void enforce_bounds();

    double get_kinetic_energy() const;
    int get_id() const;

    static std::string tsv_headings();
    static uint32_t bin_nbytes();

    void update(CollisionEvent *ev);

    CollisionEvent *next();
    CollisionEvent *update_and_get_next(CollisionEvent *ev);
    CollisionEvent *check_will_collide_image(Ball *other, Vec image) const;
    CollisionEvent *check_will_collide_minimum_image(Ball *other, double L, double simtime) const;

};
