#include <iostream>
#include <cmath>
#include <cstring>

#include "sim.hpp"
#include "config.hpp"
#include "version.hpp"


int main(int argc, const char **argv)
{
  print_header("configurator");
  struct ConfigArgs args = parse_args(argc, argv);
  Sim sim;
  sim.set_config_file(args.path);
  init_config(sim, args);
  sim.save_config();
  std::cout << "done!" << std::endl;
}
