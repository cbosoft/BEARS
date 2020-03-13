#include <fstream>

#include "version.hpp"
#include "sim.hpp"
#include "formatter.hpp"
#include "exception.hpp"


void Sim::init_trajectory_yaml() const
{
  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "---" << std::endl;
}


void Sim::append_to_trajectory_yaml(int aid, int bid) const
{
  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  // TODO log event ids
  (void) aid;
  (void) bid;

  of << " - step:" << std::endl
     << "   time: " << this->time << std::endl
     //<< "   number: " << this->balls.size() << std::endl
     << "   balls: " << std::endl;

  for (auto ball: this->balls) {
    of << ball->to_yaml(5) << std::endl;
  }

}

void Sim::init_trajectory_tsv() const
{

  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "BEARS " << VERSION << " LOG" << std::endl
    << "n:\t" << this->balls.size() << "\tL:\t" << this->side_length << std::endl
    ;
  of << Ball::tsv_headings() << "\tkinetic_energy" << std::endl;
}

void Sim::append_to_trajectory_tsv(int aid, int bid) const
{

  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "t=\t" << this->time << "\t" << aid << "\tand\t" << bid << std::endl;
  for (auto ball: this->balls)
    of << ball->to_tsv() << "\t" << ball->get_kinetic_energy() << std::endl;

}


void Sim::init_trajectory()
{
  size_t last_dot_index = this->trajectory_file_path.rfind(".");
  size_t extension_length = this->trajectory_file_path.size() - last_dot_index - 1;
  this->trajectory_file_extension = this->trajectory_file_path.substr(last_dot_index + 1, extension_length);

  if (this->trajectory_file_extension.compare("yaml") == 0) {
    init_trajectory_yaml();
  }
  else if (this->trajectory_file_extension.compare("tsv") == 0) {
    init_trajectory_tsv();
  }
  else {
    throw ArgumentError(Formatter() << "Unknown trajectory extension \"" << this->trajectory_file_extension << "\". Valid extensions are .yaml and .tsv.");
  }

}


void Sim::append_to_trajectory() const
{
  this->append_to_trajectory(-1, -1);
}


void Sim::append_to_trajectory(int aid, int bid) const
{
  if (this->trajectory_file_extension.compare("yaml") == 0) {
    append_to_trajectory_yaml(aid, bid);
  }
  else if (this->trajectory_file_extension.compare("tsv") == 0) {
    append_to_trajectory_tsv(aid, bid);
  }
  else {
    // should only happen if I forget to update this if-else branch after
    // adding/changing trajectory formats.
    throw ArgumentError(Formatter() << "Unknown trajectory extension \"" << this->trajectory_file_extension << "\". Valid extensions are .yaml and .tsv.");
  }
}
