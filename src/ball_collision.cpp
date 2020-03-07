#include "ball.hpp"
#include "sim.hpp"

void Ball::collide(Ball *other)
{
  // method which calculates new velocities, angular velocities after a
  // collision with another particle

  // Using impulsive collision method outlined on wikipedia:
  // https://en.wikipedia.org/wiki/Collision_response
  
  // TODO: add friction
  // TODO: what if COR != 1.0?
  // TODO: sort out notation 1,2 v *this,other*? *long names* v short names?

  const double COR = 1.0;
  Vec relative_velocity = other->velocity - this->velocity;
  Vec relative_momentum = (other->velocity * other->mass) - (this->velocity * this->mass);
  Vec impulse_unit_normal = relative_momentum / relative_momentum.magnitude();
  Vec collision_point = this->position + ((other->position - this->position) * this->diameter / (this->diameter + other->diameter) );
  Vec r1 = collision_point - this->position;
  Vec r2 = collision_point - other->position;

  double relative_velocity_dot_n = relative_velocity.dot(impulse_unit_normal);
  double numerator = (-1 - COR) * relative_velocity_dot_n;
  double denominator_pt_1 = (1./this->mass) + (1./other->mass);
  Vec r1_cross_normal = r1.cross(impulse_unit_normal);
  Vec r2_cross_normal = r2.cross(impulse_unit_normal);
  Vec denominator_pt_vec_1 = (r1_cross_normal * (1.0/this->inertia)).cross(r1);
  Vec denominator_pt_vec_2 = (r2_cross_normal * (1.0/other->inertia)).cross(r2);
  double denominator = denominator_pt_1 + impulse_unit_normal.dot(denominator_pt_vec_1 + denominator_pt_vec_2);
  double impulse_magnitude = numerator / denominator;

  this->velocity = this->velocity - (impulse_unit_normal * impulse_magnitude / this->mass);
  other->velocity = other->velocity + (impulse_unit_normal * impulse_magnitude / other->mass);

  this->angular_velocity = this->angular_velocity - (impulse_unit_normal.cross(r1) * impulse_magnitude / this->inertia);
  other->angular_velocity = other->angular_velocity + (impulse_unit_normal.cross(r2) * impulse_magnitude / other->inertia);



}


CollisionCheckResult *Ball::check_will_collide(Ball *other) const
{
  CollisionCheckResult *rv = new CollisionCheckResult;

  assert(this->parent == other->parent, "colliding balls must children of same sim box");

  Vec dV = other->velocity - this->velocity;
  Vec dP = this->parent->enforce_bounds(this->position, other->position);
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

  rv->event = new CollisionEvent(time, (Ball *)this, other);
  rv->will_occur = true;

  return rv;
}
