#include <iostream>
#include <cstring>

#include "sim.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{
  --argc; ++argv;
  std::string config_file;
  bool config_file_set = false;

  // TODO: improve arg parsing
  // TODO: check given path exists and is an ssv
  for (int i = 0; i < argc; i++) {
    if (EITHER("-c", "--config-file")) {
      i++;
      config_file = std::string(argv[i]);
      config_file_set = true;
    }
    else {
      std::cerr << "unknown arg: " << argv[i] << std::endl;
      exit(1);
    }
  }

  if (!config_file_set) {
    std::cerr << "need arg: config file path" << std::endl;
    exit(1);
  }

  Sim sim(config_file);
  sim.run(10);
}
