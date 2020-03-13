#include <iostream>

#include "colour.hpp"
#include "version.hpp"

void print_header()
{
  print_header(NULL);
}

void print_header(const char *subtitle)
{
  std::cerr << BOLD << "BEARS" << RESET << " " << VERSION;

  if (subtitle)
    std::cerr << " " << subtitle;

  std::cerr << std::endl;
}
