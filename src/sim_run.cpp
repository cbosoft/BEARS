#include <iostream>
#include <csignal>
#include <chrono>
#include <future>
#include <set>

#include "sim.hpp"
#include "colour.hpp"
#include "event.hpp"


struct par_event_check_out {
  EventCollection events;
};

struct par_event_check_in {
  Sim* sim;
  std::vector<unsigned int> invalid_indices;
};


static struct par_event_check_out *parallelCollisionCheckWorker(struct par_event_check_in *input)
{
  struct par_event_check_out *output = new struct par_event_check_out;
  
  for (auto i : input->invalid_indices) {
    auto a = input->sim->get_ball(i);
    for (unsigned int j = 0; j < i; j++) {
      auto b = input->sim->get_ball(j);

      if (a == b)
        continue;

      if (auto *event_ptr = a->check_will_collide_minimum_image(b, input->sim->get_side_length() )) {
        output->events.push_back(event_ptr);
      }

    }
  }

  return output;
}

void Sim::parallel_update_events(std::set<unsigned int> invalid_indices)
{
  unsigned int nchunks = this->nthreads;

  if (invalid_indices.size() % nchunks != 0)
    nchunks++;

  unsigned int chunklen = invalid_indices.size() / nchunks;

  std::vector<std::future<struct par_event_check_out *>> async_threads;
  std::vector<struct par_event_check_in *> inputs;
  auto beg_it = invalid_indices.begin();
  auto end_it = invalid_indices.begin();
  std::advance(end_it, chunklen);
  for (unsigned int i = 0; i < nchunks; i++) {
    struct par_event_check_in *input = new struct par_event_check_in;
    std::vector<unsigned int> invalid_indices_chunk(beg_it, end_it);
    std::advance(beg_it, chunklen);
    std::advance(end_it, chunklen);
    input->invalid_indices = invalid_indices_chunk;
    input->sim = this;
    inputs.push_back(input);
    async_threads.push_back(std::async(parallelCollisionCheckWorker, input));
  }

  for (unsigned int i = 0; i < nchunks; i++) {

    // get thread's result, merge into master event list
    struct par_event_check_out *output = async_threads[i].get();
    this->events.merge(output->events);

    // tidy up memory
    delete inputs[i];
    delete output;
  }

}

void Sim::linear_update_events(std::set<unsigned int> invalid_indices)
{
  for (auto i : invalid_indices) {
    auto a = this->balls[i];
    for (unsigned int j = 0; j < i; j++) {
      auto b = this->balls[j];
      if (auto *event_ptr = a->check_will_collide_minimum_image(b, this->side_length, this->time)) {
        this->events.push_back(event_ptr);
      }
    }
  }
}

static int removed = 0;
void Sim::update_events()
{
  std::set<unsigned int> invalid_indices;

  // only reprocess events for affected particles
  if (this->events.size()) {

    invalid_indices = this->events.invalidate();

  }
  else {

    for (auto ball : this->balls) {
      invalid_indices.insert(ball->get_id()-1);
    }

  }

  if (this->nthreads > 1) {
    this->parallel_update_events(invalid_indices);
  }
  else {
    this->linear_update_events(invalid_indices);
  }

  this->events.sort(event_compare_f);
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
  // double ptime = 0.0;

  double event_duration = -1.0, output_duration = -1.0;

  while (!done) {
#define CLOCK std::chrono::steady_clock
    {
      auto before = CLOCK::now();
      this->update_events();
      auto after = CLOCK::now();
      event_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
      before = after;
    }
#undef CLOCK


    if (!this->events.size()) {
      std::cerr << "No more events." << std::endl;
      double dt = 5.0;
      for (auto b: this->balls)
        b->timejump(dt);
      this->time += dt;
      this->append_to_trajectory();
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

#define CLOCK std::chrono::steady_clock
    {
      auto before = CLOCK::now();
      this->append_to_trajectory(a->get_id(), b->get_id());
      auto after = CLOCK::now();
      output_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
      before = after;
    }
#undef CLOCK

    // display some statistics
    std::cerr << "t= " << this->time 
      << " ed= " << event_duration 
      << " od= " << output_duration 
      << " ne= " << this->events.size()
      << " re= " << removed 
      << std::endl;

    // check if exit condition is satisfied
    done = ((timed and (this->time > end_time)) or (cancelled));
  }

  std::signal(SIGINT, SIG_DFL);
}
