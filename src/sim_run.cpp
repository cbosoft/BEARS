#include <iostream>
#include <fstream>
#include <csignal>
#include <chrono>
#include <future>
#include <set>

#include "sim.hpp"
#include "colour.hpp"
#include "event.hpp"
#include "threadsafe_queue.hpp"
#include "version.hpp"

#define CLOCK std::chrono::steady_clock


struct par_event_check_out {
  EventCollection events;
};

struct par_event_check_in {
  Sim* sim;
  CollisionEvent *ev;
};

static SafeQueue<Ball> ballq;


static struct par_event_check_out *parallelCollisionCheckWorker(struct par_event_check_in *input)
{
  struct par_event_check_out *output = new struct par_event_check_out;

  Ball *ball = NULL;
  while ( (ball = ballq.pop()) ) {
    output->events.push_back(ball->update_and_get_next(input->ev));
  }

  return output;
}

void Sim::parallel_update_events(CollisionEvent *ev)
{

  ballq.clear();

  for (Ball *ball : this->balls) {
    ballq.push(ball);
  }

  std::vector<std::future<struct par_event_check_out *>> async_threads;
  std::vector<struct par_event_check_in *> inputs;
  for (int i = 0; i < this->nthreads; i++) {
    struct par_event_check_in *input = new struct par_event_check_in;
    input->ev = ev;
    input->sim = this;
    inputs.push_back(input);
    async_threads.push_back(std::async(parallelCollisionCheckWorker, input));
    //std::cerr << "Launching thread " << i << std::endl;
  }

  for (unsigned int i = 0; i < async_threads.size(); i++) {

    // get thread's result, merge into master event list
    struct par_event_check_out *output = async_threads[i].get();
    this->events.merge(output->events);

    // tidy up memory
    delete inputs[i];
    delete output;
  }

}

void Sim::linear_update_events(CollisionEvent *ev)
{

  for (Ball *a : this->balls) {
    this->events.push_back(a->update_and_get_next(ev));
  }

}

void Sim::update_events(CollisionEvent *ev)
{

  if (this->nthreads > 1) {
    this->parallel_update_events(ev);
  }
  else {
    this->linear_update_events(ev);
  }

}



static volatile bool cancelled;

static void handler(int signal)
{
  (void) signal;

  static int i = 0;
  
  if (i++ > 10) {
    std::cerr << FG_RED << BOLD << "Cancelled." << RESET << std::endl;
    exit(1);
  }

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

  double event_duration = -1.0, output_duration = -1.0;
  CollisionEvent *event = nullptr;

  double total_time_to_update = 0.0;
  int number_updates = 0;

  while (!done) {

    {
      auto before = CLOCK::now();
      this->update_events(event);
      auto after = CLOCK::now();
      event_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
      before = after;

      total_time_to_update += event_duration;
      number_updates ++;
    }


    if (!this->events.size()) {
      std::cerr << "No more events." << std::endl;
      double dt = 5.0;
      for (auto b: this->balls)
        b->timejump(dt);
      this->time += dt;
      this->append_to_trajectory();
      break;
    }

    event = this->events.front();
    double dt = event->get_time();

    Ball *a = event->get_a();
    Ball *b = event->get_b();

    // particle interacts with specific image
    //b->set_image(event->get_image());

    // update all particle positions to the time of the event
    for (auto b: this->balls)
      b->timejump(dt);
    this->time += dt;

    // update the interacting particle velocities and stuff
    a->collide(b);

    {
      auto before = CLOCK::now();
      this->append_to_trajectory(a->get_id(), b->get_id());
      auto after = CLOCK::now();
      output_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
      before = after;
    }

    // display some statistics
    std::cerr << "t= " << this->time 
      << " ed= " << event_duration 
      << " od= " << output_duration 
      << " ne= " << this->events.size()
      << std::endl;

    // check if exit condition is satisfied
    done = ((timed and (this->time > end_time)) or (cancelled));
  }

  std::signal(SIGINT, SIG_DFL);

  std::ofstream ostr("benchmark.csv", std::ios::app);

  if (ostr.fail()) {
    ostr.open("benchmark.csv");
    if (ostr.fail()) {
      std::cerr << "Could not open \"benchmark.csv\" for write." << std::endl;
    }
  }
  ostr << BRANCH << "," << this->nthreads << "," << this->balls.size() << "," << (total_time_to_update / number_updates) << std::endl;
}
