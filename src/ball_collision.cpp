#include "ball.hpp"
#include "sim.hpp"

void Ball::collide(Ball *other)
{
  // method which calculates new velocities, angular velocities after a
  // collision with another particle

  // Using impulsive collision method outlined on wikipedia:
  // https://en.wikipedia.org/wiki/Collision_response
  
  double COR = this->hCOR + other->hCOR;
  Vec relative_position = other->position - this->position;
  Vec impulse_unit_normal = relative_position / relative_position.magnitude();
  Vec collision_point = this->position + (relative_position * this->diameter / (this->diameter + other->diameter) );
  Vec rthis = collision_point - this->position;
  Vec rother = collision_point - other->position;
  Vec relative_contact_velocity = other->velocity + other->angular_velocity.cross(rother) - 
    this->velocity - this->angular_velocity.cross(rthis);

  double relative_velocity_dot_n = relative_contact_velocity.dot(impulse_unit_normal);
  double numerator = (-1. - COR) * relative_velocity_dot_n;
  double denominator_pt_1 = (1./this->mass) + (1./other->mass);
  Vec denominator_pt_vec_this = rthis.cross(impulse_unit_normal).cross(rthis) / this->inertia;
  Vec denominator_pt_vec_other = rother.cross(impulse_unit_normal).cross(rother) / other->inertia;
  double denominator = denominator_pt_1 + impulse_unit_normal.dot(denominator_pt_vec_this + denominator_pt_vec_other);
  double impulse_magnitude = numerator / denominator;

  this->angular_velocity = this->angular_velocity - (impulse_unit_normal.cross(rthis) * impulse_magnitude / this->inertia);
  other->angular_velocity = other->angular_velocity + (impulse_unit_normal.cross(rother) * impulse_magnitude / other->inertia);

  Vec tr;
  bool is_direct = relative_velocity_dot_n != 0.0;
  if (is_direct) {
    tr = relative_contact_velocity - (impulse_unit_normal * relative_velocity_dot_n);
    tr = tr / tr.magnitude();
  }
  // TODO: force balance
  //else if () {
  //}
  else {
    return;
  }

  // TODO: are the roughness parameters enough to decide the friction for the
  // system?
  const double static_friction_coef = 0.5;
  const double dynamic_friction_coef = 0.3;

  double mvr_dot_tr_this = (relative_contact_velocity * this->mass).dot(tr);
  double mvr_dot_tr_other = (relative_contact_velocity * other->mass).dot(tr);
  bool is_static_this, is_static_other;
  if (tr.dot(relative_contact_velocity) == 0.0) {
    is_static_this = is_static_other = true;
  }
  else {
    is_static_this = mvr_dot_tr_this <= static_friction_coef;
    is_static_other = mvr_dot_tr_other <= static_friction_coef;
  }
  Vec friction_impulse_this = is_static_this ? (tr*-mvr_dot_tr_this) : (tr*-dynamic_friction_coef);
  Vec friction_impulse_other = is_static_other ? (tr*-mvr_dot_tr_other) : (tr*-dynamic_friction_coef);

  this->velocity = this->velocity - (impulse_unit_normal * impulse_magnitude / this->mass) + (friction_impulse_this / this->mass);
  other->velocity = other->velocity + (impulse_unit_normal * impulse_magnitude / other->mass) + (friction_impulse_other / this->mass);

}

static double check_will_collide_pv(const Vec &dP, const Vec &dV, double avdia)
{
  double A = dV.dot(dV);
  double B = 2.0*dV.dot(dP);
  double C = dP.dot(dP) - avdia;

  double discriminant = (B*B) - (4.0*A*C);
  double time = -1.0;
  if (discriminant > 0.0) {
    double discriminant_root = std::pow(discriminant, 0.5);

    double bigsol = (-B + discriminant_root) / (2.0 * A);
    double lilsol = (-B - discriminant_root) / (2.0 * A);
    if ((bigsol > 1e-7) or (lilsol > 1e-7)) {
      // if results are not both approximately zero...

      if (lilsol < 0.0) {
        if (bigsol > 0.0) {
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
  }

  return time;
}


CollisionEvent *Ball::check_will_collide_minimum_image(Ball *other, double L, double simtime) const
{
  Vec dV = other->velocity - this->velocity;
  //Vec p1 = this->position - (this->position.floordiv(L)*L);
  //Vec p2 = other->position - (other->position.floordiv(L)*L);
  Vec dP = other->position - this->position;//p2 - p1;
  dP -= (dP / L).nearbyint() * L;

  double time = check_will_collide_pv(dP, dV, (this->diameter + other->diameter) / 2.0);
  if (time > 0.0) {
    return new CollisionEvent(time+simtime, (Ball *)this, other, {0, 0, 0});
  }
  else {
    return nullptr;
  }

}

CollisionEvent *Ball::check_will_collide_image(Ball *other, Vec image) const
{
  Vec dV = other->velocity - this->velocity;
  Vec dP = (other->position + image) - this->position;

  double time = check_will_collide_pv(dP, dV, (this->diameter + other->diameter) / 2.0);
  if (time > 0.0) {
    return new CollisionEvent(time, (Ball *)this, other, image);
  }
  else {
    return nullptr;
  }

}
