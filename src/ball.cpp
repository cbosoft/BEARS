#include <algorithm>

#include "ball.hpp"
#include "sim.hpp"

Ball::Ball(const struct BallConstructorData &bcd)
{
  this->id = bcd.id;
  this->mass = bcd.mass;
  this->inertia = bcd.inertia;
  this->diameter = bcd.diameter;
  this->roughness = bcd.roughness;

  this->position = bcd.position;
  this->orientation = bcd.orientation;

  this->velocity = bcd.velocity;
  this->angular_velocity = bcd.angular_velocity;

  this->force = bcd.force;
  this->torque = bcd.torque;
}

Ball::Ball(const std::string s)
{
  std::stringstream ss(s);
  ss >> this->id
    >> this->mass
    >> this->inertia
    >> this->diameter
    >> this->roughness;

  double x, y, z;
  ss >> x; ss >> y; ss >> z;
  this->position = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->orientation = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->velocity = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->angular_velocity = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->force = {x, y, z};

  ss >> x; ss >> y; ss >> z;
  this->torque = {x, y, z};
}

std::string Ball::tsv_headings()
{
  std::stringstream ss;
  ss 
    << "id"
    << "\tmass"
    << "\tinertia"
    << "\tdiameter"
    << "\troughness"
    << "\tpositionx\tpositiony\tpositionz"
    << "\torientationx\torientationy\torientationz"
    << "\tvelocityx\tvelocityy\tvelocityz"
    << "\tangular_velocityx\tangular_velocityy\tangular_velocityz"
    << "\tforcex\tforcey\tforcez"
    << "\tpositionx\tpositiony\tpositionz"
    ;
  return ss.str();
}

std::string Ball::to_tsv() const
{
  std::stringstream ss;
  ss 
    << this->id 
    << "\t" << this->mass 
    << "\t" << this->inertia 
    << "\t" << this->diameter 
    << "\t" << this->roughness 
    << "\t" << this->position.to_tsv() 
    << "\t" << this->orientation.to_tsv() 
    << "\t" << this->velocity.to_tsv() 
    << "\t" << this->angular_velocity.to_tsv() 
    << "\t" << this->force.to_tsv() 
    << "\t" << this->torque.to_tsv()
    ;
  return ss.str();
}

Ball::~Ball()
{
}

std::string Ball::repr() const
{
  std::stringstream ss;
  ss << "Ball(" 
    << this->id 
    << ", " << this->position.repr()
    << ")";
  return ss.str();
}



void Ball::timejump(double dt)
{
  this->position = this->position + (this->velocity*dt);
  this->enforce_bounds();

  this->orientation = this->orientation + (this->angular_velocity*dt);
  this->orientation = this->orientation % (2.0*M_PI);
}

void Ball::set_parent(Sim *parent)
{
  this->parent = parent;
}

void Ball::set_image(const Vec &image)
{
  this->position = this->position + image;
}

void Ball::enforce_bounds()
{
  this->position = this->parent->enforce_bounds(this->position);
}

int Ball::get_id() const
{
  return this->id;
}

uint32_t Ball::bin_nbytes()
{
  uint32_t 
    dbl_size = sizeof(double),
    id_size = sizeof(Ball::id),
    nbytes = dbl_size * ( (3 * 4) + 5 ) + id_size; // 4 3vecs: pos, orien, vel, angvel, plus uint32: id, plus 5 dbls: roughness, mass, inertia, diameter, kinetic energy
  return nbytes;
}


#define APPEND_DBL_UINTARR(D)\
  {\
    *vi = D;\
    vi_uints = reinterpret_cast<uint32_t *>(vi);\
    for (unsigned int j = 0; j < sizeof(vi_uints); j++) \
      rv[offset+j] = vi_uints[j]; \
    offset += sizeof(double); \
  }
#define APPEND_VEC_UINTARR(V)\
  {\
    for (auto v : V.as_array()) { \
      APPEND_DBL_UINTARR(v); \
    }\
  }

uint32_t *Ball::to_bin() const
{
  uint32_t *rv = new uint32_t[Ball::bin_nbytes()];

  int offset = 0;
  double *vi = new double(0.0);
  uint32_t *vi_uints = NULL;
  APPEND_DBL_UINTARR(this->roughness);
  APPEND_DBL_UINTARR(this->mass);
  APPEND_DBL_UINTARR(this->inertia);
  APPEND_DBL_UINTARR(this->diameter);

  APPEND_VEC_UINTARR(this->position);
  APPEND_VEC_UINTARR(this->orientation);
  APPEND_VEC_UINTARR(this->velocity);
  APPEND_VEC_UINTARR(this->angular_velocity);

  APPEND_DBL_UINTARR(this->get_kinetic_energy());

  delete vi;
  return rv;
}

#undef OUTPUT
