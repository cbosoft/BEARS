#include "ball.hpp"
CollisionCheckResult *Ball::check_will_collide(Ball *other) const
{
  CollisionCheckResult *rv = new CollisionCheckResult;

  Vec dV = this->velocity - other->velocity;
  Vec dP = this->position - other->position;
  double A = dV.dot(dV);
  double B = 2.0*dV.dot(dP);
  double C = dP.dot(dP) - ((this->diameter+other->diameter)/2.0) ;

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

  Vec c1 = this->position + this->velocity*time;
  Vec c2 = other->position + other->velocity*time;

  rv->event = new CollisionEvent(time, c1, c2, (Ball *)this, other);
  rv->will_occur = true;

  return rv;
}
