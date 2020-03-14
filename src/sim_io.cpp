#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "bfstream.hpp"
#include "progress.hpp"
#include "sim.hpp"
#include "exception.hpp"
#include "formatter.hpp"


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

  std::cerr << "Loaded configuration of " << this->balls.size() << " particles." << std::endl;
}


void Sim::save_config_tsv() const
{
  std::ofstream of(this->config_file_path, std::ios::trunc);
  of << this->periodic_boundaries << "\t" << this->side_length << std::endl;
  ProgressBar pb(this->balls.size(), "saving", 1.0);
  for (auto ball : this->balls) {
    of << ball->to_tsv() << std::endl;
    pb.update();
  }
}


void Sim::save_config_bin() const
{
  OutputByteStreamer of(this->config_file_path, std::ios::trunc | std::ios::binary);
  // TODO: header bytes informing about software version, number of balls, box bounds and such
  ProgressBar pb(this->balls.size(), "saving", 1.0);
  for (auto ball : this->balls) {
    uint32_t *data = ball->to_bin();
    for (unsigned int i = 0; i < Ball::bin_nbytes(); i++) {
      of << data[i];
    }
  }
}


void Sim::save_config() const
{

  if (this->config_file_extension.compare("tsv") == 0) {
    this->save_config_tsv();
  }
  else if (this->config_file_extension.compare("bin") == 0) {
    this->save_config_bin();
  }
  else {
    throw ArgumentError(Formatter() << "Unknown config file extension \"" << this->config_file_extension << "\". Valid extensions are .tsv and .bin.");
  }

}
