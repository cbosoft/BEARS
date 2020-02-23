#include "sim.hpp"

int main(void)
{
  Sim sim;
  sim.load_from_file("test.ssv");
  sim.run(10);
}
