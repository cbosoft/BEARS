#include <iostream>
#include <cmath>
#include <utility>
#include <string>
#include <sstream>

#include "ball.hpp"

typedef std::vector<double> Vec;

std::string vec3_repr(const Vec &v)
{
  std::stringstream ss;
  ss << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
  return ss.str();
}

Vec vec3_cross(Vec u, Vec v)
{
  return { 
      (u[1]*v[2]) - (u[2]*v[1]),
      (u[2]*v[0]) - (u[0]*v[2]),
      (u[0]*v[1]) - (u[1]*v[0])
  };
}

Vec vec3_diff(const Vec &u, const Vec &v)
{
  return {u[0] - v[0], u[1] - v[1], v[2] - v[2]};
}

double vec3_dot(const Vec &u, const Vec &v)
{
  return (u[0]*v[0]) + (u[1]*v[1]) + (u[2]*v[2]);
}

Vec vec3_scalar_mult(double m, const Vec &v)
{
  return {v[0]*m, v[1]*m, v[2]*m};
}

Vec vec3_add(const Vec &u, const Vec &v)
{
  return {u[0]+v[0], u[1]+v[1], u[2]+v[2]};
}

double vec3_magnitude(Vec u)
{
  double asq = (u[0]*u[0]) + (u[1]*u[1]) + (u[2]*u[2]);
  return std::pow(asq, 0.5);
}

// https://math.stackexchange.com/questions/1414285/location-of-shortest-distance-between-two-skew-lines-in-3d?rq=1
std::pair<Vec, Vec> get_points_of_closest_approach(const Vec &p1, 
                     const Vec &v1, 
                     const Vec &p2, 
                     const Vec &v2)
{
  Vec n = vec3_cross(v1, v2);
  // what if lines are parallel? (n == [0, 0, 0])
  // TODO
  Vec n1 = vec3_cross(v1, n);
  Vec n2 = vec3_cross(v2, n);

  Vec c1 = vec3_add(p1, vec3_scalar_mult(vec3_dot(vec3_diff(p2, p1), n2)/vec3_dot(v1, n2), v1));
  Vec c2 = vec3_add(p2, vec3_scalar_mult(vec3_dot(vec3_diff(p1, p2), n1)/vec3_dot(v2, n1), v2));

  std::cout << vec3_repr(n) << std::endl;

  return std::make_pair(c1, c2);
}

bool lines_intersect(const Ball &a, const Ball &b)
{
  Vec v1 = a.velocity;
  Vec v2 = b.velocity;
  Vec p1 = a.position;
  Vec p2 = b.position;

  std::cout << vec3_repr(v1) << " " << vec3_repr(v2) << std::endl;
  std::pair<Vec, Vec> close_points = get_points_of_closest_approach(p1, v1, p2, v2);
  Vec c1 = close_points.first;
  Vec c2 = close_points.second;
  double closest_distance = vec3_magnitude(vec3_diff(c1, c2));
  std::cout << vec3_repr(c1) << " " << vec3_repr(c2) << " " << closest_distance << std::endl;
  double thresh_distance = (a.diameter/2) + (b.diameter/2);

  double d1 = vec3_magnitude(vec3_diff(c1, p1));
  double d2 = vec3_magnitude(vec3_diff(c2, p2));

  if (std::abs(d1 - d2) > thresh_distance)
    return false; // balls will miss each other

  double t1 = d1 / vec3_magnitude(v1);
  double t2 = d2 / vec3_magnitude(v2);

  return closest_distance <= thresh_distance;
}



int main(void)
{
  Ball a = Ball({
      .diameter=1.0, 
      .mass=1.0, 
      .roughness=0.0, 
      .position={2, 0.0, 0.0},
      .orientation={0.0, 0.0, 0.0},
      .velocity={-1.0, 0.0, 0.0},
      .angular_velocity={0.0, 0.0, 0.0},
      .force={0.0, 0.0, 0.0},
      .torque={0.0, 0.0, 0.0}});

  Ball b = Ball({
      .diameter=1.0, 
      .mass=1.0, 
      .roughness=0.0, 
      .position={ 1.0, 2.0, 0.0},
      .orientation={0.0, 0.0, 0.0},
      .velocity={0.0, -1.0, 0.0},
      .angular_velocity={0.0, 0.0, 0.0},
      .force={0.0, 0.0, 0.0},
      .torque={0.0, 0.0, 0.0}});

  // two balls, when will they impact?
  // step back a bit, given two lines, will they intersect? will they intersect
  // at same time?
  std::cout << lines_intersect(a, b) << std::endl;

}
