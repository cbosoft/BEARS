#pragma once

#include <string>
#include <sstream>
#include <cmath>

class Vec {
  private:
    double i, j, k;

  public:

    Vec() : Vec(0.0, 0.0, 0.0){}
    Vec(double i, double j, double k)
    {
      this->i = i;
      this->j = j;
      this->k = k;
    }

    std::string repr() const
    {
      std::stringstream ss;
      ss << "[" << this->i << ", " << this->j << ", " << this->k << "]";
      return ss.str();
    }

    std::string to_ssv() const
    {
      std::stringstream ss;
      ss << this->i << " " << this->j << " "  << this->k;
      return ss.str();
    }

    Vec add(const Vec &v) const
    {
      return Vec( this->i + v.i,
                  this->j + v.j,
                  this->k + v.k );
    }

    Vec subtract(const Vec &v) const
    {
      return Vec( this->i - v.i,
                  this->j - v.j,
                  this->k - v.k );
    }

    Vec cross(const Vec &v) const
    {
      return Vec( (this->j*v.k) - (this->k*v.j),
                  (this->k*v.i) - (this->i*v.k),
                  (this->i*v.j) - (this->j*v.i) );
    }

    double dot(const Vec &v) const
    {
      return (this->i*v.i) + (this->j*v.j) + (this->k*v.k);
    }

    Vec scalar_mult(double m) const
    {
      return Vec( this->i*m, this->j*m, this->k*m);
    }

    double magnitude() const
    {
      return std::pow( (this->i*this->i) + (this->j*this->j) + (this->k*this->k), 0.5);
    }

    Vec operator+(const Vec &v) const { return this->add(v); }
    Vec operator-(const Vec &v) const { return this->subtract(v); }
    Vec operator*(const Vec &v) const { return this->cross(v); }
    Vec operator*(double m) const { return this->scalar_mult(m); }
};
