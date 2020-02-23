#include <iostream>
#include <cmath>
#include <tuple>
#include <string>
#include <sstream>

#include "vec.hpp"
#include "ball.hpp"

// https://math.stackexchange.com/questions/1414285/location-of-shortest-distance-between-two-skew-lines-in-3d?rq=1
// std::pair<Vec, Vec> get_points_of_closest_approach(const Vec &p1, 
//                      const Vec &v1, 
//                      const Vec &p2, 
//                      const Vec &v2)
// {
//   Vec n = vec3_cross(v1, v2);
//   // what if lines are parallel? (n == [0, 0, 0])
//   // TODO
//   Vec n1 = vec3_cross(v1, n);
//   Vec n2 = vec3_cross(v2, n);
// 
//   Vec c1 = vec3_add(p1, vec3_scalar_mult(vec3_dot(vec3_diff(p2, p1), n2)/vec3_dot(v1, n2), v1));
//   Vec c2 = vec3_add(p2, vec3_scalar_mult(vec3_dot(vec3_diff(p1, p2), n1)/vec3_dot(v2, n1), v2));
// 
//   std::cout << vec3_repr(n) << std::endl;
// 
//   return std::make_pair(c1, c2);
// }

// bool lines_intersect(const Ball &a, const Ball &b)
// {
//   Vec v1 = a.velocity;
//   Vec v2 = b.velocity;
//   Vec p1 = a.position;
//   Vec p2 = b.position;
// 
//   std::cout << vec3_repr(v1) << " " << vec3_repr(v2) << std::endl;
//   std::pair<Vec, Vec> close_points = get_points_of_closest_approach(p1, v1, p2, v2);
//   Vec c1 = close_points.first;
//   Vec c2 = close_points.second;
//   double closest_distance = vec3_magnitude(vec3_diff(c1, c2));
//   std::cout << vec3_repr(c1) << " " << vec3_repr(c2) << " " << closest_distance << std::endl;
//   double thresh_distance = (a.diameter/2) + (b.diameter/2);
// 
//   double d1 = vec3_magnitude(vec3_diff(c1, p1));
//   double d2 = vec3_magnitude(vec3_diff(c2, p2));
// 
//   if (std::abs(d1 - d2) > thresh_distance)
//     return false; // balls will miss each other
// 
//   double t1 = d1 / vec3_magnitude(v1);
//   double t2 = d2 / vec3_magnitude(v2);
// 
//   return closest_distance <= thresh_distance;
// }

double time_to_collide(const Ball &a, const Ball &b)
{
  Vec dV = a.velocity - b.velocity;
  Vec dP = a.position - b.position;
  double A = dV.dot(dV);
  double B = 2.0*dV.dot(dP);
  double C = dP.dot(dP) - ((a.diameter+b.diameter)/2.0) ;

  double discriminant = (B*B) - (4*A*C);

  std::cout << discriminant << std::endl;

  if (discriminant < 0.0)
    return -1.0;

  double sqr_discriminant = std::pow(discriminant, 0.5);

  double bigsol = (-B + sqr_discriminant) / (2.0 * A);
  double lilsol = (-B - sqr_discriminant) / (2.0 * A);

  std::cout << bigsol << " " << lilsol << std::endl;
  if (bigsol < lilsol)
    return bigsol;
  return lilsol;
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
  //std::cout << lines_intersect(a, b) << std::endl;
  time_to_collide(a, b);

}
