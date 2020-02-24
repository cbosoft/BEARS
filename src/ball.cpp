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

std::string Ball::to_ssv() const
{
  std::stringstream ss;
  ss << this->mass << " ";
  ss << this->diameter << " ";
  ss << this->roughness << " ";
  ss << this->position.to_ssv() << " ";
  ss << this->orientation.to_ssv() << " ";
  ss << this->velocity.to_ssv() << " ";
  ss << this->angular_velocity.to_ssv() << " ";
  ss << this->force.to_ssv() << " ";
  ss << this->torque.to_ssv() << " ";
  return ss.str();
}

Ball::~Ball()
{
}
