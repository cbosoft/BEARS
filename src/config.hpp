#pragma once

#include <string>

#include "sim.hpp"

struct ConfigArgs {
  int n; 
  double mass;
  double diameter;
  double separation;
  double roughness;
  double velocity_mag;
  double angular_velocity_mag;
  std::string path;
  std::string config_type;
};

void simple_cubic_monodisperse_random_velocity(Sim &sim, const struct ConfigArgs &args);
void interacting_pair(Sim &sim, const struct ConfigArgs &args);
struct ConfigArgs default_args();
struct ConfigArgs parse_args(int argc, const char **argv);
void init_config(Sim &sim, const struct ConfigArgs &args);
