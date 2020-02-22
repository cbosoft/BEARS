#include "ball.hpp"

Ball::Ball(struct BallConstructorData data)
{
  this->diameter = data.diameter;
  this->mass = data.mass;
  this->roughness = data.roughness;

  this->position = data.position;
  this->orientation = data.orientation;

  this->velocity = data.velocity;
  this->angular_velocity = data.angular_velocity;

  this->force = data.force;
  this->torque = data.torque;
}

Ball::~Ball()
{
}
