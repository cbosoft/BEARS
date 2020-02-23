#include <string>
#include <iostream>
#include <fstream>

#include "sim.hpp"


void Sim::load_from_file(std::string path)
{
  std::ifstream inf(path);

  for (std::string line; std::getline(inf, line);) {

    if (line.rfind("#", 0) == 0) 
      continue;

    // std::cout << line << std::endl;
    std::stringstream ss(line);
    double diameter;
    double mass;
    double roughness;
    double px, py, pz;
    double wx, wy, wz;
    double vx, vy, vz;
    double avx, avy, avz;
    double fx, fy, fz;
    double mx, my, mz;
    ss >> diameter;
    ss >> mass;
    ss >> roughness;
    ss >> px; ss >> py; ss >> pz;
    ss >> wx; ss >> wy; ss >> wz;
    ss >> vx; ss >> vy; ss >> vz;
    ss >> avx; ss >> avy; ss >> avz;
    ss >> fx; ss >> fy; ss >> fz;
    ss >> mx; ss >> my; ss >> mz;
    Ball *b = new Ball({.diameter=diameter,
        .mass=mass, .roughness=roughness,
        .position={px, py, pz},
        .orientation={wx, wy, wz},
        .velocity={vx, vy, vz},
        .angular_velocity={avx, avy, avz},
        .force={fx, fy, fz},
        .torque={mx, my, mz}});
    this->balls.push_back(b);
  }
}


// void Sim::append_to_file(std::string path) const
// {
// }
