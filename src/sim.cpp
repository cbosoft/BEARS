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
  this->trajectory_file_path = "traj.tsv";
  this->periodic_boundaries = true;
  this->set_side_length(1.0);
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
  size_t last_dot_index = this->config_file_path.rfind(".");
  size_t extension_length = this->config_file_path.size() - last_dot_index - 1;
  this->config_file_extension = this->config_file_path.substr(last_dot_index + 1, extension_length);
}


void Sim::set_trajectory_file(std::string trajectory_file_path) 
{
  this->trajectory_file_path = trajectory_file_path;
  size_t last_dot_index = this->trajectory_file_path.rfind(".");
  size_t extension_length = this->trajectory_file_path.size() - last_dot_index - 1;
  this->trajectory_file_extension = this->trajectory_file_path.substr(last_dot_index + 1, extension_length);
  this->init_trajectory();
}

void Sim::set_parallel(int nthreads)
{
  if (nthreads < 1) {
    throw ArgumentError("Invalid number of parallel threads selected (must be at least 1).");
  }

  this->nthreads = nthreads;
}

void Sim::set_side_length(double sl)
{
  this->side_length = sl;
  this->inv_side_length = 1.0/sl;

  for (int dx = -1, i = 0; dx < 2; dx++) {
    for (int dy = -1; dy < 2; dy++) {
      for (int dz = -1; dz < 2; dz++, i++) {
        this->images[i] = {dx*sl, dy*sl, dz*sl};
      }
    }
  }
}

double Sim::get_side_length() const
{
  return this->side_length;
}

double Sim::get_time() const
{
  return this->time;
}
