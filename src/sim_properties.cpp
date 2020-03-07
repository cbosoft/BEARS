#include "sim.hpp"

double Sim::get_kinetic_energy() const
{
  double total = 0.0;
  for (auto ball : this->balls) 
  {
    total += ball->get_kinetic_energy();
  }
  return total;
}
