#include <iostream>
#include <cstring>

#include "sim.hpp"
#include "version.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{

  struct {
    std::string config_file_path;
    std::string output_file_path;
    double end_time;
    int nthreads;
    bool should_output_trajectory;
  } args = {
    "config.tsv", 
    "traj.tsv",
    1.0,
    1,
    true
  };

  // TODO: improve arg parsing
  // TODO: check given path exists and is in correct format
  --argc; ++argv;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-c", "--config-file")) {
      args.config_file_path = std::string(argv[++i]);
    }
    else if (EITHER("-i", "--input-file")) {
      args.config_file_path = std::string(argv[++i]);
    }
    else if (EITHER("-o", "--output-file")) {
      args.output_file_path = std::string(argv[++i]);
      args.should_output_trajectory = true;
    }
    else if (EITHER("-t", "--end-time")) {
      args.end_time = atof(argv[++i]);
    }
    else if (EITHER("-e", "--endless")) {
      args.end_time = -1;
    }
    else if (EITHER("-n", "--number-threads")) {
      args.nthreads = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "--no-trajectory") == 0) {
      args.should_output_trajectory = false;
    }
    else {
      std::cerr << "unknown arg: " << argv[i] << std::endl;
      exit(1);
    }
  }

  print_header();
  std::cerr << "Setting up..." << std::endl;
  Sim sim(args.config_file_path, args.output_file_path);

  sim.set_should_output_trajectory(args.should_output_trajectory);

  if (args.nthreads > 1) {
    sim.set_parallel(args.nthreads);
  }

  sim.append_to_trajectory();

  std::cerr << "Begin!" << std::endl;
  sim.run(args.end_time);
}
