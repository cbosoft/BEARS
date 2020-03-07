#include <iostream>
#include <cmath>
#include <cstring>

#include "sim.hpp"
#include "config.hpp"


int main(int argc, const char **argv)
{
  struct ConfigArgs args = parse_args(argc, argv);
  std::cerr << args.n << std::endl;
  Sim sim;
  init_config(sim, args);
  sim.save_to_file(args.path);
  std::cout << "done!" << std::endl;
}
