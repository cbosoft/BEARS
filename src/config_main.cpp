#include <iostream>
#include <cmath>
#include <cstring>

#include "sim.hpp"
#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{
  
  struct {
    int n; 
    double mass;
    double diameter;
    double separation;
    double roughness;
    double velocity_mag;
    std::string path;
  } args = {9, 1.0, 1.0, 1.1, 0.0, 1.0, "config.ssv"};

  --argc; ++argv;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-n", "--number")) {
      args.n = atoi(argv[++i]);
    }
    else if (EITHER("-m", "--mass")) {
      args.mass = atoi(argv[++i]);
    }
    else if (EITHER("-d", "--diameter")) {
      args.diameter = atoi(argv[++i]);
    }
    else if (EITHER("-s", "--separation")) {
      args.separation = atoi(argv[++i]);
    }
    else if (EITHER("-r", "--roughness")) {
      args.roughness = atoi(argv[++i]);
    }
    else if (EITHER("-v", "--velocity")) {
      args.velocity_mag = atoi(argv[++i]);
    }
    else if (EITHER("-p", "--path")) {
      args.path = argv[++i];
    }
    else {
      std::cerr << "unknown arg: " << argv[i] << std::endl;
      exit(1);
    }
  }

  int n = args.n;
  int xn = ceil(cbrt(n));
  int yn = ceil(cbrt(n));
  int zn = ceil(cbrt(n));
  double sep = args.separation;

  struct BallConstructorData bcd;
  bcd.id = 0;
  bcd.mass = args.mass;
  bcd.diameter = args.diameter;
  bcd.roughness = args.roughness;

  Sim sim;
  int pperc = 0;
  for (int xi = 0; xi < xn; xi++) {
    for (int yi = 0; yi < yn; yi++) {
      for (int zi = 0; zi < zn; zi++) {
        bcd.id++;
        bcd.position = {sep*xi, sep*yi, sep*zi};
        bcd.velocity = vec_nrand(0, 1.0) * args.velocity_mag;
        sim.add_ball(bcd);
        int perc = bcd.id*100/n;
        if (perc > pperc) {
          std::cerr << '#';
          pperc = perc;
        }
      }
    }
  }
  std::cerr << std::endl;
  sim.save_to_file(args.path);
  std::cout << "done!" << std::endl;
}
