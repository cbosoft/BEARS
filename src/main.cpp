#include <iostream>
#include <cstring>

#include "sim.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{

  struct {
    std::string config_file_path;
    double end_time;
  } args = {
    "config.ssv", 
    10.0
  };

  // TODO: improve arg parsing
  // TODO: check given path exists and is an ssv
  --argc; ++argv;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-c", "--config-file")) {
      args.config_file_path = std::string(argv[++i]);
    }
    else if (EITHER("-t", "--end-time")) {
      args.end_time = atof(argv[++i]);
    }
    else if (EITHER("-e", "--endless")) {
      args.end_time = -1;
    }
    else {
      std::cerr << "unknown arg: " << argv[i] << std::endl;
      exit(1);
    }
  }

  std::cerr << "reading config" << std::endl;
  Sim sim(args.config_file_path);

  std::cerr << "begin!" << std::endl;
  sim.run(args.end_time);
}
