#pragma once

#include <string>
#include <sstream>
#include <cmath>
#include <array>

#include "exception.hpp"

class Vec {

  public:
    std::array<double, 3> v;

    Vec() : Vec(0.0, 0.0, 0.0){}
    Vec(double i, double j, double k)
    {
      this->v[0] = i;
      this->v[1] = j;
      this->v[2] = k;
    }


    std::string repr() const
    {
      std::stringstream ss;
      ss << "[" << this->v[0] << ",\t" << this->v[1] << ",\t" << this->v[2] << "]";
      return ss.str();
    }

    std::string to_tsv() const
    {
      std::stringstream ss;
      ss << this->v[0] << "\t" << this->v[1] << "\t"  << this->v[2];
      return ss.str();
    }

    std::string to_yaml() const
    {
      std::stringstream ss;
      ss << "[" << this->v[0] << ", " << this->v[1] << ", " << this->v[2] << "]";
      return ss.str();
    }

    Vec add(const Vec &v) const
    {
      return Vec( this->v[0] + v.v[0],
                  this->v[1] + v.v[1],
                  this->v[2] + v.v[2] );
    }

    Vec subtract(const Vec &v) const
    {
      return Vec( this->v[0] - v.v[0],
                  this->v[1] - v.v[1],
                  this->v[2] - v.v[2] );
    }

    Vec cross(const Vec &v) const
    {
      return Vec( (this->v[1]*v.v[2]) - (this->v[2]*v.v[1]),
                  (this->v[2]*v.v[0]) - (this->v[0]*v.v[2]),
                  (this->v[0]*v.v[1]) - (this->v[1]*v.v[0]) );
    }

    double dot(const Vec &v) const
    {
      return (this->v[0]*v.v[0]) + (this->v[1]*v.v[1]) + (this->v[2]*v.v[2]);
    }

    Vec scalar_mult(double m) const
    {
      return Vec( this->v[0]*m, this->v[1]*m, this->v[2]*m);
    }

    double magnitude() const
    {
      return std::pow( (this->v[0]*this->v[0]) + (this->v[1]*this->v[1]) + (this->v[2]*this->v[2]), 0.5);
    }

    Vec component_along(const Vec &other) const
    {
      // returns the vector (RES) representing the part of THIS which follows the
      // direction of OTHER. So RES is parallel to OTHER, RES is perpendicular
      // to (THIS-RES).
      double scalar_component = this->dot(other) / other.dot(other);
      return other*scalar_component;
    }

    const std::array<double, 3> &as_array() const
    {
      return this->v;
    }

    Vec operator+(const Vec &v) const { return this->add(v); }
    Vec operator-(const Vec &v) const { return this->subtract(v); }
    Vec operator*(const Vec &v) const { return this->cross(v); }
    Vec operator*(double m) const { return this->scalar_mult(m); }
    Vec operator/(double m) const { return this->scalar_mult(1./m); }
    Vec operator%(double divisor) const 
    {
      return {
        std::fmod(this->v[0], divisor), 
        std::fmod(this->v[1], divisor), 
        std::fmod(this->v[2], divisor)};
    }
};

Vec vec_urand(double min, double max);
Vec vec_nrand(double mean, double std);
