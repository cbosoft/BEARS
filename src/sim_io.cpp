#include <iostream>
#include <fstream>
#include <string>

#include "sim.hpp"


void Sim::load_from_file(std::string path)
{
  std::ifstream inf(path);

  for (std::string line; std::getline(inf, line);) {

    if (line.rfind("#", 0) == 0) 
      continue;

    // std::cout << line << std::endl;
    std::stringstream ss(line);
    Ball *b = new Ball(line);
    this->balls.push_back(b);
  }
}


// void Sim::append_to_trajectory(std::string path) const
// {
// }

void Sim::save_to_file(std::string path) const
{
  std::ofstream of(path, std::ios::trunc);

  for (auto ball : this->balls) {
    of << ball->to_ssv() << std::endl;
  }

}
