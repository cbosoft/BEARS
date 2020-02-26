#include "ball.hpp"

void Ball::collide(Ball *other)
{

  double av_roughness = (this->roughness + other->roughness) * 0.5;

  // the tangent to the centre-centre vector is diminished by friction
  Vec dC = this->position - other->position;
  Vec velocity_normal = this->velocity.component_along(dC);
  Vec velocity_tangent = this->velocity - velocity_normal;
  Vec velocity_tangent_reduced = velocity_tangent * (1 - av_roughness);
  this->velocity = (velocity_tangent_reduced + velocity_normal) + (other->angular_velocity*av_roughness*other->diameter*0.5*other->mass/this->mass);

  Vec momentum_lost = velocity_tangent * av_roughness * this->mass;
  Vec other_angular_velocity_gain = ((momentum_lost * (other->diameter*0.5)) + (this->angular_velocity * av_roughness * this->mass)) / other->mass;


  dC = other->position - this->position;
  velocity_normal = other->velocity.component_along(dC);
  velocity_tangent = other->velocity - velocity_normal;
  velocity_tangent_reduced = velocity_tangent * (1 - av_roughness);
  other->velocity = (velocity_tangent_reduced + velocity_normal) + (this->angular_velocity*av_roughness*this->diameter*0.5*this->mass/other->mass);

  momentum_lost = velocity_tangent * av_roughness * other->mass;
  Vec this_angular_velocity_gain = (momentum_lost * (this->diameter*0.5)) / this->mass;

  this->angular_velocity = this->angular_velocity * (1 - av_roughness) + this_angular_velocity_gain;
  other->angular_velocity = other->angular_velocity * (1 - av_roughness) + other_angular_velocity_gain;


  {
    double totmass = this->mass + other->mass;
    Vec new_this_velocity = (this->velocity*((this->mass - other->mass) / (totmass))) + (other->velocity * (other->mass * 2 / totmass));
    Vec new_other_velocity = (other->velocity*((other->mass - this->mass) / (totmass))) + (this->velocity * (this->mass * 2 / totmass));
    this->velocity = new_this_velocity;
    other->velocity = new_other_velocity;
  }


  // {
  //   double totinertia = this->inertia + other->inertia;
  //   Vec new_a_angular_velocity = (((this->angular_velocity - other->angular_velocity) * other->inertia * av_roughness) + (this->angular_velocity*this->inertia) + (other->angular_velocity*other->inertia)) * (1.0/totinertia);
  //   Vec new_b_angular_velocity = (((other->angular_velocity - this->angular_velocity) * this->inertia * av_roughness) + (other->angular_velocity*other->inertia) + (this->angular_velocity*this->inertia)) * (1.0/totinertia);
  //   this->angular_velocity = new_a_angular_velocity;
  //   other->angular_velocity = new_b_angular_velocity;
  // }

}


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
