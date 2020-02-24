#include "ball.hpp"

Ball::Ball(const struct BallConstructorData &bcd)
{
  this->mass = bcd.mass;
  this->diameter = bcd.diameter;
  this->roughness = bcd.roughness;

  this->position = bcd.position;
  this->orientation = bcd.orientation;

  this->velocity = bcd.velocity;
  this->angular_velocity = bcd.angular_velocity;

  this->force = bcd.force;
  this->torque = bcd.torque;
}

Ball::Ball(const std::string s)
{
  std::stringstream ss(s);
  ss >> this->mass;
  ss >> this->diameter;
  ss >> this->roughness;
  
  double x, y, z;
  ss >> x; ss >> y; ss >> z;
  this->position = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->orientation = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->velocity = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->angular_velocity = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->force = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->torque = {x, y, z};
}

Ball::~Ball()
{
}
