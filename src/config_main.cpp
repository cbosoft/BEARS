#include <iostream>
#include <cmath>
#include <cstring>

#include "sim.hpp"
#include "config.hpp"
#include "version.hpp"


int main(int argc, const char **argv)
{
  struct ConfigArgs args = parse_args(argc, argv);
  Sim sim;
  print_header("configurator");
  init_config(sim, args);
  sim.save_to_file(args.path);
  std::cout << "done!" << std::endl;
}
