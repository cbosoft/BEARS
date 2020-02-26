#include <fstream>

#include "sim.hpp"
#include "exception.hpp"
#include "formatter.hpp"

Sim::~Sim()
{
  for (auto ball : this->balls) {
    delete ball;
  }
}

Sim::Sim()
{
  this->time = 0.0;
  this->nthreads = 1;
  this->config_file_path = "config.tsv";
  this->trajectory_file_path = "traj.yaml";
}



Sim::Sim(std::string config_file_path) : Sim()
{
  this->set_config_file(config_file_path);
  this->load_from_file(config_file_path);
}



Sim::Sim(std::string configuration_file_path, std::string trajectory_file_path) : Sim(configuration_file_path)
{
  this->set_trajectory_file(trajectory_file_path);
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


void Sim::set_config_file(std::string config_file_path)
{
  this->config_file_path = config_file_path;
}


void Sim::set_trajectory_file(std::string trajectory_file_path) 
{
  this->trajectory_file_path = trajectory_file_path;
  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "---" << std::endl;
}

void Sim::set_parallel(int nthreads)
{
  if (nthreads == 1) {
    throw ArgumentError("Don't use 'Sim::set_parallel()' with an argument of 1: use 'Sim::set_linear()'.");
  }
  else if (nthreads < 1) {
    throw ArgumentError("Invalid number of parallel threads selected (must be at least 2).");
  }

  this->nthreads = nthreads;
}
