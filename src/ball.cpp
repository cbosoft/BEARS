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


std::string Ball::to_yaml() const
{
  return this->to_yaml(0);
}

std::string Ball::to_yaml(int indent_size) const
{
  std::stringstream ss;
  for (int i = 0; i < indent_size; i++){
    ss << " ";
  }
  std::string indent = ss.str();

  ss.str("");
  ss << indent << " - " << std::endl
     << indent << "   id: " << this->id << std::endl
     << indent << "   mass: " << this->mass << std::endl
     //<< indent << "   inertia: " << this->inertia << std::endl
     << indent << "   diameter: " << this->diameter << std::endl
     //<< indent << "   roughness: " << this->roughness << std::endl
     << indent << "   position: " << this->position.to_yaml() << std::endl
     //<< indent << "   orientation: " << this->orientation.to_yaml() << std::endl
     << indent << "   velocity: " << this->velocity.to_yaml() << std::endl
     //<< indent << "   angular_velocity: " << this->angular_velocity.to_yaml() << std::endl
     //<< indent << "   force: " << this->force.to_yaml() << std::endl
     //<< indent << "   torque: " << this->torque.to_yaml() << std::endl
     << indent << "   kinetic_energy: " << this->get_kinetic_energy() << std::endl
     ;
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
