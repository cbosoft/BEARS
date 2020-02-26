#pragma once

#include <string>
#include <sstream>
#include <cmath>

#include "exception.hpp"

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
      ss << "[" << this->i << ",\t" << this->j << ",\t" << this->k << "]";
      return ss.str();
    }

    std::string to_tsv() const
    {
      std::stringstream ss;
      ss << this->i << "\t" << this->j << "\t"  << this->k;
      return ss.str();
    }

    std::string to_yaml() const
    {
      std::stringstream ss;
      ss << "[" << this->i << ", " << this->j << ", " << this->k << "]";
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

    Vec component_along(const Vec &other) const
    {
      // returns the vector (RES) representing the part of THIS which follows the
      // direction of OTHER. So RES is parallel to OTHER, RES is perpendicular
      // to (THIS-RES).
      double scalar_component = this->dot(other) / other.dot(other);
      return other*scalar_component;
    }

    Vec operator+(const Vec &v) const { return this->add(v); }
    Vec operator-(const Vec &v) const { return this->subtract(v); }
    Vec operator*(const Vec &v) const { return this->cross(v); }
    Vec operator*(double m) const { return this->scalar_mult(m); }
    Vec operator/(double m) const { return this->scalar_mult(1./m); }
    Vec operator%(double mod) const 
    {
      (void) mod;
      throw NotImplementedError("This was executed without being implemented. Figure out float modulo already!");
      return {this->i, this->j, this->k};
    }
};

Vec vec_urand(double min, double max);
Vec vec_nrand(double mean, double std);
