#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "progress.hpp"
#include "sim.hpp"


void Sim::load_from_file(std::string path)
{
  std::ifstream inf(path);
  std::string line;

  while (line.rfind("#", 0) == 0) getline(inf, line);

  std::getline(inf, line);
  std::stringstream ss(line);
  ss >> this->periodic_boundaries;

  double side_length;
  ss >> side_length;
  this->set_side_length(side_length);

  for (; std::getline(inf, line) ;) {

    if (line.rfind("#", 0) == 0) 
      continue;

    // std::cout << line << std::endl;
    std::stringstream ss(line);
    Ball *b = new Ball(line);
    b->set_parent(this);
    this->balls.push_back(b);
  }
}



void Sim::save_to_file(std::string path) const
{
  std::ofstream of(path, std::ios::trunc);

  of << this->periodic_boundaries << "\t" << this->side_length << std::endl;

  ProgressBar pb(this->balls.size(), "saving", 1.0);
  for (auto ball : this->balls) {
    of << ball->to_tsv() << std::endl;
    pb.update();
  }

}


void Sim::show_config() const
{
  for (auto ball : this->balls) {
    std::cerr << ball->repr() << std::endl;
  }
}
