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
    Ball *b = new Ball(line);
    this->balls.push_back(b);
  }
}


// void Sim::append_to_file(std::string path) const
// {
// }
