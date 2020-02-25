#include <iostream>
#include <csignal>

#ifdef PARALLEL
#include <future>
#endif

#include "sim.hpp"

static bool event_compare_f(const CollisionEvent *a, const CollisionEvent *b)
{
  return a->get_time() < b->get_time();
}

#ifdef PARALLEL

struct par_event_check_out {
  std::list<CollisionEvent *> events;
};

struct par_event_check_in {
  Sim *sim;
  unsigned int to;
  unsigned int from;
};


static struct par_event_check_out *parallelCollisionCheckWorker(struct par_event_check_in *input)
{
  struct par_event_check_out *output = new struct par_event_check_out;
  output->events = std::list<CollisionEvent *>();
  
  for (unsigned int i = input->from; i < input->to; i++) {
    auto a = input->sim->get_ball(i);
    for (unsigned int j = 0; j < i; j++) {
      auto b = input->sim->get_ball(j);

      if (a == b)
        continue;

      CollisionCheckResult *cer = a->check_will_collide(b);
      if (cer->will_occur) {
        output->events.push_back(cer->event);
      }
      delete cer;
    }
  }

  return output;
}

#endif

void Sim::update_events()
{
  this->clear_events();

#ifdef PARALLEL
  unsigned int nchunks = N_THREADS;

  if (this->balls.size() % nchunks != 0)
    nchunks++;

  unsigned int chunklen = this->balls.size() / nchunks;

  std::vector<std::future<struct par_event_check_out *>> async_threads;
  std::vector<struct par_event_check_in *> inputs;
  for (unsigned int i = 0; i < nchunks; i++) {
    struct par_event_check_in *input = new struct par_event_check_in;
    input->sim = this;
    input->from = i*chunklen;
    input->to = (i+1)*chunklen;
    if ((i+1) > (this->balls.size()-1))
      input->to = this->balls.size()-1;
    inputs.push_back(input);
    async_threads.push_back(std::async(parallelCollisionCheckWorker, input));
  }

  for (unsigned int i = 0; i < nchunks; i++) {
    struct par_event_check_out *output = async_threads[i].get();
    delete inputs[i];
    for (auto event : output->events) {
      this->events.push_back(event);
    }
    delete output;
  }

#else

  for (unsigned int i = 0; i < this->balls.size(); i++) {
    auto a = this->balls[i];
    for (unsigned int j = 0; j < i; j++) {
      auto b = this->balls[j];
      CollisionCheckResult *cer = a->check_will_collide(b);
      if (cer->will_occur) {
        this->events.push_back(cer->event);
      }
      delete cer;
    }
  }

#endif

  this->events.sort(event_compare_f);
}


void Sim::clear_events()
{
  for (auto event : this->events) {
    delete event;
  }
  this->events.erase(this->events.begin(), this->events.end());
}



static volatile bool cancelled;

static void handler(int signal)
{
  (void) signal;

  static int i = 0;
  
  if (i++ > 10)
    exit(1);

  cancelled = true;
}



void Sim::run(double end_time)
{
  bool done = false, timed = true;

  if (end_time < 0.0) {
    std::cerr << "running until cancelled" << std::endl;
    timed = false;
  }
  else {
    std::cerr << "running until sim time exceeds " << end_time << " units" << std::endl;
  }

#ifdef PARALLEL
  std::cerr << "parallel computation enabled" << std::endl;
  std::cerr << "using " << N_THREADS << " threads" << std::endl;
#endif

  std::signal(SIGINT, handler);
  this->time = 0.0;
  // double ptime = 0.0;

  while (!done) {
    this->update_events();

    if (!this->events.size()) {
      std::cerr << "no events" << std::endl;
      break;
    }

    auto event = this->events.front();
    for (auto b: this->balls) {
      b->position = b->position + (b->velocity*event->get_time());
    }
    this->time += event->get_time();

    // update the interacting particle velocities and stuff
    auto a = event->get_a();
    auto b = event->get_b();
    double totmass = a->mass + b->mass;
    Vec new_a_velocity = (a->velocity*((a->mass - b->mass)/totmass)) + (b->velocity*(2.0*b->mass/totmass) );
    Vec new_b_velocity = (b->velocity*((b->mass - a->mass)/totmass)) + (a->velocity*(2.0*a->mass/totmass) );
    a->velocity = new_a_velocity;
    b->velocity = new_b_velocity;
    // TODO friction, forces, torque, rotation?

    std::cerr << this->time << std::endl;

    // TODO append step to output
    this->append_to_trajectory();

    done = ((timed && (this->time > end_time)) || (cancelled));
  }

  std::signal(SIGINT, SIG_DFL);
}
