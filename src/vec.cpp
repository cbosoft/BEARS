#include "random.hpp"
#include "vec.hpp"

Vec vec_urand(double min, double max)
{
  return {urand(min, max), urand(min, max), urand(min, max)};
}


Vec vec_nrand(double mean, double std)
{
  return {nrand(mean, std), nrand(mean, std), nrand(mean, std)};
}
