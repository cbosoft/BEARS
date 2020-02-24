#include <iostream>

#include "sim.hpp"
#include "event.hpp"


void Sim::update_events()
{
  this->clear_events();

  auto a = this->balls.begin();
  auto b = this->balls.begin();
  for (int i = 0; a != this->balls.end(); i++, a++) {
    for (int j = 0; b != this->balls.end() && j < i; j++, b++) {
      CollisionCheckResult *cer = collision_check(*a, *b);
      if (cer->will_occur) {
        this->events.push_back(cer->event);
      }
    }
  }

  this->events.sort();
}


void Sim::clear_events()
{
  for (auto event : this->events) {
    free(event);
  }
  this->events.erase(this->events.begin(), this->events.end());
}



void Sim::run(double end_time)
{
  int i = 0;
  while (this->time < end_time) {
    this->update_events();

    std::cerr << this->events.size() << std::endl;
    if (!this->events.size()) {
      std::cerr << "no events" << std::endl;
      break;
    }

    auto event = this->events.front();
    for (auto b: this->balls) {
      b->position = b->position + b->velocity*event->get_time();
    }
    this->time += event->get_time();

    // update the interacting particle velocities and stuff
    auto a = event->get_a();
    auto b = event->get_b();
    double totmass = a->mass + b->mass;
    a->velocity = (a->velocity*((a->mass - b->mass)/totmass)) + (b->velocity*(2.0*b->mass/totmass) );
    b->velocity = (b->velocity*((b->mass - a->mass)/totmass)) + (a->velocity*(2.0*a->mass/totmass) );
    // Vec temp = a->velocity;
    // a->velocity = b->velocity;
    // b->velocity = temp;

    std::cerr << i++ << ") " << this->time << " " << a->velocity.repr() << "  " << b->velocity.repr() << std::endl;
    if (i > 3)
      break;
  }
}
