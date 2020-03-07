#include <cmath>
#include "sim.hpp"

// https://en.wikipedia.org/wiki/Periodic_boundary_conditions#Practical_implementation:_continuity_and_the_minimum_image_convention

Vec Sim::enforce_bounds(const Vec &p) const
{
  Vec rv;
  for (unsigned int i = 0; i < p.v.size(); i++) {
    rv.v[i] = std::fmod(p.v[i], this->side_length);
    if (rv.v[i] < 0.0) rv.v[i] += this->side_length;
  }
  return rv;
}

Vec Sim::enforce_bounds(const Vec &p1, const Vec &p2) const
{
  Vec dp;
  for (unsigned int i = 0; i < p1.v.size(); i++) {
    dp.v[i] = p2.v[i] - p1.v[i];
    dp.v[i] -= this->side_length * std::nearbyint(dp.v[i] * this->inv_side_length);
  }
  return dp;
}
