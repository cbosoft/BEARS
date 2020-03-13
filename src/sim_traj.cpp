#include <fstream>

#include "version.hpp"
#include "sim.hpp"
#include "formatter.hpp"
#include "exception.hpp"


void Sim::init_trajectory_tsv() const
{

  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "BEARS " << VERSION << " LOG" << std::endl
    << "n: " << this->balls.size() << " L: " << this->side_length << std::endl
    ;
  of << Ball::tsv_headings() << "\tkinetic_energy" << std::endl;
}


void Sim::init_trajectory_bin() const
{

  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (of.fail()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }
  
  // TODO: header bytes: software version, number of balls, etc
}


void Sim::init_trajectory()
{

  if (this->trajectory_file_extension.compare("tsv") == 0) {
    init_trajectory_tsv();
  }
  else if (this->trajectory_file_extension.compare("bin") == 0) {
    init_trajectory_bin();
  }
  else {
    throw ArgumentError(Formatter() << "Unknown trajectory extension \"" << this->trajectory_file_extension << "\". Valid extensions are .tsv and .bin.");
  }

}


void Sim::append_to_trajectory_tsv() const
{

  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "t= " << this->time << std::endl;
  for (auto ball: this->balls)
    of << ball->to_tsv() << ball->get_kinetic_energy() << std::endl;

}


void Sim::append_to_trajectory_bin() const
{

  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  // TODO: timestep information
  for (auto ball: this->balls) {
    auto bytes = ball->to_bin();
    for (auto byte : bytes) {
      of << byte << std::endl; // TODO kinetic energy?
    }
  }

}


void Sim::append_to_trajectory() const
{
  if (this->trajectory_file_extension.compare("tsv") == 0) {
    append_to_trajectory_tsv();
  }
  else if (this->trajectory_file_extension.compare("bin") == 0) {
    append_to_trajectory_bin();
  }
  else {
    // should only happen if I forget to update this if-else branch after
    // adding/changing trajectory formats.
    throw ArgumentError(Formatter() << "Unknown trajectory extension \"" << this->trajectory_file_extension << "\". Valid extensions are .tsv and .bin.");
  }
}
