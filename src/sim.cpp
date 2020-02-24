#include "sim.hpp"

Sim::~Sim()
{
  for (auto ball : this->balls) {
    delete ball;
  }
}



Sim::Sim(std::string configuration_file_path)
{
  this->configuration_file_path = configuration_file_path;
  this->load_from_file(configuration_file_path);
}



Sim::Sim(std::string configuration_file_path, std::string trajectory_file_path) : Sim(configuration_file_path)
{
  this->trajectory_file_path = trajectory_file_path;
}


void Sim::add_ball(const struct BallConstructorData &bcd)
{
  Ball *ball = new Ball(bcd);
  this->balls.push_back(ball);
}


Ball *Sim::get_ball(int i) const
{
  return this->balls[i];
}
