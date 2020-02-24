#include "ball.hpp"

Ball::Ball(const struct BallConstructorData &bcd)
{
  this->id = bcd.id;
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
  ss >> this->id;
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

std::string Ball::to_tsv() const
{
  std::stringstream ss;
  ss << this->id << "\t";
  ss << this->mass << "\t";
  ss << this->diameter << "\t";
  ss << this->roughness << "\t";
  ss << this->position.to_tsv() << "\t";
  ss << this->orientation.to_tsv() << "\t";
  ss << this->velocity.to_tsv() << "\t";
  ss << this->angular_velocity.to_tsv() << "\t";
  ss << this->force.to_tsv() << "\t";
  ss << this->torque.to_tsv() << "\t";
  return ss.str();
}

Ball::~Ball()
{
}

std::string Ball::repr() const
{
  std::stringstream ss;
  ss << "Ball(" 
    << this->id 
    << ", " << this->position.repr()
    << ")";
  return ss.str();
}
