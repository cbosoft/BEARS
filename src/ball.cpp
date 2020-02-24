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
