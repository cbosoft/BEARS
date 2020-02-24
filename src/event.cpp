#include <iostream>

#include "vec.hpp"
#include "ball.hpp"
#include "event.hpp"


CollisionCheckResult *collision_check(Ball *a, Ball *b)
{
  CollisionCheckResult *rv = new CollisionCheckResult;

  Vec dV = a->velocity - b->velocity;
  Vec dP = a->position - b->position;
  double A = dV.dot(dV);
  double B = 2.0*dV.dot(dP);
  double C = dP.dot(dP) - ((a->diameter+b->diameter)/2.0) ;

  double discriminant = (B*B) - (4*A*C);
  double time = -1.0;
  if (discriminant < 0.0) {
    // std::cerr << "no collision" << std::endl;
    rv->will_occur = false;
    return rv;
  }
  else if (discriminant == 0.0) {
    // std::cerr << "zero discriminant" << std::endl;
    rv->will_occur = false;
    return rv;
  }
  else {
    double discriminant_root = std::pow(discriminant, 0.5);

    double bigsol = (-B + discriminant_root) / (2.0 * A);
    double lilsol = (-B - discriminant_root) / (2.0 * A);
    if ((bigsol < 1e-7) || (lilsol < 1e-7)) {
      // std::cerr << "both results zero" << std::endl;
      rv->will_occur = false;
      return rv;
    }

    if (lilsol < 0.0) {
      if (bigsol < 0.0) {
        // std::cerr << "both results negative" << std::endl;
        time = 0.0;
        rv->will_occur = false;
      }
      else {
        time = bigsol;
      }
    }
    else {
      if (bigsol < 0.0) {
        time = lilsol;
      }
      else {
        time = lilsol;
        if (bigsol < lilsol) {
          time = bigsol;
        }
      }
    }
  }

  Vec c1 = a->position + a->velocity*time;
  Vec c2 = b->position + b->velocity*time;

  rv->event = new CollisionEvent(time, c1, c2, a, b);
  rv->will_occur = true;

  return rv;
}
