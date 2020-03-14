#include <fstream>

#include "version.hpp"
#include "sim.hpp"
#include "formatter.hpp"
#include "exception.hpp"
#include "bfstream.hpp"


void Sim::init_trajectory_tsv() const
{

  std::ofstream of(trajectory_file_path, std::ios::trunc);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "BEARS v" << VERSION << " LOG" << std::endl
    << "n: " << this->balls.size() << " L: " << this->side_length << std::endl
    ;
  of << Ball::tsv_headings() << "\tkinetic_energy" << std::endl;
}


void Sim::init_trajectory_bin() const
{

  OutputByteStreamer of(trajectory_file_path, std::ios::trunc);

  if (of.fail()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }
  
  // header bytes: software version, number of balls, ball information size, followed by 8 zeros.
  static_assert(sizeof(double) == 8);
  of << double(VERSION) << uint32_t(this->balls.size()) << uint8_t(Ball::bin_nbytes()) << uint8_t(0);
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


void Sim::append_to_trajectory_tsv(uint32_t aid, uint32_t bid) const
{

  std::ofstream of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << "t=\t" << this->time << "\t" << aid << "\tand\t" << bid << std::endl;
  for (auto ball: this->balls)
    of << ball->to_tsv() << "\t" << ball->get_kinetic_energy() << std::endl;

}


void Sim::append_to_trajectory_bin(uint32_t aid, uint32_t bid) const
{

  OutputByteStreamer of(this->trajectory_file_path, std::ios::app);

  if (!of.is_open()) {
    throw IOError(Formatter() << "Error opening trajectory ('" << trajectory_file_path << "')", true);
  }

  of << double(this->time) << aid << bid;
  for (auto ball: this->balls) {
    uint32_t *data = ball->to_bin();
    for (unsigned int i = 0; i < Ball::bin_nbytes(); i++) {
      of << data[i];
    }
  }

}


void Sim::append_to_trajectory() const
{
  this->append_to_trajectory(-1, -1);
}


void Sim::append_to_trajectory(int aid, int bid) const
{
  if (this->trajectory_file_extension.compare("tsv") == 0) {
    append_to_trajectory_tsv(aid, bid);
  }
  else if (this->trajectory_file_extension.compare("bin") == 0) {
    append_to_trajectory_bin(aid, bid);
  }
  else {
    // should only happen if I forget to update this if-else branch after
    // adding/changing trajectory formats.
    throw ArgumentError(Formatter() << "Unknown trajectory extension \"" << this->trajectory_file_extension << "\". Valid extensions are .tsv and .bin.");
  }
}
