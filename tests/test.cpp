#include <iostream>

#include "../src/ball.hpp"
#include "../src/colour.hpp"

#define DEF_TEST_FN(OP,SYMBOL,FNNAME) \
  template<typename T>\
  void FNNAME(const char *testname, T lhs, T rhs) \
  {\
    std::cerr << testname << " " << lhs << " " << SYMBOL << " " << rhs << " ? ";\
    if (lhs OP rhs) {\
      std::cerr << FG_GREEN "PASSED" RESET << std::endl;\
      return;\
    }\
    std::cerr << BG_RED "FAILED" RESET << std::endl;\
    exit(1);\
  }\

DEF_TEST_FN(==,"==",test_eq)
DEF_TEST_FN(!=,"!=",test_neq)
DEF_TEST_FN(<,"<",test_lt)
DEF_TEST_FN(>,">",test_gt)
DEF_TEST_FN(<=,">=",test_leq)
DEF_TEST_FN(>=,">=",test_geq)

template<typename T>
void test_approx(const char *testname, T lhs, T rhs, T thresh) 
{
  std::cerr << testname << " " << lhs << " ≈ " << rhs << " ? ";
  if (lhs - thresh < rhs and rhs < lhs + thresh) {
    std::cerr << FG_GREEN "PASSED" RESET << std::endl;
    return;
  }
  std::cerr << BG_RED "FAILED" RESET << std::endl;
  exit(1);
}

int main(void)
{
  BallConstructorData bcd = {
    .id=0, 
    .diameter=1.0, 
    .mass=1.0, 
    .inertia=0.1, 
    .roughness=0.0, 
    .position={0, 0, 0},
    .orientation={0, 0, 0},
    .velocity={0, 0, 0},
    .angular_velocity={0, 0, 0},
    .force={0, 0, 0},
    .torque={0, 0, 0}
  };

  {
    Ball *a = new Ball(bcd);
    bcd.position = {3, 0, 0}; // 2 units between particles in x
    bcd.velocity = {-1, 0, 0}; // 1 unit distance per unit time: 2 time units to collide
    Ball *b = new Ball(bcd);

    CollisionEvent *ev = a->check_will_collide_minimum_image(b, 100.0, 0.0);
    test_neq("1. " BOLD "ballistic pair " RESET "(event occurs)", ev, (CollisionEvent *)nullptr);
    test_eq("2. " BOLD "ballistic pair " RESET "(event time correct)", ev->get_time(), 2.0);
    delete ev;
    delete a;
    delete b;
  }

  {
    bcd.position = {0, 0, 0};
    bcd.velocity = {0, 0, 0};
    Ball *a = new Ball(bcd);
    bcd.position = {3, 0, 0.99999}; // 2 units between particles in x, 1 in z
    bcd.velocity = {-1, 0, 0}; // 1 unit distance per unit time: 3 time units to collide
    Ball *b = new Ball(bcd);
    CollisionEvent *ev = a->check_will_collide_minimum_image(b, 100.0, 0.0);
    test_neq("3. " BOLD "ballistic pair (glancing) " RESET "(event occurs)", ev, (CollisionEvent *)nullptr);
    test_approx("4. " BOLD "ballistic pair (glancing) " RESET "(event time correct)", ev->get_time(), 3.0, 0.01);
    delete ev;
    delete a;
    delete b;
  }

  {
    bcd.position = {0, 0, 0};
    bcd.velocity = {0, 0, 0};
    Ball *a = new Ball(bcd);
    bcd.position = {3, 0, 3}; // 2 units between particles in x, 2 in z
    bcd.velocity = {-1, 0, 0}; // particles will never collide
    Ball *b = new Ball(bcd);
    CollisionEvent *ev = a->check_will_collide_minimum_image(b, 100.0, 0.0);
    test_eq("5. " BOLD "ballistic pair (miss) " RESET "(event does not occur)", ev, (CollisionEvent *)nullptr);
    delete a;
    delete b;
  }

  {
    bcd.position = {1, 1, 1};
    bcd.velocity = {0, 0, 0};
    Ball *a = new Ball(bcd);
    bcd.position = {7, 1, 1}; // 6 units between particles in x: actually only 3
    bcd.velocity = {1, 0, 0}; // 1 unit distance per unit time: 3 time units to collide
    Ball *b = new Ball(bcd);
    CollisionEvent *ev = a->check_will_collide_minimum_image(b, 10.0, 0.0);
    test_neq("6. " BOLD "ballistic pair (periodic 1) " RESET "(event occurs)", ev, (CollisionEvent *)nullptr);
    test_eq("7. " BOLD "ballistic pair (periodic 1) " RESET "(event time correct)", ev->get_time(), 3.0);
    delete ev;
    delete a;
    delete b;
  }

  {
    bcd.position = {1, 1, 1};
    bcd.velocity = {0, 0, 0};
    Ball *a = new Ball(bcd);
    bcd.position = {13, 1, 1}; // 12 units between particles in x: actually only 1
    bcd.velocity = {-1, 0, 0}; // 1 unit distance per unit time: 1 time units to collide
    Ball *b = new Ball(bcd);
    CollisionEvent *ev = a->check_will_collide_minimum_image(b, 10.0, 0.0);
    test_neq("8. " BOLD "ballistic pair (periodic 2) " RESET "(event occurs)", ev, (CollisionEvent *)nullptr);
    test_eq("9. " BOLD "ballistic pair (periodic 2) " RESET "(event time correct)", ev->get_time(), 1.0);
    delete ev;
    delete a;
    delete b;
  }
}
