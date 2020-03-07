#include <iostream>
#include <csignal>
#include <future>

#include "sim.hpp"

static bool event_compare_f(const CollisionEvent *a, const CollisionEvent *b)
{
  return a->get_time() < b->get_time();
}


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

      for (auto image : input->sim->images) {
        CollisionCheckResult *cer = a->check_will_collide_image(b, image);
        if (cer->will_occur) {
          output->events.push_back(cer->event);
        }
        delete cer;
      }

    }
  }

  return output;
}

void Sim::parallel_update_events()
{
  unsigned int nchunks = this->nthreads;

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
}

void Sim::linear_update_events()
{
  for (unsigned int i = 0; i < this->balls.size(); i++) {
    auto a = this->balls[i];
    for (unsigned int j = 0; j < i; j++) {
      auto b = this->balls[j];
      for (auto image : this->images) {
        CollisionCheckResult *cer = a->check_will_collide_image(b, image);
        if (cer->will_occur) {
          this->events.push_back(cer->event);
        }
        delete cer;
      }
    }
  }
}


void Sim::update_events()
{
  this->clear_events();

  if (this->nthreads > 1) {
    this->parallel_update_events();
  }
  else {
    this->linear_update_events();
  }

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
    std::cerr << "Running until cancelled or events run out." << std::endl;
    timed = false;
  }
  else {
    std::cerr << "Running until sim time exceeds " << end_time << " units or events run out." << std::endl;
  }

  if (this->nthreads > 1) {
    std::cerr << "Parallel computation enabled." << std::endl;
    std::cerr << "Using " << this->nthreads << " threads." << std::endl;
  }

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
    double dt = event->get_time();

    Ball *a = event->get_a();
    Ball *b = event->get_b();

    // particle interacts with specific image
    b->set_image(event->get_image());

    // update all particle positions to the time of the event
    for (auto b: this->balls)
      b->timejump(dt);
    this->time += dt;

    // update the interacting particle velocities and stuff
    a->collide(b);

    std::cerr << this->time << std::endl;

    this->append_to_trajectory();

    // check if exit condition is satisfied
    done = ((timed && (this->time > end_time)) || (cancelled));
  }

  std::signal(SIGINT, SIG_DFL);
}
