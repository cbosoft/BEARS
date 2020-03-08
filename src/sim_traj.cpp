#include <fstream>

#include "sim.hpp"


void Sim::append_to_trajectory() const
{
  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    std::cerr << "E: error opening file (\"" << this->trajectory_file_path << "\") for write (" << errno << ": " << strerror(errno) << ")" << std::endl;
    exit(1);
  }

  // of << " - " << this->time << std::endl;

  of << " - step:" << std::endl
     << "   time: " << this->time << std::endl
  //    << "     number: " << this->balls.size() << std::endl
     << "   balls: " << std::endl;

  for (auto ball: this->balls) {
    of << ball->to_yaml(5) << std::endl;
  }

}
