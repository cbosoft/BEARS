#include "ball.hpp"

double Ball::get_kinetic_energy() const
{
  double linear = this->velocity.dot(this->velocity) * this->mass * 0.5;
  double angular = this->angular_velocity.dot(this->angular_velocity) * this->inertia * 0.5;
  return linear + angular;
}
